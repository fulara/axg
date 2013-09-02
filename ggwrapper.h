#ifndef GGWRAPPER_H
#define GGWRAPPER_H

template <class T>
class SynchronizedQueue;

struct ggEvent;
struct ggLoginEvent;
struct gg_session;
struct gg_event;
struct gg_event_msg;
struct ggMessageEvent;
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

    Wt::Signal<boost::shared_ptr<Event> > &eventSignal();
private:
    gg_session *mpSession;
    SynchronizedQueue<ggEvent> *mpQueue;
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

    //process internal events.
    void processLoginEvent(ggLoginEvent& event);
    void processMessageEvent(ggMessageEvent &event);
};

#endif // GGWRAPPER_H
