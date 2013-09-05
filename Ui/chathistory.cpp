#include <Wt/WTable>
#include <Wt/WLabel>
#include <Wt/WScrollArea>
#include <Wt/WBoxLayout>
#include <sstream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/local_time/local_time.hpp>
#include <boost/algorithm/string.hpp>
#include "chathistory.h"
#include "historyentry.h"
#include "logger.h"


std::string formatTime(const boost::posix_time::ptime &time)
{
    boost::posix_time::time_facet* facet = new boost::posix_time::time_facet("%Y%m%d %H:%M:%S");
    std::stringstream ss;
    ss.imbue(std::locale(ss.getloc(), facet));
    ss << time;
    return ss.str();
}

ChatHistory::ChatHistory(Wt::WContainerWidget *parent,Wt::WBoxLayout *layout)
{
    mpParent = parent;

    mpScrollArea = new Wt::WScrollArea();
    layout->addWidget(mpScrollArea,1);
    mpScrollArea->setWidget(mpTable = new Wt::WTable());
    mpScrollArea->setStyleClass("DialogWindowChatHistoryScroll");
    mpTable->setStyleClass("DialogWindowChatHistory");




}


void ChatHistory::addSentMessage(std::string &message)
{
    std::string brStr("<br />");
    std::string nl("\n");
    boost::replace_all(message,nl,brStr);
    Logger::log(std::string("String with replaced..") + message);
    boost::posix_time::ptime timeNow = boost::posix_time::second_clock::local_time();
    auto cell = mpTable->elementAt(mpTable->rowCount(),0);
    cell->addWidget(new HistoryEntry(formatTime(timeNow),message,true,cell));

    std:: stringstream ss;
    ss << "setTimeout(function() {";
    ss << "var divx = document.getElementById(\"" << mpScrollArea->id() << "\");";
    ss << "divx.scrollTop = divx.scrollHeight;";
    ss << "},100);";
    mpParent->doJavaScript(ss.str());


}

void ChatHistory::addRecvMessage(unsigned int fromUin, std::string &content)
{
    std::string brStr("<br />");
    std::string nl("\n");
    boost::replace_all(content,nl,brStr);
    boost::posix_time::ptime timeNow = boost::posix_time::second_clock::local_time();
    auto cell = mpTable->elementAt(mpTable->rowCount(),0);
    cell->addWidget(new HistoryEntry(formatTime(timeNow),content,false,cell));

    std:: stringstream ss;
    ss << "setTimeout(function() {";
    ss << "var divx = document.getElementById(\"" << mpScrollArea->id() << "\");";
    ss << "divx.scrollTop = divx.scrollHeight;";
    ss << "},100);";
    mpParent->doJavaScript(ss.str());
}
