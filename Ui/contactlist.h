#ifndef CONTACTLIST_H
#define CONTACTLIST_H

#include "WtForwards.h"
class ContactEntry;
class ContactGroup;
class ContactInfo;

#include <list>
#include <Wt/WSignal>
#include <Wt/WContainerWidget>
#include <boost/ptr_container/ptr_list.hpp>
class ContactList : public Wt::WContainerWidget
{
public:
    ContactList(Wt::WContainerWidget *parent);
    ~ContactList();
    Wt::Signal<ContactInfo> &openContactRequest();
    Wt::Signal<ContactInfo> &openContactForceOpenRequest();
    void initContacts(const std::list<ContactGroup> & contactGroups);
    void handleNewContactInfoRequest(unsigned int uin);
    void showFullList();
    void filterList(std::string filter);
private:

    ContactEntry *mpLastEntryClicked;
    Wt::Signal<ContactInfo> *mpOpenContactSignal;
    Wt::Signal<ContactInfo> *mpOpenContactSignalForceOpen;
    std::list<ContactInfo> mContacts;
    boost::ptr_list<ContactEntry> mContactEntries;

    bool findContactAndEmitInfo(unsigned int targetUin);
    void addToContactList(ContactEntry *widget);
    void findContactAndEmitForceOpenInfo(unsigned int targetUin);
    void addContact(unsigned int uin, std::string showName);
    void onEntryClicked(ContactEntry *invoker);
    void createAnonymousContact(unsigned int uin);
};

#endif // CONTACTLIST_H
