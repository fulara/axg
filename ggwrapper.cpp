#include <libgadu.h>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <Wt/WSignal>
#include "ggwrapper.h"
#include "synchronizedqueue.h"
#include "ggevent.h"
#include "ggloginevent.h"

GGWrapper::GGWrapper()
    : mpSession(NULL),
      mpLoginResultSignal(new Wt::Signal<bool>()),
      mIsCancelled(false),
      mIsInsideProcessingLoop(false)
{
    gg_debug_level = 255;
}

GGWrapper::~GGWrapper()
{
    mIsCancelled = true;
    while(mIsInsideProcessingLoop) boost::this_thread::sleep_for(boost::chrono::milliseconds(10));

    if(mpSession)
    {
        gg_free_session(mpSession);
    }

    delete mpQueue;
}

void GGWrapper::connect(unsigned int uin, const std::string &pass)
{
    addToEventLoop(ggEvent(ggEvent::LoginEvent,ggLoginEvent(uin,pass)));
}
void GGWrapper::enterLoop()
{
    mIsInsideProcessingLoop = true;
    while(!mIsCancelled)
    {
        boost::any item = mpQueue->poll();
        if(item.empty())
            continue;
        processEvent(boost::any_cast<ggEvent&>(item));
    }

    mIsInsideProcessingLoop = false;
    //probably here some cleaning or just do everything in destructor;
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
    }

    //do switch case and process each item accordingly.
}
void GGWrapper::processLoginEvent(ggLoginEvent &event)
{
    struct gg_login_params p;
    p.uin = event.uin;
    p.password = const_cast<char *>(event.pass.c_str());
    if(mpSession = gg_login(&p))
    {

    }
}

void GGWrapper::addToEventLoop(const ggEvent &event)
{
    mpQueue->push_back(event);
}


Wt::Signal<bool> &GGWrapper::loginResultSignal()
{
    return *mpLoginResultSignal;
}
