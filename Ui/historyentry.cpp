#include <Wt/WText>
#include <Wt/WBreak>
#include "historyentry.h"

HistoryEntry::HistoryEntry(const std::string date, const std::string msg,bool myMessage, Wt::WContainerWidget *parent)
    : Wt::WContainerWidget(parent)
{
    mpDateText = new Wt::WText(date,this);

    new Wt::WBreak(this);
    mpMessage = new Wt::WText(msg,this);

    if(myMessage)
        setStyleClass("ChatHistoryEntryMine");
    else
        setStyleClass("ChatHistoryEntryTarget");
}
