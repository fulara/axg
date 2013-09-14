#include <Wt/WLineEdit>
#include <Wt/WPushButton>
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
    mpContactList = new ContactList(this);
    Wt::WContainerWidget *spaceFiller = new Wt::WContainerWidget(this);
    spaceFiller->setHeight(50);
    createContactFillEdit();

}
void ContactWindow::onContactFilterChanged(Wt::WKeyEvent keyEvent)
{
    if(keyEvent.key() == Wt::Key_Escape)
    {
        cancelFiltering();
        return;
    }
    auto text = mpContactFilterEdit->text();
    if(text.empty())
    {
        mpContactList->showFullList();
    }
    else
    {
        mpContactList->filterList(text.toUTF8());
    }
}

void ContactWindow::createContactFillEdit()
{
    mpContactFilterEdit = new Wt::WLineEdit(this);
    mpContactFilterEdit->setWidth(155);
    mpContactFilterEdit->setMargin("5",Wt::Left);
    mpContactFilterEdit->keyWentUp().connect(this,&ContactWindow::onContactFilterChanged);

    Wt::WPushButton *cancelSearchButton = new Wt::WPushButton("X",this);
    cancelSearchButton->clicked().connect(this,&ContactWindow::cancelFiltering);
}
void ContactWindow::cancelFiltering()
{
    mpContactFilterEdit->setText("");
    mpContactList->showFullList();
}
void ContactWindow::contactsReceived(ContactImportEvent *ev)
{
    mpContactList->initContacts(ev->contactGroups);

}

Wt::Signal<ContactInfo> &ContactWindow::windowOpenRequest()
{
    return mpContactList->openContactRequest();
}
Wt::Signal<ContactInfo> &ContactWindow::windowOpenRequestForceActivate()
{
    return mpContactList->openContactForceOpenRequest();
}
void ContactWindow::onNewContactInfoRequest(const unsigned int uin)
{
    mpContactList->handleNewContactInfoRequest(uin);
}
