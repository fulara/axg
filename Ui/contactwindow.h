#ifndef CONTACTWINDOW_H
#define CONTACTWINDOW_H

class ContactList;
class ContactImportEvent;
class ContactInfo;
#include "WtForwards.h"

#include <Wt/WContainerWidget>
class ContactWindow : public Wt::WContainerWidget
{
public:
    ContactWindow(Wt::WContainerWidget *parent);
    void contactsReceived(ContactImportEvent *ev);
    void onNewContactInfoRequest(const unsigned int uin);
    Wt::Signal<ContactInfo> &windowOpenRequest();
    Wt::Signal<ContactInfo> &windowOpenRequestForceActivate();
    void createContactFillEdit();

private:
    ContactList *mpContactList;
    Wt::WLineEdit *mpContactFilterEdit;

    void cancelFiltering();
    void onContactFilterChanged(Wt::WKeyEvent keyEvent);

};

#endif // CONTACTWINDOW_H
