#include <Wt/WTable>
#include <Wt/WLabel>
#include <Wt/WScrollArea>
#include <Wt/WBoxLayout>
#include <sstream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/local_time/local_time.hpp>
#include <boost/chrono.hpp>
#include <boost/algorithm/string.hpp>
#include "chathistory.h"
#include "historyentry.h"
#include "logger.h"


std::string dateToStr(const boost::posix_time::ptime &time)
{
    boost::posix_time::time_facet* facet = new boost::posix_time::time_facet("%Y-%m-%d %H:%M:%S");
    std::stringstream ss;
    ss.imbue(std::locale(ss.getloc(), facet));
    ss << time;
    return ss.str();
}
std::string dateToTimeStr(const boost::posix_time::ptime &time)
{
    boost::posix_time::time_facet* facet = new boost::posix_time::time_facet("%H:%M:%S");
    std::stringstream ss;
    ss.imbue(std::locale(ss.getloc(), facet));
    ss << time;
    return ss.str();
}


ChatHistory::ChatHistory(Wt::WContainerWidget *parent)
{
    mpParent = parent;

    mpScrollArea = new Wt::WScrollArea();
    parent->addWidget(mpScrollArea);
    mpScrollArea->setWidget(mpTable = new Wt::WTable());
    mpScrollArea->setStyleClass("DialogWindowChatHistoryScroll");
    mpTable->setStyleClass("DialogWindowChatHistory");

    mLastHistoryEntryTimeStamp = boost::posix_time::second_clock::local_time();
    mLastHistoryEntryTimeStamp -=  boost::posix_time::hours(1);
    mIsFirstEntryEver = true;


}


void ChatHistory::updateScrollbar()
{
    std:: stringstream ss;
    ss << "setTimeout(function() {";
    ss << "var divx = document.getElementById(\"" << mpScrollArea->id() << "\");";
    ss << "divx.scrollTop = divx.scrollHeight;";
    ss << "},100);";
    mpParent->doJavaScript(ss.str());
}

void ChatHistory::updateLastTimeStamp()
{
    mLastHistoryEntryTimeStamp = boost::posix_time::second_clock::local_time();
}

bool ChatHistory::shouldEntryHaveTimeStamp(boost::posix_time::ptime timeNow)
{
    boost::posix_time::time_duration timeSinceLastMessage = timeNow - mLastHistoryEntryTimeStamp;
    if(timeSinceLastMessage.seconds() > 0)
    {
        updateLastTimeStamp();
        return true;
    }
    return false;
}
std::string ChatHistory::dateToText(boost::posix_time::ptime timeNow)
{
    if(timeNow.date().day() != mLastHistoryEntryTimeStamp.date().day())
    {
        return dateToStr(timeNow);
    }
    else
    {
        return dateToTimeStr(timeNow);
    }
}

void ChatHistory::addNewSentHistoryEntry(std::string &message)
{
    boost::posix_time::ptime timeNow = boost::posix_time::second_clock::local_time();
    std::string timeAsText = dateToText(timeNow);
    bool includeTimeStamp = shouldEntryHaveTimeStamp(timeNow);

    auto cell = mpTable->elementAt(mpTable->rowCount(),0);
    bool includeNick = (mIsFirstEntryEver || !mWasLastMessageMine);

    if(includeNick)
        cell->addWidget(HistoryEntry::createMyEntryDateNickname(timeAsText,"Ja",message,cell));
    else if(includeTimeStamp)
        cell->addWidget(HistoryEntry::createMyEntryDate(timeAsText,message,cell));
    else
        cell->addWidget(HistoryEntry::createMyEntry(message,cell));

    mWasLastMessageMine = true;
    mIsFirstEntryEver = false;
}

void ChatHistory::addSentMessage(std::string &message)
{
    addNewSentHistoryEntry(message);

    updateScrollbar();


}

void ChatHistory::AddNewRecvHistoryEntry(std::string &message)
{
    boost::posix_time::ptime timeNow = boost::posix_time::second_clock::local_time();
    std::string timeAsText = dateToText(timeNow);
    bool includeTimeStamp = shouldEntryHaveTimeStamp(timeNow);
    auto cell = mpTable->elementAt(mpTable->rowCount(),0);
    bool includeNick = (mIsFirstEntryEver || mWasLastMessageMine);

    if(includeNick)
        cell->addWidget(HistoryEntry::createTargetEntryDateNickname(timeAsText,"Nie-Ja",message,cell));
    else if(includeTimeStamp)
        cell->addWidget(HistoryEntry::createTargetEntryDate(timeAsText,message,cell));
    else
        cell->addWidget(HistoryEntry::createTargetEntry(message,cell));

    mWasLastMessageMine = false;
    mIsFirstEntryEver = false;
}

void ChatHistory::addRecvMessage(unsigned int fromUin, std::string &content)
{
    AddNewRecvHistoryEntry(content);

    updateScrollbar();
}
