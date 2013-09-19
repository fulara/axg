#ifndef CHATHISTORY_H
#define CHATHISTORY_H

struct ParsedHistoryEntry;
#include "WtForwards.h"

#include <string>
#include <Wt/WJavaScript>
#include <boost/date_time/posix_time/posix_time.hpp>
class ChatHistory
{
public:
    ChatHistory(const unsigned int ownerUin, const unsigned int talkingWith,std::string targetName,Wt::WContainerWidget *parent);

    void addSentMessage(std::string&);
    void addRecvMessage(unsigned int fromUin,std::string& content, const boost::posix_time::ptime &time);
    void updateScrollbar();
    void AddNewRecvHistoryEntry(std::string &content, const boost::posix_time::ptime &time);
    void addNewSentHistoryEntry(std::string &message);
    void updateLastTimeStamp();
    std::string dateToText(boost::posix_time::ptime timeNow);
    bool shouldEntryHaveTimeStamp(boost::posix_time::ptime timeNow);

    void handleParsedHistoryEntries(const std::list<ParsedHistoryEntry> &entries);
private:
    bool mIsFirstEntryEver;
    bool mWasLastMessageMine;
    const unsigned int mOwnerUin;
    const unsigned int mTalkingWith;
    std::string mTargetName;
    boost::posix_time::ptime mLastHistoryEntryTimeStamp;
    Wt::WTable *mpTable;
    Wt::WContainerWidget *mpParent;
    Wt::WScrollArea *mpScrollArea;

    void onHistoryRequest();

};

#endif // CHATHISTORY_H
