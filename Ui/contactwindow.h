#ifndef CONTACTWINDOW_H
#define CONTACTWINDOW_H

class ContactList;
class ContactImportEvent;
#include "WtForwards.h"

#include <Wt/WContainerWidget>
class ContactWindow : public Wt::WContainerWidget
{
public:
    ContactWindow(Wt::WContainerWidget *parent);
    void contactsReceived(ContactImportEvent *ev);
private:
    ContactList *mpContactList;
};

#endif // CONTACTWINDOW_H
