#include <Wt/WTable>
#include <Wt/WLabel>
#include <Wt/WScrollArea>
#include <Wt/WBoxLayout>
#include <Wt/WApplication>
#include <sstream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/local_time/local_time.hpp>
#include <boost/chrono.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include "chathistory.h"
#include "historyentry.h"
#include "logger.h"
#include "Utils/formattingutils.h"
#include "firsthistoryentry.h"
#include "History/historymanager.h"
#include "History/parsedhistoryentry.h"
using namespace FormattingUtils;

ChatHistory::ChatHistory(const unsigned int ownerUin, const unsigned int talkingWith, std::string targetName, Wt::WContainerWidget *parent)
    :
      mOwnerUin(ownerUin),
      mTalkingWith(talkingWith)
{

    mpParent = parent;
    mTargetName = targetName;
    mpScrollArea = new Wt::WScrollArea();
    parent->addWidget(mpScrollArea);
    mpScrollArea->setWidget(mpTable = new Wt::WTable());
    mpScrollArea->setStyleClass("DialogWindowChatHistoryScroll");
    mpTable->setStyleClass("DialogWindowChatHistory");

    mLastHistoryEntryTimeStamp = boost::posix_time::second_clock::local_time();
    mLastHistoryEntryTimeStamp -=  boost::posix_time::hours(1);
    mIsFirstEntryEver = true;

    auto cell = mpTable->elementAt(0,0);
    auto fhe = new FirstHistoryEntry(cell);
    fhe->clicked().connect(boost::bind(&ChatHistory::onHistoryRequest,this));
    cell->addWidget(fhe);

}
void ChatHistory::handleParsedHistoryEntries(const std::list<ParsedHistoryEntry> &entries)
{
    mpTable->deleteRow(0);
    BOOST_REVERSE_FOREACH(const ParsedHistoryEntry& entry,entries)
    {
        auto cell = mpTable->insertRow(0)->elementAt(0);
        if(entry.isReceived)
        {
            cell->addWidget(HistoryEntry::createTargetEntryDateNickname(FormattingUtils::dateToStr(entry.time),mTargetName,entry.content,cell));
        }
        else
        {
            cell->addWidget(HistoryEntry::createMyEntryDateNickname(FormattingUtils::dateToStr(entry.time),"Ja",entry.content,cell));
        }

    }

}

void ChatHistory::onHistoryRequest()
{
    HistoryManager::requestHistory(this,mOwnerUin,mTalkingWith, Wt::WApplication::instance()->sessionId());
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
    if(timeSinceLastMessage.seconds() != 0)
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

void ChatHistory::AddNewRecvHistoryEntry(std::string &message, const boost::posix_time::ptime &time)
{
    std::string timeAsText = dateToText(time);
    bool includeTimeStamp = shouldEntryHaveTimeStamp(time);
    auto cell = mpTable->elementAt(mpTable->rowCount(),0);
    bool includeNick = (mIsFirstEntryEver || mWasLastMessageMine);

    if(includeNick)
        cell->addWidget(HistoryEntry::createTargetEntryDateNickname(timeAsText,mTargetName,message,cell));
    else if(includeTimeStamp)
        cell->addWidget(HistoryEntry::createTargetEntryDate(timeAsText,message,cell));
    else
        cell->addWidget(HistoryEntry::createTargetEntry(message,cell));

    mWasLastMessageMine = false;
    mIsFirstEntryEver = false;
}

void ChatHistory::addRecvMessage(unsigned int fromUin, std::string &content, const boost::posix_time::ptime &time)
{
    AddNewRecvHistoryEntry(content,time);

    updateScrollbar();
}
