#include <sys/select.h>
#include <sys/types.h>
#include <unistd.h>

#include <libgadu.h>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/locale.hpp>
#include <Wt/WSignal>

#include <vector>
#include <map>

#include "pugixml.hpp"

#include "ggwrapper.h"
#include "synchronizedqueue.h"
#include "ggevent.h"
#include "ggloginevent.h"
#include "logger.h"

#include "messageevent.h"
#include "loginresultevent.h"
#include "TypingNotificationEvent.h"
#include "contactinfo.h"
#include "ContactGroup.h"
#include "ggmessageevent.h"
#include "ggTypingEvent.h"

#include "CharacterConversion.h"

#include "ContactImportEvent.h"
typedef boost::shared_ptr<Event> spEvent;
GGWrapper::GGWrapper()
    :
      mpSession(NULL),
      mpQueue( new SynchronizedQueue<ggEvent>()),
      mpEventSignal(new Wt::Signal<boost::shared_ptr<Event>>())
{
    gg_debug_level = 255;
    mWorkerThread = boost::thread(boost::bind(&GGWrapper::enterLoop,this));
    mTimeSinceLastPing = boost::posix_time::second_clock::local_time();
}
GGWrapper::~GGWrapper()
{
    delete mpEventSignal;
    mWorkerThread.interrupt();
    mWorkerThread.join();


    if(mpSession)
    {
        gg_logoff(mpSession);
        gg_free_session(mpSession);
    }

    delete mpQueue;
}

void GGWrapper::connect(unsigned int uin, const std::string &pass)
{
    addToEventLoop(ggEvent(ggEvent::LoginEvent,ggLoginEvent(uin,pass)));
}
gg_event* GGWrapper::pollGGEvent()
{
    if(!mpSession) return 0;
    fd_set rd, wd;
    FD_ZERO(&rd);
    FD_ZERO(&wd);
    timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0.05;
    if ((mpSession->check & GG_CHECK_READ))
            FD_SET(mpSession->fd, &rd);
    if ((mpSession->check & GG_CHECK_WRITE))
            FD_SET(mpSession->fd, &wd);

    select(mpSession->fd + 1,&rd,&wd,NULL,&tv);

    if(FD_ISSET(mpSession->fd, &rd) || FD_ISSET(mpSession->fd, &wd))
    {
        return gg_watch_fd(mpSession);
    }
    else
    {
        return 0;
    }
}

void GGWrapper::enterLoop()
{
    while(!mWorkerThread.interruption_requested())
    {
        boost::any item = mpQueue->poll();
        if(item.empty())
        {
            //Logger::log("Looping\r\n");
            gg_event *event = pollGGEvent();
            if(event != 0)
            {
                processGGEvent(*event);
                gg_free_event(event);
            }
        }
        else
        {
            processEvent(boost::any_cast<ggEvent&>(item));
        }

        sendPing();

    }
    Logger::log("Finished Looping!");
    //probably here some cleaning or just do everything in destructor;
}


void GGWrapper::sendMessage(unsigned int targetUin, const std::string &content)
{
    std::string toLog("Sending msg: " + content);
    toLog += "\n Length was: ";
    toLog += boost::lexical_cast<std::string>(content.size());
    auto message = conversions::fromUtf8(content);
    for(auto it = message.cbegin(); it != message.cend(); ++it)
    {
        char c = *it;
        int intValue = (int)c;
        toLog += std::string("Character : ") + c + "has value of " + boost::lexical_cast<std::string>(intValue) + " \n";
    }
    Logger::log(toLog);
    addToEventLoop(ggEvent(ggEvent::MessageEvent,ggMessageEvent(targetUin,message)));
}
void GGWrapper::sendTypingNotification(unsigned int targetUin, int length)
{
    addToEventLoop(ggEvent(ggEvent::TypingNotificationEvent,ggTypingEvent(targetUin,length)));
}

void GGWrapper::sendPing()
{
    if(mpSession == NULL)
        return;
    auto timeNow = boost::posix_time::second_clock::local_time();
    boost::posix_time::time_duration durationSinceLastPing(timeNow - mTimeSinceLastPing);
    if(durationSinceLastPing.minutes() > 2)
    {
        gg_ping(mpSession);
        mTimeSinceLastPing = timeNow;
    }
}

