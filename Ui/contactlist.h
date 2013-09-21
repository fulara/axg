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
#include <boost/ptr_container/ptr_map.hpp>
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
    void clearPanels();
    void createDebugContactGroup();
private:

    ContactEntry *mpLastEntryClicked;
    Wt::Signal<ContactInfo> *mpOpenContactSignal;
    Wt::Signal<ContactInfo> *mpOpenContactSignalForceOpen;
    std::map<ContactGroup,Wt::WPanel*> mContactPanels;
    std::list<ContactInfo> mContacts;
    boost::ptr_list<ContactEntry> mContactEntries;
    std::map<unsigned int, ContactEntry*> mUinToEntry;



    bool findContactAndEmitInfo(unsigned int targetUin);
    void addToContactList(ContactEntry *widget);
    void findContactAndEmitForceOpenInfo(unsigned int targetUin);
    void addContact(unsigned int uin, std::string showName);
    void onEntryClicked(ContactEntry *invoker);
    void createAnonymousContact(unsigned int uin);

};

#endif // CONTACTLIST_H
