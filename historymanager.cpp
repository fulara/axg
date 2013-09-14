#include "historymanager.h"

#include <boost/filesystem.hpp>
HistoryManager::HistoryManager()
{
    boost::filesystem::create_directory("history");
}


HistoryManager& HistoryManager::getInstance()
{
    static HistoryManager instance;
    return instance;
}