void GGWrapper::onRecvMsg(gg_event_msg& msg)
{
    std::string toLog("Received Message from: ");
    toLog += boost::lexical_cast<std::string>(msg.sender);
    toLog +="containing :" ;
    toLog += boost::lexical_cast<std::string>(msg.msgclass);
    if(msg.msgclass & GG_CLASS_CHAT)
    {
        unsigned int a;
        toLog += "\n content: ";
        std::string message((const char*)msg.message);
        toLog += "\n lengthWas: ";
        toLog += boost::lexical_cast<std::string>(message.size());
        toLog += " ";
        for(auto it = message.cbegin(); it != message.cend(); ++it)
        {
            char c = *it;
            int intValue = (int)c;
            toLog += std::string("Character : ") + c + "has value of " + boost::lexical_cast<std::string>(intValue) + " \n";
        }
        toLog += boost::lexical_cast<std::string>(a = (unsigned int)message.c_str()[0]);
        toLog += "\n";
        toLog += message;
        std::string decodedMsg = conversions::toUtf8(message);
        eventSignal().emit(spEvent(new MessageEvent(msg.sender,decodedMsg)));
    }

    //toLog += std::string((const char*)msg.message);
    Logger::log(toLog);
}
void GGWrapper::onRecvOwnInfo(gg_event_user_data &data)
{
    return;
    //for future
    int userCount = data.user_count;
    std::vector<ContactInfo> contacts;
    for(int userIndex = 0 ; userIndex < userCount ; ++userIndex)
    {
        std::map<std::string,std::string> attributes;
        gg_event_user_data_user &userData = data.users[userIndex];
        //Logger::log(std::string("info from: ") + boost::lexical_cast<std::string>(userData.uin));
        for(unsigned int attriIndex = 0 ; attriIndex < userData.attr_count;  ++attriIndex)
        {
            gg_event_user_data_attr &attri =  userData.attrs[attriIndex];
            std::string attriKey(attri.key);
            std::string attriValue(attri.value);
        }

    }
}
void GGWrapper::onRecvContacts(gg_event_userlist100_reply& data)
{
    std::map<std::string,bool> convertSBMap;
    convertSBMap.insert(std::make_pair("true",1));
    convertSBMap.insert(std::make_pair("false",0));
    pugi::xml_document doc;
    doc.load(data.reply);
    auto contactBookNode  = doc.child("ContactBook");
    auto groups = contactBookNode.child("Groups");
    auto contacts = contactBookNode.child("Contacts");
    std::list<ContactGroup> groupList;
    std::map<std::string,std::vector<ContactInfo>> mapContacts;
    for(auto it = contacts.begin(); it != contacts.end(); ++it)
    {
        std::string guid = it->child("Guid").child_value(); // dunno what it is
        auto ggNode = it->child("GGNumber");
        if(strlen(ggNode.child_value()) == 0)
            continue;
        unsigned int uin = boost::lexical_cast<unsigned int>(ggNode.child_value());
        std::string showName = it->child("ShowName").child_value();
        std::string nickName = it->child("NickName").child_value();
        bool isBuddy = convertSBMap[it->child("FlagBuddy").child_value()];
        bool isNormal = convertSBMap[it->child("FlagNormal").child_value()];
        bool isFriend = convertSBMap[it->child("FlagFriend").child_value()];
        std::string groupId = it->child("Groups").child("GroupId").child_value();
        Logger::log(groupId + " groupIdContact");
        mapContacts[groupId].push_back(ContactInfo(uin,showName,nickName,isBuddy,isNormal,isFriend));
    }
    for(auto it = groups.begin(); it != groups.end(); ++it)
    {
        std::string id = it->child("Id").child_value();
        auto groupName = it->child("Name").child_value();
        auto isExpanded = convertSBMap[it->child("IsExpanded").child_value()];
        auto isRemovable = convertSBMap[it->child("IsRemovable").child_value()];
        Logger::log(id + " groupIdGroups");
        groupList.push_back(ContactGroup(isExpanded,isRemovable,groupName,mapContacts[id]));
    }

    mpEventSignal->emit(spEvent(new ContactImportEvent(groupList)));
}

void GGWrapper::onRecvTypingNotification(gg_event_typing_notification& typingNotification)
{
    mpEventSignal->emit(spEvent(new TypingNotificationEvent(typingNotification.uin,typingNotification.length)));
}

