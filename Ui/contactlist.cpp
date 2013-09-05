#include <Wt/WVBoxLayout>

#include "contactlist.h"
#include "contactentry.h"
ContactList::ContactList(Wt::WContainerWidget *parent)
    : Wt::WContainerWidget(parent)
{
    setStyleClass("ContactList");



    addWidget(new ContactEntry(2577961,"Olek"));
    addWidget(new ContactEntry(2577961,"Olek"));
    addWidget(new ContactEntry(2577961,"Olek"));
    addWidget(new ContactEntry(2577961,"Olek"));
    addWidget(new ContactEntry(2577961,"Olek"));
    addWidget(new ContactEntry(2577961,"Olek"));
    addWidget(new ContactEntry(2577961,"Olek"));
    addWidget(new ContactEntry(2577961,"Olek"));
    addWidget(new ContactEntry(2577961,"Olek"));
    addWidget(new ContactEntry(2577961,"Olek"));
    addWidget(new ContactEntry(2577961,"Olek"));
    addWidget(new ContactEntry(2577961,"Olek"));
    addWidget(new ContactEntry(2577961,"Olek"));
    addWidget(new ContactEntry(2577961,"Olek"));
    addWidget(new ContactEntry(2577961,"Olek"));


}

void ContactList::addContact(unsigned int uin, std::string showName)
{
    addWidget(new ContactEntry(uin,showName));
}
