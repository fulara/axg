#ifndef CONTACTENTRY_H
#define CONTACTENTRY_H

#include "WtForwards.h"

#include <Wt/WContainerWidget>

class ContactEntry : public Wt::WContainerWidget
{
public:
    ContactEntry(unsigned int uin, std::string name);

    bool matchesFilter(std::string filter) const;
    unsigned int getUin() const
    {
        return mUin;
    }
private:
    bool mIsClicked;
    unsigned int mUin;
    Wt::WText *mpShowText;

    void ensureProperTextLength(std::string &str);
};

#endif // CONTACTENTRY_H
