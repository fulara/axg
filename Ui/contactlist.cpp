#include <Wt/WVBoxLayout>
#include <Wt/WSignal>
#include <functional>
#include "logger.h"
#include "contactlist.h"
#include "ContactGroup.h"
#include "contactinfo.h"
#include "contactentry.h"
#include "boost/foreach.hpp"
ContactList::ContactList(Wt::WContainerWidget *parent)
    : Wt::WContainerWidget(parent),
      mpLastEntryClicked(0)
{
    setStyleClass("ContactList");
    mpOpenContactSignal = new Wt::Signal<ContactInfo>(this);


    addContact(3788407,"Guc");
    mContacts.push_back(ContactInfo(3788407,"Guc","",true,true,true));
    //addContact(2577966,"Tomek");
    /*addWidget(new ContactEntry(2577961,"Olek"));
    addWidget(new ContactEntry(2577961,"Olek"));
    addWidget(new ContactEntry(2577961,"Olek"));
    addWidget(new ContactEntry(2577961,"Olek"));
    addWidget(new ContactEntry(2577961,"Olek"));
    addWidget(new ContactEntry(2577961,"Olek"));
    addWidget(new ContactEntry(2577961,"Olek"));
    addWidget(new ContactEntry(2577961,"Olek"));
    addWidget(new ContactEntry(2577961,"Olek"));
    addWidget(new ContactEntry(2577961,"Olek"));
    addWidget(new ContactEntry(2577961,"Olek"));
    addWidget(new ContactEntry(2577961,"Olek"));
    addWidget(new ContactEntry(2577961,"Olek"));
    addWidget(new ContactEntry(2577961,"Olek"));*/


}

void ContactList::addContact(unsigned int uin, std::string showName)
{
    ContactEntry *entry = new ContactEntry(uin,showName);
    entry->clicked().connect(std::bind([=]()
    {
        Logger::log("Entry Clicked..");
        onEntryClicked(entry);
    }));
    addWidget(entry);
}

void ContactList::initContacts(const std::list<ContactGroup> &contactGroups)
{
    for(auto it = contactGroups.cbegin(); it != contactGroups.cend(); ++it)
    {
        const ContactGroup &group = *it;
        for(auto contactIt = group.contacts.cbegin(); contactIt != group.contacts.cend(); ++contactIt)
        {
            const ContactInfo &info = *contactIt;
            addContact(info.uin,info.showName);
            mContacts.push_back(info);
        }

    }


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

void ContactList::onEntryClicked(ContactEntry *invoker)
{
    if(mpLastEntryClicked == invoker)
    {
        if(findContactAndEmitInfo(invoker->getUin()))
        {
            //..
        }
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
