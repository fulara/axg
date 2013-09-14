#include <Wt/WVBoxLayout>
#include <Wt/WSignal>
#include <Wt/WStackedWidget>
#include <functional>
#include <boost/foreach.hpp>
#include "logger.h"
#include "contactlist.h"
#include "ContactGroup.h"
#include "contactinfo.h"
#include "contactentry.h"
#include "WtUtilFuns.h"

ContactList::ContactList(Wt::WContainerWidget *parent)
    : Wt::WContainerWidget(parent),
      mpLastEntryClicked(0)
{
    setStyleClass("ContactList");

    mpOpenContactSignal = new Wt::Signal<ContactInfo>(this);
    mpOpenContactSignalForceOpen = new Wt::Signal<ContactInfo>(this);

    addContact(2577961,"Olek");
    mContacts.push_back(ContactInfo(2577961,"Olek","",true,true,true));
    addContact(6307826,"Pan LekPan LekPan LekPan Lek");
    mContacts.push_back(ContactInfo(6307826,"Pan Lek","",true,true,true));

}
ContactList::~ContactList()
{

}

void ContactList::addContact(unsigned int uin, std::string showName)
{
    ContactEntry *entry = new ContactEntry(uin,showName);
    entry->clicked().connect(std::bind([=]()
    {
        Logger::log("Entry Clicked..");
        onEntryClicked(entry);
    }));
    addToContactList(entry);
}

void ContactList::initContacts(const std::list<ContactGroup> &contactGroups)
{
    for(auto it = contactGroups.cbegin(); it != contactGroups.cend(); ++it)
    {
        const ContactGroup &group = *it;
        //if(group.name == "[default]")

        for(auto contactIt = group.contacts.cbegin(); contactIt != group.contacts.cend(); ++contactIt)
        {

            const ContactInfo &info = *contactIt;
            addContact(info.uin,info.showName);
            mContacts.push_back(info);

        }

    }


}
void ContactList::showFullList()
{
    WtUtilFuns::removeChildWidgets(this);

    BOOST_FOREACH(ContactEntry& entry, mContactEntries)
    {
        addWidget(&entry);
    }
}
void ContactList::filterList(std::string filter)
{
    WtUtilFuns::removeChildWidgets(this);

    BOOST_FOREACH(ContactEntry& entry, mContactEntries)
    {
        if(entry.matchesFilter(filter))
        {
            addWidget(&entry);
        }
    }
}
void ContactList::addToContactList(ContactEntry *widget)
{
    mContactEntries.push_back(widget);
    addWidget(widget);
}

Wt::Signal<ContactInfo> &ContactList::openContactForceOpenRequest()
{
    return *mpOpenContactSignalForceOpen;
}

void ContactList::handleNewContactInfoRequest(unsigned int uin)
{
    Logger::log("HandleNewContactInfoRequest");
    if(!findContactAndEmitInfo(uin))
    {
        createAnonymousContact(uin);

        findContactAndEmitInfo(uin);
    }
}
Wt::Signal<ContactInfo> &ContactList::openContactRequest()
{
    return *mpOpenContactSignal;
}

bool ContactList::findContactAndEmitInfo(unsigned int targetUin)
{
    BOOST_FOREACH(ContactInfo& contactInfo, mContacts)
    {
        if(contactInfo.uin == targetUin)
        {
            mpOpenContactSignal->emit(contactInfo);
            return true;
        }
    }
    return false;
}
void ContactList::findContactAndEmitForceOpenInfo(unsigned int targetUin)
{
    BOOST_FOREACH(ContactInfo& contactInfo,mContacts)
    {
        if(contactInfo.uin == targetUin)
        {
            mpOpenContactSignalForceOpen->emit(contactInfo);
            return;
        }
    }
    assert(false);
}

void ContactList::onEntryClicked(ContactEntry *invoker)
{
    if(mpLastEntryClicked == invoker)
    {
        findContactAndEmitForceOpenInfo(invoker->getUin());
        //static int fak = 0;
       // mpOpenContactSignal->emit(ContactInfo(2577961+ fak++,"Olek","Olek",false,false,false));
        mpLastEntryClicked->removeStyleClass("Highlighted");
        mpLastEntryClicked = 0;
    }
    else if(mpLastEntryClicked != 0)
    {
        mpLastEntryClicked->removeStyleClass("Highlighted");
        mpLastEntryClicked = invoker;
        mpLastEntryClicked->addStyleClass("Highlighted");
    }
    else
    {
        mpLastEntryClicked = invoker;
        mpLastEntryClicked->addStyleClass("Highlighted");
    }
}

void ContactList::createAnonymousContact(unsigned int uin)
{
    Logger::log("CreateAnonymousContact");
    mContacts.push_back(ContactInfo(uin,"","",true,true,true));
    addContact(uin,"AnonymousContact");
}
