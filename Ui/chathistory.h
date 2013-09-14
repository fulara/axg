#ifndef CHATHISTORY_H
#define CHATHISTORY_H

#include "WtForwards.h"

#include <string>
#include <Wt/WJavaScript>
#include <boost/date_time/posix_time/posix_time.hpp>
class ChatHistory
{
public:
    ChatHistory(Wt::WContainerWidget *parent);

    void addSentMessage(std::string&);
    void addRecvMessage(unsigned int fromUin,std::string&);
    void updateScrollbar();
    void AddNewRecvHistoryEntry(std::string &content);
    void addNewSentHistoryEntry(std::string &message);
    void updateLastTimeStamp();
    std::string dateToText(boost::posix_time::ptime timeNow);
    bool shouldEntryHaveTimeStamp(boost::posix_time::ptime timeNow);
private:
    bool mIsFirstEntryEver;
    bool mWasLastMessageMine;
    boost::posix_time::ptime mLastHistoryEntryTimeStamp;
    Wt::WTable *mpTable;
    Wt::WContainerWidget *mpParent;
    Wt::WScrollArea *mpScrollArea;

};

#endif // CHATHISTORY_H
