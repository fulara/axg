#ifndef HISTORYMANAGER_H
#define HISTORYMANAGER_H

class ChatHistory;
class SynchronizedQueue;
struct HistoryPack;
struct HistoryRequest;
struct ParsedHistoryEntry;
struct LoginPackage;
#include <string>
#include <boost/thread/mutex.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/interprocess/smart_ptr/unique_ptr.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/regex.hpp>
#include <fstream>
#include <map>
#include <list>
typedef boost::shared_ptr<HistoryPack> spHistoryPack;
typedef boost::shared_ptr<HistoryRequest> spHistoryRequest;
typedef boost::shared_ptr<std::ofstream> spOfstream;
typedef boost::shared_ptr<std::ifstream> spIfstream;
class HistoryManager
{
public:

    static HistoryManager& getInstance();
    HistoryManager(const HistoryManager&) = delete;
    HistoryManager(const HistoryManager&&) = delete;
    HistoryManager& operator=(const HistoryManager&) = delete;
    static void saveSendEntry(const std::string& msg,const unsigned int userUin,const unsigned int talkingWith);
    static void saveRcvEntry(const std::string& msg, const boost::posix_time::ptime &timestamp,const unsigned int userUin,const unsigned int talkingWith);
    static void requestHistory(ChatHistory* caller, const unsigned int ownerUin, const unsigned int talkingWith, const std::string& owningSessionId);
    static void informAboutLogin(unsigned int uin);

private:
    boost::mutex mSessionCountMutex;
    boost::mutex mGlobalFileMutex;
    SynchronizedQueue *mpQueue;
    std::multimap<unsigned int,unsigned int> mAlreadyClearedDirs;
    HistoryManager();

    void createNeededDirectories(const unsigned int ownerUin, const unsigned int talkingWith);
    void loop();
    void clearLastChat(unsigned int ownerUin, unsigned int talkingWith);
    void processPack(spHistoryPack pack);
    void processRequest(spHistoryRequest requestHistory);
    void processLogin(LoginPackage loginPackage);
    void appendEntryToFile(std::ofstream& os, bool isSent, const boost::posix_time::ptime &saveTime, const std::string &content );
    std::list<ParsedHistoryEntry> parseHistoryEntries(const std::string& path);
    spOfstream getFileAppend(const std::string &path, const std::string& day);
    spIfstream getFileRead(const std::string &path, const std::string& day);
    std::string getPathForHistoryStorage(const unsigned int ownerUin);
    std::string getPathForHistoryStorage(const unsigned int ownerUin,const unsigned int targetUin);


    static boost::regex msMsgEntryWidget;

};

#endif // HISTORYMANAGER_H

