#include <boost/filesystem.hpp>
#include <boost/thread/thread.hpp>
#include <boost/lexical_cast.hpp>
#include "historymanager.h"
#include "synchronizedqueue.h"
#include "historypack.h"
#include "Utils/formattingutils.h"
#include "logger.h"
HistoryManager::HistoryManager()
{
    boost::filesystem::create_directory("history");
    mpQueue = new SynchronizedQueue<spHistoryPack>();
    new boost::thread(boost::bind(&HistoryManager::loop,this));
}


HistoryManager& HistoryManager::getInstance()
{
    static HistoryManager instance;
    return instance;
}

void HistoryManager::loop()
{
    while(true)
    {
        boost::any polled = mpQueue->poll();
        if(!polled.empty())
        {
            spHistoryPack sphp = boost::any_cast<spHistoryPack>(polled);
            Logger::log("hai");
            processPack(sphp);
        }
    }
}

void HistoryManager::saveEntry(const std::string &msg, const unsigned int userUin, const unsigned int talkingWith, const bool isSent)
{
    HistoryManager &manager = getInstance();

    manager.mpQueue->push_back(spHistoryPack(new HistoryPack(msg,userUin,talkingWith,isSent)));
}

void HistoryManager::processPack(spHistoryPack pack)
{
    boost::posix_time::ptime saveTime = boost::posix_time::second_clock::local_time();
    createNeededDirectories(pack->ownerUin,pack->talkingWith);

    spOfstream spof(getFile(getPathForHistoryStorage(pack->ownerUin,pack->talkingWith),FormattingUtils::dateToDayStr(saveTime)));
    std::ofstream& of = *spof.get();
    of << "" << FormattingUtils::dateToStr(saveTime) << "";
    if(pack->isSent)
    {
        of << "-S-"  << boost::lexical_cast<std::string>(pack->ownerUin) << "-";
    }
    else
    {
        of << "-R-" << boost::lexical_cast<std::string>(pack->talkingWith) << "-";
    }
    of << pack->content << std::endl;

}
spOfstream HistoryManager::getFile(const std::string &path, const std::string &day)
{
    return spOfstream(new std::ofstream(path + "/" + day, std::ofstream::app));
}

void HistoryManager::createNeededDirectories(const unsigned int ownerUin, const unsigned int talkingWith)
{
    boost::filesystem::create_directory(getPathForHistoryStorage(ownerUin));
    boost::filesystem::create_directory(getPathForHistoryStorage(ownerUin,talkingWith));
}

std::string HistoryManager::getPathForHistoryStorage(const unsigned int ownerUin)
{
    return std::string("history/") + boost::lexical_cast<std::string>(ownerUin);
}

std::string HistoryManager::getPathForHistoryStorage(const unsigned int ownerUin,const unsigned int targetUin)
{
    return getPathForHistoryStorage(ownerUin) + "/" + boost::lexical_cast<std::string>(targetUin);
}
