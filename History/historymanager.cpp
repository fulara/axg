#include <boost/filesystem.hpp>
#include <boost/thread/thread.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <Wt/WServer>
#include "historymanager.h"
#include "synchronizedqueue.h"
#include "LoginPackage.h"
#include "historypack.h"
#include "HistoryRequest.h"
#include "Utils/formattingutils.h"
#include "parsedhistoryentry.h"
#include "logger.h"
#include "Ui/chathistory.h"

boost::regex HistoryManager::msMsgEntryWidget("^\\[(\\S+ \\S+)\\]-(R|S)-(.*)$");
HistoryManager::HistoryManager()
{
    boost::filesystem::create_directory("history");
    mpQueue = new SynchronizedQueue();
    new boost::thread(boost::bind(&HistoryManager::loop,this));
}


HistoryManager& HistoryManager::getInstance()
{
    static HistoryManager instance;
    return instance;
}

void HistoryManager::loop()
{
    while(true)
    {
        boost::any polled = mpQueue->poll();

        if(!polled.empty())
        {
            if(polled.type() == typeid(spHistoryPack))
            {
                spHistoryPack sphp = boost::any_cast<spHistoryPack>(polled);
                processPack(sphp);
            }
            else if(polled.type() == typeid(spHistoryRequest))
            {
                spHistoryRequest request = boost::any_cast<spHistoryRequest>(polled);
                processRequest(request);
            }
            else if(polled.type() == typeid(LoginPackage))
            {
                LoginPackage loginPackage = boost::any_cast<LoginPackage>(polled);
                processLogin(loginPackage);
            }
            else
            {
            }

        }
    }
}
void HistoryManager::informAboutLogin(unsigned int uin)
{
    HistoryManager &manager = getInstance();
    manager.mpQueue->push_back(LoginPackage(uin));
}

void HistoryManager::requestHistory(ChatHistory *caller, const unsigned int ownerUin, const unsigned int talkingWith, const std::string& owningSessionId)
{
    HistoryManager &manager = getInstance();

    manager.mpQueue->push_back(spHistoryRequest(new HistoryRequest(caller,ownerUin,talkingWith,owningSessionId)));
}

void HistoryManager::saveRcvEntry(const std::string &msg, const unsigned int userUin, const unsigned int talkingWith)
{
    HistoryManager &manager = getInstance();

    manager.mpQueue->push_back(spHistoryPack(new HistoryPack(msg,userUin,talkingWith,false)));
}
void HistoryManager::saveSendEntry(const std::string &msg, const unsigned int userUin, const unsigned int talkingWith)
{
    HistoryManager &manager = getInstance();

    manager.mpQueue->push_back(spHistoryPack(new HistoryPack(msg,userUin,talkingWith,true)));
}
void HistoryManager::clearLastChat(unsigned int ownerUin, unsigned int talkingWith)
{
    auto itPair = mAlreadyClearedDirs.equal_range(ownerUin);
    bool found = false;
    for(auto it = itPair.first; it != itPair.second; ++it)
    {
        if(it->second == talkingWith)
            found = true;
    }
    if (found)
        return;
    std::string storagePath = getPathForHistoryStorage(ownerUin,talkingWith);
    std::string path = storagePath + "/CurrentConversation";
    if(!boost::filesystem::exists(path))
        return;

    std::list<ParsedHistoryEntry> entries(parseHistoryEntries(path));
    boost::ptr_map<std::string, std::ofstream> fileHandles;
    BOOST_FOREACH(ParsedHistoryEntry &entry,entries)
    {
        if(entry.time.is_not_a_date_time())
            continue;
        std::string fileName = storagePath + "/" + FormattingUtils::dateToDayStr(entry.time);
        auto it = fileHandles.find(fileName);

        if(it == fileHandles.end())
        {
            fileHandles.insert(fileName, new std::ofstream(fileName,std::ofstream::app));
        }
        std::ofstream &os = fileHandles[fileName];
        appendEntryToFile(os,!entry.isReceived,entry.time,entry.content);
    }
    boost::filesystem::remove(path);
    mAlreadyClearedDirs.insert(std::make_pair(ownerUin,talkingWith));
}
void HistoryManager::processLogin(LoginPackage loginPackage)
{
    mAlreadyClearedDirs.erase(loginPackage.loggedUin);
}

