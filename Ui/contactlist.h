#ifndef CONTACTLIST_H
#define CONTACTLIST_H

#include "WtForwards.h"
class ContactEntry;
class ContactGroup;
class ContactInfo;

#include <list>
#include <Wt/WSignal>
#include <Wt/WContainerWidget>
class ContactList : public Wt::WContainerWidget
{
public:
    ContactList(Wt::WContainerWidget *parent);

    Wt::Signal<ContactInfo> &openContactRequest();
    void initContacts(const std::list<ContactGroup> & contactGroups);
    void handleNewContactInfoRequest(unsigned int uin);

private:

    ContactEntry *mpLastEntryClicked;
    Wt::Signal<ContactInfo> *mpOpenContactSignal;
    std::list<ContactInfo> mContacts;

    bool findContactAndEmitInfo(unsigned int targetUin);
    void addContact(unsigned int uin, std::string showName);
    void onEntryClicked(ContactEntry *invoker);
    void createAnonymousContact(unsigned int uin);
};

#endif // CONTACTLIST_H
