#ifndef CONTACTWINDOW_H
#define CONTACTWINDOW_H

class ContactList;
#include "WtForwards.h"

#include <Wt/WContainerWidget>
class ContactWindow : public Wt::WContainerWidget
{
public:
    ContactWindow(Wt::WContainerWidget *parent);

private:
    ContactList *mpContactList;
};

#endif // CONTACTWINDOW_H
