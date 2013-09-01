#ifndef GGWRAPPER_H
#define GGWRAPPER_H

template <class T>
class SynchronizedQueue;
struct ggEvent;
struct ggLoginEvent;
struct gg_session;

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>
#include <Wt/WSignal>
#include <Wt/WJavaScript>
#include "WtForwards.h"

class GGWrapper
{
public:
    GGWrapper();
    ~GGWrapper();


    void connect(unsigned int uin,const std::string &pass);
    void sendMessage(unsigned int targetUin,const std::string &content);

    Wt::Signal<bool> &loginResultSignal();
private:
    volatile bool mIsCancelled;
    volatile bool mIsInsideProcessingLoop;
    gg_session *mpSession;
    SynchronizedQueue<ggEvent> *mpQueue;
    Wt::Signal<bool> *mpLoginResultSignal;

    void enterLoop();
    void processEvent(ggEvent& event);
    void processLoginEvent(ggLoginEvent& event);
    void addToEventLoop(const ggEvent&);

};

#endif // GGWRAPPER_H
