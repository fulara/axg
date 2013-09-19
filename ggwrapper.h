#ifndef GGWRAPPER_H
#define GGWRAPPER_H

class SynchronizedQueue;

struct ggEvent;
struct ggLoginEvent;
struct ggTypingEvent;
struct gg_session;
struct gg_event;
struct gg_event_msg;
struct gg_event_user_data;
struct gg_event_typing_notification;
struct ggMessageEvent;
struct gg_event_userlist100_reply;
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>
#include <Wt/WSignal>
#include <Wt/WJavaScript>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include "WtForwards.h"
#include "event.h"

class GGWrapper
{
public:
    GGWrapper();
    ~GGWrapper();


    void connect(unsigned int uin,const std::string &pass);
    void sendMessage(unsigned int targetUin,const std::string &content);
    void sendTypingNotification(unsigned int targetUin, int length);
    Wt::Signal<boost::shared_ptr<Event> > &eventSignal();
private:
    gg_session *mpSession;
    SynchronizedQueue *mpQueue;
    Wt::Signal<boost::shared_ptr<Event> > *mpEventSignal;
    boost::thread mWorkerThread;
    boost::posix_time::ptime mTimeSinceLastPing;

    void enterLoop();
    gg_event* pollGGEvent();
    void processEvent(ggEvent& event);
    void processGGEvent(gg_event &ev);

    void addToEventLoop(const ggEvent&);
    void sendPing();


    //process gg events methods.
    void onRecvMsg(gg_event_msg& msg);
    void onRecvOwnInfo(gg_event_user_data &data);
    void onRecvContacts(gg_event_userlist100_reply& data);
    void onRecvTypingNotification(gg_event_typing_notification&);
    //process internal events.
    void processLoginEvent(ggLoginEvent& event);
    void processMessageEvent(ggMessageEvent &event);
    void processTypingEvent(ggTypingEvent& event);
};

#endif // GGWRAPPER_H
