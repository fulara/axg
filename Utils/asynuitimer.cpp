#include <Wt/WServer>
#include "asynuitimer.h"

AsyncUiTimer::AsyncUiTimer(int msTimeout, const std::string &sessionId, boost::function<void ()> callBack, Wt::WObject *parent)
    : Wt::WObject(parent),
      mMsTimeOut(msTimeout),
      mCallback(callBack),
      mSessionId(sessionId)
{
    mThread = boost::thread(boost::bind(&AsyncUiTimer::enterLoop,this));
}
AsyncUiTimer::~AsyncUiTimer()
{
    mThread.interrupt();
    mThread.join();
}


void  AsyncUiTimer::enterLoop()
{
    while(!mThread.interruption_requested())
    {
        boost::this_thread::sleep_for(boost::chrono::milliseconds(mMsTimeOut));
        Wt::WServer::instance()->post(mSessionId,mCallback);
    }
}