void GGWrapper::processGGEvent(gg_event &ev)
{

    switch(ev.type)
    {
        case GG_EVENT_NONE:		/**< Nie wydarzylo sie nic wartego uwagi */
            Logger::log("Received GG_EVENT_NONE\n");

            break;
        case GG_EVENT_MSG:			/**< \brief Otrzymano wiadomosc. Przekazuje równiez wiadomosci systemowe od numeru 0. */
            onRecvMsg(reinterpret_cast<gg_event_msg&>(ev.event));
            break;
        case GG_EVENT_NOTIFY:		/**< \brief Informacja o statusach osób z listy kontaktów (przed 6.0). Zdarzenie nalezy obslugiwac, jesli planuje sie uzywac protokolu w wersji starszej niz domyslna. Ostatni element tablicy zawiera uin równy 0, a pozostale pola sa niezainicjowane. */
            Logger::log("Received GG_EVENT_NOTIFY\n");
            break;
        case GG_EVENT_NOTIFY_DESCR:		/**< \brief Informacja o statusie opisowym osoby z listy kontaktów (przed 6.0). Zdarzenie nalezy obslugiwac, jesli planuje sie uzywac protokolu w wersji starszej niz domyslna. */
            Logger::log("Received GG_EVENT_NOTIFY_DESCR\n");
            break;
        case GG_EVENT_STATUS:		/**< \brief Zmiana statusu osoby z listy kontaktów (przed 6.0). Zdarzenie nalezy obslugiwac, jesli planuje sie uzywac protokolu w wersji starszej niz domyslna. */
            Logger::log("Received GG_EVENT_STATUS\n");
            break;
        case GG_EVENT_ACK:			/**< Potwierdzenie doreczenia wiadomosci */

            Logger::log("Received GG_EVENT_ACK\n");
            break;
        case GG_EVENT_PONG:			/**< \brief Utrzymanie polaczenia. Obecnie serwer nie wysyla juz do klienta ramek utrzymania polaczenia, polega wylacznie na wysylaniu ramek przez klienta. */
            Logger::log("Received GG_EVENT_PONG\n");
            break;
        case GG_EVENT_CONN_FAILED:		/**< \brief Nie udalo sie polaczyc */
            Logger::log("Received GG_EVENT_CONN_FAILED\n");
            eventSignal().emit(spEvent(new LoginResultEvent(false)));
            mpSession = 0;
            throw std::exception();
            //gg_free_session(mpSession);

            break;
        case GG_EVENT_CONN_SUCCESS:		/**< \brief Polaczono z serwerem. Pierwsza rzecza, jaka nalezy zrobic jest wyslanie listy kontaktów. */
            Logger::log("Received GG_EVENT_CONN_SUCCESS\n");
            break;
        case GG_EVENT_DISCONNECT:		/**< \brief Serwer zrywa polaczenie. Zdarza sie, gdy równolegle do serwera podlaczy sie druga sesja i trzeba zerwac polaczenie z pierwsza. */
            Logger::log("Received GG_EVENT_DISCONNECT\n");
            break;

        case GG_EVENT_DCC_NEW:		/**< Nowe polaczenie bezposrednie (6.	) */
            Logger::log("Received GG_EVENT_DCC_NEW\n");
            break;
        case GG_EVENT_DCC_ERROR:		/**< Blad polaczenia bezposredniego (6.	) */
            Logger::log("Received GG_EVENT_DCC_ERROR\n");
            break;
        case GG_EVENT_DCC_DONE:		/**< Zakonczono polaczenie bezposrednie (6.	) */
            Logger::log("Received GG_EVENT_DCC_DONE\n");
            break;
        case GG_EVENT_DCC_CLIENT_ACCEPT:	/**< Moment akceptacji klienta w polaczeniu bezposrednim (6.	) */
            Logger::log("Received GG_EVENT_DCC_CLIENT_ACCEPT\n");
            break;
        case GG_EVENT_DCC_CALLBACK:		/**< Zwrotne polaczenie bezposrednie (6.	) */
            Logger::log("Received GG_EVENT_DCC_CALLBACK\n");
            break;
        case GG_EVENT_DCC_NEED_FILE_INFO:	/**< Nalezy wypelnic \c file_info dla polaczenia bezposredniego (6.	) */
            Logger::log("Received GG_EVENT_DCC_NEED_FILE_INFO\n");
            break;
        case GG_EVENT_DCC_NEED_FILE_ACK:	/**< Czeka na potwierdzenie pliku w polaczeniu bezposrednim (6.	) */
            Logger::log("Received GG_EVENT_DCC_NEED_FILE_ACK\n");
            break;
        case GG_EVENT_DCC_NEED_VOICE_ACK:	/**< Czeka na potwierdzenie rozmowy w polaczeniu bezposrednim (6.	) */
            Logger::log("Received GG_EVENT_DCC_NEED_VOICE_ACK\n");
            break;
        case GG_EVENT_DCC_VOICE_DATA: 	/**< Dane bezposredniego polaczenia glosowego (6.	) */
            Logger::log("Received GG_EVENT_DCC_VOICE_DATA\n");
            break;

        case GG_EVENT_PUBDIR50_SEARCH_REPLY:	/**< Odpowiedz katalogu publicznego */
            Logger::log("Received GG_EVENT_PUBDIR50_SEARCH_REPLY\n");
            break;
        case GG_EVENT_PUBDIR50_READ:		/**< Odczytano wlasne dane z katalogu publicznego */
            Logger::log("Received GG_EVENT_PUBDIR50_READ\n");
            break;
        case GG_EVENT_PUBDIR50_WRITE:	/**< Zmieniono wlasne dane w katalogu publicznym */
            Logger::log("Received GG_EVENT_PUBDIR50_WRITE\n");
            break;

        case GG_EVENT_STATUS60:		/**< Zmiana statusu osoby z listy kontaktów */
            Logger::log("Received GG_EVENT_STATUS60\n");
            break;
        case GG_EVENT_NOTIFY60:		/**< Informacja o statusach osób z listy kontaktów. Ostatni element tablicy zawiera uin równy 0, a pozostale pola sa niezainicjowane.  */
            Logger::log("Received GG_EVENT_NOTIFY60\n");
            break;
        case GG_EVENT_USERLIST:		/**< Wynik importu lub eksportu listy kontaktów */
            Logger::log("Received GG_EVENT_USERLIST\n");
            break;
        case GG_EVENT_IMAGE_REQUEST:		/**< Zadanie przeslania obrazka z wiadomosci */
            Logger::log("Received GG_EVENT_IMAGE_REQUEST\n");
            break;
        case GG_EVENT_IMAGE_REPLY:		/**< Przyslano obrazek z wiadomosci */
            Logger::log("Received GG_EVENT_IMAGE_REPLY\n");
            break;
        case GG_EVENT_DCC_ACK:		/**< Potwierdzenie transmisji w polaczeniu bezposrednim (6.	) */
            Logger::log("Received GG_EVENT_DCC_ACK\n");
            break;

        case GG_EVENT_DCC7_NEW:		/**< Nowe polaczenie bezposrednie (7.	) */
            Logger::log("Received GG_EVENT_DCC7_NEW\n");
            break;
        case GG_EVENT_DCC7_ACCEPT:		/**< Zaakceptowano polaczenie bezposrednie (7.	), nowy deskryptor */
            Logger::log("Received GG_EVENT_DCC7_ACCEPT\n");
            break;
        case GG_EVENT_DCC7_REJECT:		/**< Odrzucono polaczenie bezposrednie (7.	) */
            Logger::log("Received GG_EVENT_DCC7_REJECT\n");
            break;
        case GG_EVENT_DCC7_CONNECTED:	/**< Zestawiono polaczenie bezposrednie (7.	), nowy deskryptor */
            Logger::log("Received GG_EVENT_DCC7_CONNECTED\n");
            break;
        case GG_EVENT_DCC7_ERROR:		/**< Blad polaczenia bezposredniego (7.	) */
            Logger::log("Received GG_EVENT_DCC7_ERROR\n");
            break;
        case GG_EVENT_DCC7_DONE:		/**< Zakonczono polaczenie bezposrednie (7.	) */
            Logger::log("Received GG_EVENT_DCC7_DONE\n");
            break;
        case GG_EVENT_DCC7_PENDING:		/**< Trwa próba polaczenia bezposredniego (7.	), nowy deskryptor */
            Logger::log("Received GG_EVENT_DCC7_PENDING\n");
            break;

        case GG_EVENT_XML_EVENT:		/**< Otrzymano komunikat systemowy (7.7) */
            Logger::log("Received GG_EVENT_	ML_EVENT\n");
            break;
        case GG_EVENT_DISCONNECT_ACK:	/**< \brief Potwierdzenie zakonczenia sesji. Informuje o tym, ze zmiana stanu na niedostepny z opisem dotarla do serwera i mozna zakonczyc polaczenie TCP. */
            Logger::log("Received GG_EVENT_DISCONNECT_ACK\n");
            break;
        case GG_EVENT_TYPING_NOTIFICATION:	/**< Powiadomienie o pisaniu */
            onRecvTypingNotification(reinterpret_cast<gg_event_typing_notification&>(ev.event));
            Logger::log("Received GG_EVENT_TYPING_NOTIFICATION\n");

            break;
        case GG_EVENT_USER_DATA:		/**< Informacja o kontaktach */
            onRecvOwnInfo(reinterpret_cast<gg_event_user_data&>(ev.event));
            Logger::log("Received GG_EVENT_USER_DATA\n");
            break;
        case GG_EVENT_MULTILOGON_MSG:	/**< Wiadomosc wyslana z innej sesji multilogowania */
            Logger::log("Received GG_EVENT_MULTILOGON_MSG\n");
            break;
        case GG_EVENT_MULTILOGON_INFO:	/**< Informacja o innych sesjach multilogowania */
            Logger::log("Received GG_EVENT_MULTILOGON_INFO\n");
            break;

        case GG_EVENT_USERLIST100_VERSION:	/**< Otrzymano numer wersji listy kontaktów na serwerze (10.0) */
            Logger::log("Received GG_EVENT_USERLIST100_VERSION\n");
            break;
        case GG_EVENT_USERLIST100_REPLY:	/**< Wynik importu lub eksportu listy kontaktów (10.0) */
            onRecvContacts(reinterpret_cast<gg_event_userlist100_reply&>(ev.event));
            Logger::log("Received GG_EVENT_USERLIST100_REPLY");
            break;
    }
}

