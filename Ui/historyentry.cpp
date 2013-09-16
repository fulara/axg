#include <Wt/WText>
#include <Wt/WBreak>
#include "historyentry.h"


HistoryEntry::HistoryEntry(Wt::WContainerWidget *parent)
    : Wt::WContainerWidget(parent)
{
}

HistoryEntry *HistoryEntry::createMyEntryDateNickname(const std::string date, const std::string nickname, const std::string &msg, Wt::WContainerWidget *parent)
{
    HistoryEntry *newEntry = new HistoryEntry(parent);
    Wt::WText* dateWidget = new Wt::WText(std::string("(") + date + ")",newEntry);
    Wt::WText* nicknameWidget = new Wt::WText(nickname,newEntry);
    nicknameWidget->setInline(false);
    Wt::WText* msgWidget = new Wt::WText(Wt::WString::fromUTF8(msg),newEntry);
    msgWidget->setInline(false);
    newEntry->addWidget(nicknameWidget);
    nicknameWidget->setStyleClass("ChatHistoryNick");
    newEntry->addWidget(dateWidget);
    dateWidget->setStyleClass("ChatHistoryTimeStamp");
    newEntry->addWidget(msgWidget);
    newEntry->setStyleClass("ChatHistoryEntryMine");
    return newEntry;
}

HistoryEntry *HistoryEntry::createMyEntryDate(const std::string date, const std::string &msg, Wt::WContainerWidget *parent)
{
    HistoryEntry *newEntry = new HistoryEntry(parent);
    Wt::WText* dateWidget = new Wt::WText(std::string("(") + date + ")",newEntry);
    Wt::WText* msgWidget = new Wt::WText(Wt::WString::fromUTF8(msg),newEntry);
    msgWidget->setInline(false);
    newEntry->addWidget(dateWidget);
    dateWidget->setStyleClass("ChatHistoryTimeStamp");
    newEntry->addWidget(msgWidget);
    newEntry->setStyleClass("ChatHistoryEntryMine");
    return newEntry;
}

HistoryEntry *HistoryEntry::createMyEntry(const std::string &msg, Wt::WContainerWidget *parent)
{
    HistoryEntry *newEntry = new HistoryEntry(parent);
    Wt::WText* msgWidget = new Wt::WText(Wt::WString::fromUTF8(msg),newEntry);
    msgWidget->setInline(false);
    newEntry->addWidget(msgWidget);
    newEntry->setStyleClass("ChatHistoryEntryMine");
    return newEntry;
}

HistoryEntry *HistoryEntry::createTargetEntryDateNickname(const std::string date, const std::string nickname, const std::string &msg, Wt::WContainerWidget *parent)
{
    HistoryEntry *newEntry = new HistoryEntry(parent);
    Wt::WText* dateWidget = new Wt::WText(std::string("(") + date + ")",newEntry);
    Wt::WText* nicknameWidget = new Wt::WText(nickname,newEntry);
    nicknameWidget->setInline(false);
    Wt::WText* msgWidget = new Wt::WText(Wt::WString::fromUTF8(msg),newEntry);
    msgWidget->setInline(false);
    newEntry->addWidget(nicknameWidget);
    nicknameWidget->setStyleClass("ChatHistoryNick");
    newEntry->addWidget(dateWidget);
    dateWidget->setStyleClass("ChatHistoryTimeStamp");
    newEntry->addWidget(msgWidget);
    newEntry->setStyleClass("ChatHistoryEntryTarget");
    return newEntry;
}


HistoryEntry *HistoryEntry::createTargetEntryDate(const std::string date, const std::string &msg, Wt::WContainerWidget *parent)
{
    HistoryEntry *newEntry = new HistoryEntry(parent);
    Wt::WText* dateWidget = new Wt::WText(std::string("(") + date + ")",newEntry);
    Wt::WText* msgWidget = new Wt::WText(Wt::WString::fromUTF8(msg),newEntry);
    msgWidget->setInline(false);
    newEntry->addWidget(dateWidget);
    dateWidget->setStyleClass("ChatHistoryTimeStamp");
    newEntry->addWidget(msgWidget);
    newEntry->setStyleClass("ChatHistoryEntryTarget");
    return newEntry;
}

HistoryEntry *HistoryEntry::createTargetEntry(const std::string &msg, Wt::WContainerWidget *parent)
{
    HistoryEntry *newEntry = new HistoryEntry(parent);
    Wt::WText* msgWidget = new Wt::WText(Wt::WString::fromUTF8(msg),newEntry);
    msgWidget->setInline(false);
    newEntry->addWidget(msgWidget);
    newEntry->setStyleClass("ChatHistoryEntryTarget");
    return newEntry;
}
