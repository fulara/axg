#ifndef HISTORYENTRY_H
#define HISTORYENTRY_H

#include "WtForwards.h"

#include <Wt/WContainerWidget>

class HistoryEntry : public Wt::WContainerWidget
{
public:
    HistoryEntry(const std::string date,const std::string msg, bool myMessage,Wt::WContainerWidget *parent);

private:
    Wt::WText *mpDateText;
    Wt::WText *mpMessage;

};

#endif // HISTORYENTRY_H
