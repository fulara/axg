#ifndef SYNCHRONIZEDQUEUE_H
#define SYNCHRONIZEDQUEUE_H

#include <list>
#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_guard.hpp>
#include <boost/thread.hpp>
#include <boost/any.hpp>
#include <boost/chrono.hpp>

using boost::lock_guard;
template <class T>
class SynchronizedQueue
{
public:
    SynchronizedQueue()
    {

    }

    void push_back(const T& item)
    {
        lock_guard<boost::mutex> lock(mMutex);
        mList.push_back(item);
    }
    void push_front(const T& item)
    {
        lock_guard<boost::mutex> lock(mMutex);
        mList.push_front(item);
    }

    //time out in ms
    boost::any poll(int timeOut = 40)
    {
        const int sleepPeriod = 10;
        while(timeOut)
        {
            mMutex.lock();
            if(mList.size())
            {
                boost::any item(mList.front());
                mList.pop_front();;
                mMutex.unlock();
                return item;
            }
            mMutex.unlock();
            boost::this_thread::sleep_for(boost::chrono::milliseconds(sleepPeriod));
            timeOut -= sleepPeriod;
        }

        return boost::any();
    }

private:
    boost::mutex mMutex;
    std::list<T> mList;
};

#endif // SYNCHRONIZEDQUEUE_H
