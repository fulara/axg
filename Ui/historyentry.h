#ifndef HISTORYENTRY_H
#define HISTORYENTRY_H

#include "WtForwards.h"

#include <Wt/WContainerWidget>

class HistoryEntry : public Wt::WContainerWidget
{
public:
    static HistoryEntry *createMyEntryDateNickname(const std::string date, const std::string nickname, const std::string &msg, Wt::WContainerWidget *parent);
    static HistoryEntry *createMyEntryDate(const std::string date, const std::string &msg, Wt::WContainerWidget *parent);
    static HistoryEntry *createMyEntry(const std::string &msg,Wt::WContainerWidget *parent);

    static HistoryEntry *createTargetEntryDateNickname(const std::string date, const std::string nickname, const std::string &msg, Wt::WContainerWidget *parent);
    static HistoryEntry *createTargetEntryDate(const std::string date, const std::string &msg, Wt::WContainerWidget *parent);
    static HistoryEntry *createTargetEntry(const std::string &msg,Wt::WContainerWidget *parent);
private:
    HistoryEntry(Wt::WContainerWidget *parent);



};

#endif // HISTORYENTRY_H
