#include "contactwindow.h"
#include "contactlist.h"
ContactWindow::ContactWindow(Wt::WContainerWidget *parent)
    : Wt::WContainerWidget(parent)
{
    setStyleClass("ContactWindow");
    resize("250px","500px");

    mpContactList = new ContactList(this);
}
