#include "contactwindow.h"
#include "contactlist.h"
#include "ContactImportEvent.h"
#include "ContactGroup.h"
#include "contactinfo.h"
ContactWindow::ContactWindow(Wt::WContainerWidget *parent)
    : Wt::WContainerWidget(parent)
{
    setStyleClass("ContactWindow");
    resize("250px","500px");

    mpContactList = new ContactList(this);
}

void ContactWindow::contactsReceived(ContactImportEvent *ev)
{
    auto &groups = ev->contactGroups;
    for(auto it = groups.cbegin(); it != groups.cend(); ++it)
    {
        const ContactGroup &group = *it;
        for(auto contactIt = group.contacts.cbegin(); contactIt != group.contacts.cend(); ++contactIt)
        {
            const ContactInfo &info = *contactIt;
            mpContactList->addContact(info.uin,info.showName);
        }

    }
}
