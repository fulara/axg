#include "contactwindow.h"
#include "contactlist.h"
#include "ContactImportEvent.h"
#include "ContactGroup.h"
#include "contactinfo.h"
#include "logger.h"
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
    mpContactList->initContacts(ev->contactGroups);

}

Wt::Signal<ContactInfo> &ContactWindow::windowOpenRequest()
{
    return mpContactList->openContactRequest();
}
void ContactWindow::onNewContactInfoRequest(const unsigned int uin)
{
    mpContactList->handleNewContactInfoRequest(uin);
}
