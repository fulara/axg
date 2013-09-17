#ifndef ALIVECHECKER_H
#define ALIVECHECKER_H



#include "WtForwards.h"
#include <Wt/WObject>
#include <Wt/WJavaScript>
#include <Wt/WSignal>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>

class AliveChecker : public Wt::WObject
{
public:
    //use container just to get access to doJavaScript.
    AliveChecker(Wt::WContainerWidget *parent);
    ~AliveChecker();
    Wt::Signal<void> &died();
private:
    const int mTimeOut;
    Wt::JSignal<void> *mpActiveTimer;
    Wt::Signal<void> *mpDiedSignal;
    boost::posix_time::ptime mLastJSUpdate;
    boost::thread mWorkerThread;
    boost::mutex mMutex;

    void workerThreadLoop();
    void initJSScripts();
    void initConnections();
    void onTimeout();
    void onJSTimeout();

};

#endif // ALIVECHECKER_H
