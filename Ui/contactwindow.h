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
private:
    ContactList *mpContactList;


};

#endif // CONTACTWINDOW_H