void GGWrapper::processEvent(ggEvent &event)
{
    switch(event.eventType)
    {
        case  ggEvent::LoginEvent:
        {
            ggLoginEvent &loginEvent = boost::any_cast<ggLoginEvent&>(event.content);
            processLoginEvent(loginEvent);
            break;
        }
        case ggEvent::MessageEvent:
        {
            ggMessageEvent &messageEvent = boost::any_cast<ggMessageEvent&>(event.content);
            processMessageEvent(messageEvent);
            break;
        }
        case ggEvent::TypingNotificationEvent:
        {
            ggTypingEvent &typingEvent = boost::any_cast<ggTypingEvent&>(event.content);
            processTypingEvent(typingEvent);
            break;
        }
    }

    //do switch case and process each item accordingly.
}
void GGWrapper::processLoginEvent(ggLoginEvent &event)
{
    struct gg_login_params p;
    memset(&p,0,sizeof(gg_login_params));
    p.uin = event.uin;
    p.password = const_cast<char *>(event.pass.c_str());
    p.async = 0;
    if((mpSession = gg_login(&p)) && gg_notify(mpSession,NULL,0) != -1)
    {
        eventSignal().emit(spEvent(new LoginResultEvent(true)));
        if ( -1 != gg_userlist100_request(mpSession,GG_USERLIST100_GET,0,GG_USERLIST100_FORMAT_TYPE_GG100,0))
        {
            Logger::log("Succesfully requested userlist..?");
        }

    }
    else
    {
        eventSignal().emit(spEvent(new LoginResultEvent(false)));
        gg_free_session(mpSession);
        mpSession = 0;
    }
}
void GGWrapper::processMessageEvent(ggMessageEvent &event)
{
    gg_send_message(mpSession,GG_CLASS_CHAT,event.uin,(unsigned char*)event.msg.c_str());
}

void GGWrapper::addToEventLoop(const ggEvent &event)
{
    mpQueue->push_back(event);
}
void GGWrapper::processTypingEvent(ggTypingEvent& event)
{
    gg_typing_notification(mpSession,event.recipientUin,event.length);
}

Wt::Signal<spEvent> &GGWrapper::eventSignal()
{
    return *mpEventSignal;
}
