#include <Wt/WVBoxLayout>
#include <Wt/WSignal>
#include <Wt/WStackedWidget>
#include <Wt/WPanel>
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


    createDebugContactGroup();
}
ContactList::~ContactList()
{
    WtUtilFuns::removeChildWidgets(this);
    clearPanels();

    for(std::map<ContactGroup,Wt::WPanel*>::iterator it = mContactPanels.begin(); it != mContactPanels.end(); ++it)
    {
        Wt::WPanel *panel = it->second;
        delete panel;
    }
}

void ContactList::createDebugContactGroup()
{
    std::multiset<ContactInfo> debugContacts;
    ContactInfo mineContact(2577961,"Olek","",true,true,true);
    addContact(2577961,"Olek");
    mContacts.push_back(mineContact);
    debugContacts.insert(mineContact);;

    addContact(6307826,"Pan LekPan LekPan LekPan Lek");
    ContactInfo leksContact(6307826,"Pan Lek","",true,true,true);
    mContacts.push_back(leksContact);
    debugContacts.insert(leksContact);

    ContactInfo guccisContact(3788407,"Pan Gucci","",true,true,true);
    addContact(3788407,"Pan Gucci");
    mContacts.push_back(guccisContact);
    debugContacts.insert(guccisContact);

    ContactInfo bazContact(1067760,"Pan Baz","",true,true,true);
    addContact(1067760,"Pan Baz");
    mContacts.push_back(bazContact);
    debugContacts.insert(bazContact);;

    ContactGroup debugGroup(true,true,"DebugGroup",debugContacts);

    Wt::WPanel *panel = new Wt::WPanel();
    panel->setTitle(debugGroup.name);
    panel->setCollapsible(true);
    panel->setCentralWidget(new Wt::WContainerWidget());
    mContactPanels.insert(std::make_pair(debugGroup,panel));
}

void ContactList::addContact(unsigned int uin, std::string showName)
{
    ContactEntry *entry = new ContactEntry(uin,showName);

    entry->clicked().connect(std::bind([=]()
    {
        onEntryClicked(entry);
    }));
    addToContactList(entry);
}

void ContactList::initContacts(const std::list<ContactGroup> &contactGroups)
{
    for(auto it = contactGroups.cbegin(); it != contactGroups.cend(); ++it)
    {
        const ContactGroup &group = *it;
        auto groupName = group.name;
        Wt::WPanel * panel = new Wt::WPanel;
        panel->setTitle(groupName);
        panel->setCollapsible(true);
        panel->setCentralWidget(new Wt::WContainerWidget());
        mContactPanels.insert(std::make_pair(group,panel));

        for(auto contactIt = group.contacts.cbegin(); contactIt != group.contacts.cend(); ++contactIt)
        {

            const ContactInfo &info = *contactIt;
            addContact(info.uin,info.showName);
            mContacts.push_back(info);

        }

    }

    showFullList();

}
void ContactList::clearPanels()
{
    for(auto it = mContactPanels.begin(); it != mContactPanels.end(); ++it)
    {
        Wt::WPanel *panel = it->second;
        Wt::WContainerWidget *centralWidget = static_cast<Wt::WContainerWidget*>(panel->centralWidget());
        WtUtilFuns::removeChildWidgets(centralWidget);
    }
}

void ContactList::showFullList()
{
    WtUtilFuns::removeChildWidgets(this);
    clearPanels();

    for(std::map<ContactGroup,Wt::WPanel*>::iterator it = mContactPanels.begin(); it != mContactPanels.end(); ++it)
    {

        //uto& contacts = it->;
        const ContactGroup &group = it->first;
        auto panel = it->second;
        Wt::WContainerWidget* container = static_cast<Wt::WContainerWidget*>(panel->centralWidget());
        BOOST_FOREACH(const ContactInfo& info, group.contacts)
        {
            ContactEntry *entry = mUinToEntry[info.uin];
            container->addWidget(entry);
        }
        if(container->count())
            addWidget(panel);
    }
}
void ContactList::filterList(std::string filter)
{
    WtUtilFuns::removeChildWidgets(this);
    clearPanels();
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
    mUinToEntry.insert(std::make_pair(widget->getUin(),widget));
}

Wt::Signal<ContactInfo> &ContactList::openContactForceOpenRequest()
{
    return *mpOpenContactSignalForceOpen;
}

void ContactList::handleNewContactInfoRequest(unsigned int uin)
{
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
