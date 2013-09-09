#include <Wt/WWidget>
#include <Wt/WText>
#include <Wt/WSignal>
#include "contactentry.h"

ContactEntry::ContactEntry(unsigned int uin, std::string name)
    : mUin(uin)
{
    setStyleClass("ContactEntry");

    mpStatus = new Wt::WText(name,this);
    mpTargetUin = new Wt::WText(boost::lexical_cast<std::string>(uin),this);
}
