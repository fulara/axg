#ifndef CONTACTLIST_H
#define CONTACTLIST_H

#include "WtForwards.h"

#include <Wt/WContainerWidget>
class ContactList : public Wt::WContainerWidget
{
public:
    ContactList(Wt::WContainerWidget *parent);
    void addContact(unsigned int uin, std::string showName);
};

#endif // CONTACTLIST_H
