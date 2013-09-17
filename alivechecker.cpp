
#include <Wt/WContainerWidget>
#include <sstream>

#include "alivechecker.h"
#include "logger.h"

#include <boost/asio.hpp>
AliveChecker::AliveChecker(Wt::WContainerWidget *parent)

    :
      Wt::WObject(parent),
      mTimeOut(15),
      mpActiveTimer(new Wt::JSignal<void>(this,"IsWindowActive")),
      mpDiedSignal(new Wt::Signal<void>(this)),
      mLastJSUpdate(boost::posix_time::second_clock::local_time())

{



    initJSScripts();
    initConnections();
    mWorkerThread = boost::thread(boost::bind(&AliveChecker::workerThreadLoop,this));

}
AliveChecker::~AliveChecker()
{
    mWorkerThread.interrupt();
    mWorkerThread.join();
}

void AliveChecker::initJSScripts()
{
   std::stringstream ss;
   ss << "setInterval(function(){ " << mpActiveTimer->createCall() << "},4000)";

   dynamic_cast<Wt::WContainerWidget*>(parent())->doJavaScript(ss.str());

}
void AliveChecker::initConnections()
{

    mpActiveTimer->connect(this,&AliveChecker::onJSTimeout);
}

void AliveChecker::onJSTimeout()
{
    boost::lock_guard<boost::mutex> lock(mMutex);
    mLastJSUpdate = boost::posix_time::second_clock::local_time();
}
void AliveChecker::onTimeout()
{
    boost::posix_time::ptime timeNow = boost::posix_time::second_clock::local_time();
    mMutex.lock();
    boost::posix_time::time_duration timeSpan(timeNow - mLastJSUpdate);
    mMutex.unlock();
    if (timeSpan.seconds() > mTimeOut)
    {
        Logger::log("Emiting We Are Dead SIGNAL!");
        mpDiedSignal->emit();
    }
}
void AliveChecker::workerThreadLoop()
{
    while(!mWorkerThread.interruption_requested())
    {
        onTimeout();

        boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
    }
}

Wt::Signal<void> &AliveChecker::died()
{
    return *mpDiedSignal;
}
