#include <Wt/WWidget>
#include <Wt/WText>
#include "contactentry.h"

ContactEntry::ContactEntry(unsigned int uin, std::string name)
{
    setStyleClass("ContactEntry");

    mpStatus = new Wt::WText(name,this);
    mpTargetUin = new Wt::WText(boost::lexical_cast<std::string>(uin),this);
}