void HistoryManager::processPack(spHistoryPack pack)
{
    clearLastChat(pack->ownerUin,pack->talkingWith);
    boost::posix_time::ptime saveTime = boost::posix_time::second_clock::local_time();
    createNeededDirectories(pack->ownerUin,pack->talkingWith);

    spOfstream spof(getFileAppend(getPathForHistoryStorage(pack->ownerUin,pack->talkingWith),FormattingUtils::dateToDayStr(saveTime)));
    std::ofstream& of = *spof.get();
    appendEntryToFile(of,pack->isSent,saveTime,pack->content);


}
void HistoryManager::appendEntryToFile(std::ofstream &os, bool isSent, const boost::posix_time::ptime &saveTime, const std::string &content)
{
    if(saveTime.is_not_a_date_time())
        return;
    if(os.tellp() != 0)
        os << std::endl;
    os << "[" << FormattingUtils::dateToStr(saveTime) << "]";
    if(isSent)
    {
        os << "-S-";
    }
    else
    {
        os << "-R-";
    }
    os << content;
}

std::list<ParsedHistoryEntry> HistoryManager::parseHistoryEntries(const std::string &path)
{
    std::ifstream is(path);
    const int buffSize = 2048;
    char buff[buffSize];
    std::string line;
    std::string msgContent;
    boost::posix_time::ptime time;
    bool isRcv = false;
    std::list<ParsedHistoryEntry> entries;
    while(!is.eof())
    {
        boost::match_results< std::string::const_iterator > matches;
        is.getline(buff,buffSize);
        line = buff;
        if(boost::regex_match(line,matches,msMsgEntryWidget))
        {
            if(msgContent.size())
            {
                entries.push_back(ParsedHistoryEntry(isRcv,time,msgContent));
            }
            const std::string timeAsStr = matches[1];
            time =FormattingUtils::parseTime(timeAsStr);
            const std::string rOrS = matches[2];
            isRcv = rOrS == "R";
            msgContent = matches[3];

        }
        else
        {
            msgContent.push_back('\n');
            msgContent += line;
        }

    }
    if(msgContent.size())
    {
        entries.push_back(ParsedHistoryEntry(isRcv,time,msgContent));
    }

    return std::move(entries);
}

void HistoryManager::processRequest(spHistoryRequest requestHistory)
{
    clearLastChat(requestHistory->ownerUin,requestHistory->talkingWith);
    std::list<ParsedHistoryEntry> entries(parseHistoryEntries(getPathForHistoryStorage(requestHistory->ownerUin,requestHistory->talkingWith) + "/" + FormattingUtils::dateToDayStr(boost::posix_time::second_clock::local_time())));

    ChatHistory * const requester = requestHistory->requester;
    Wt::WServer::instance()->post(requestHistory->owningSessionId,std::bind(
                          [entries,requester]()
    {
        requester->handleParsedHistoryEntries(entries);
    }
                          ));
}
spIfstream HistoryManager::getFileRead(const std::string &path, const std::string &day)
{
    return spIfstream(new std::ifstream(path + "/" + day));
}
spOfstream HistoryManager::getFileAppend(const std::string &path, const std::string &day)
{
    return spOfstream(new std::ofstream(path + "/CurrentConversation", std::ofstream::app));
}

void HistoryManager::createNeededDirectories(const unsigned int ownerUin, const unsigned int talkingWith)
{
    boost::filesystem::create_directory(getPathForHistoryStorage(ownerUin));
    boost::filesystem::create_directory(getPathForHistoryStorage(ownerUin,talkingWith));
}

std::string HistoryManager::getPathForHistoryStorage(const unsigned int ownerUin)
{
    return std::string("history/") + boost::lexical_cast<std::string>(ownerUin);
}

std::string HistoryManager::getPathForHistoryStorage(const unsigned int ownerUin,const unsigned int targetUin)
{
    return getPathForHistoryStorage(ownerUin) + "/" + boost::lexical_cast<std::string>(targetUin);
}
