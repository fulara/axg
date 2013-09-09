#ifndef CONTACTENTRY_H
#define CONTACTENTRY_H

#include "WtForwards.h"

#include <Wt/WContainerWidget>

class ContactEntry : public Wt::WContainerWidget
{
public:
    ContactEntry(unsigned int uin, std::string name);

    unsigned int getUin() const
    {
        return mUin;
    }

private:
    bool mIsClicked;
    unsigned int mUin;
    Wt::WText *mpStatus;
    Wt::WText *mpTargetUin;
};

#endif // CONTACTENTRY_H
