#ifndef HISTORYMANAGER_H
#define HISTORYMANAGER_H

#include <boost/thread/mutex.hpp>
class HistoryManager
{
public:

    static HistoryManager& getInstance();

private:
    HistoryManager();


    boost::mutex mSessionCountMutex;
    boost::mutex mGlobalFileMutex;
};

#endif // HISTORYMANAGER_H
