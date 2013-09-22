#ifndef ASYNUITIMER_H
#define ASYNUITIMER_H
#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <string>
#include <Wt/WObject>
class AsyncUiTimer : public Wt::WObject
{
public:
    AsyncUiTimer(int msTimeout,const std::string &sessionId, boost::function<void ()> callBack,Wt::WObject *parent);
    ~AsyncUiTimer();
private:
    boost::thread mThread;
    int mMsTimeOut;
    boost::function<void ()> mCallback;
    std::string mSessionId;
    void enterLoop();
};

#endif // ASYNUITIMER_H
