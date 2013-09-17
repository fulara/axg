#ifndef HISTORYMANAGER_H
#define HISTORYMANAGER_H

template <class T>
class SynchronizedQueue;
struct HistoryPack;

#include <string>
#include <boost/thread/mutex.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/interprocess/smart_ptr/unique_ptr.hpp>
#include <fstream>
typedef boost::shared_ptr<HistoryPack> spHistoryPack;
typedef boost::shared_ptr<std::ofstream> spOfstream;
class HistoryManager
{
public:

    static HistoryManager& getInstance();
    HistoryManager(const HistoryManager&) = delete;
    HistoryManager(const HistoryManager&&) = delete;
    HistoryManager& operator=(const HistoryManager&) = delete;
    static void saveEntry(const std::string& msg,const unsigned int userUin,const unsigned int talkingWith, const bool isSent);

private:
    boost::mutex mSessionCountMutex;
    boost::mutex mGlobalFileMutex;
    SynchronizedQueue<spHistoryPack> *mpQueue;

    HistoryManager();

    void createNeededDirectories(const unsigned int ownerUin, const unsigned int talkingWith);
    void loop();
    void processPack(spHistoryPack pack);
    spOfstream getFile(const std::string &path, const std::string& day);
    std::string getPathForHistoryStorage(const unsigned int ownerUin);
    std::string getPathForHistoryStorage(const unsigned int ownerUin,const unsigned int targetUin);




};

#endif // HISTORYMANAGER_H
