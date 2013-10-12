#include <Wt/WTabWidget>
#include <Wt/WMenuItem>
#include <Wt/WMenu>
#include <Wt/WText>
#include <boost/functional.hpp>
#include <boost/foreach.hpp>
#include <exception>

#include "logger.h"
#include "contactinfo.h"
#include "dialogwindow.h"
#include "dialogwindowholder.h"
#include "messageevent.h"
#include "TypingNotificationEvent.h"
#include "draggablediv.h"

DialogWindowHolder::DialogWindowHolder(const unsigned int userUin, Wt::WContainerWidget *parent)
    : Wt::WContainerWidget(parent),
      mUserUin(userUin),
      mpTabWidget(new Wt::WTabWidget(this)),
      mpNewContactInfoRequest(new Wt::Signal<unsigned int>(this)),
      mpSendMessageSignal(new Wt::Signal<unsigned int, std::string>(this)),
      mpSendTypingNotificationSignal(new Wt::Signal<unsigned int, int>(this)),
      mpNewUnreadMessageSignal(new Wt::Signal<ContactInfo, unsigned int>(this)),
      mpMessagesReadSignal(new Wt::Signal<unsigned int>(this))
{
    mpTabWidget->resize("100%","100%");
    resize(400,400);
    setStyleClass("DialogWindowHolder");
    mpTabWidget->tabClosed().connect(this,&DialogWindowHolder::onTabClosed);
    mpTabWidget->currentChanged().connect(this,&DialogWindowHolder::activeChanged);
    mCurrentlyActiveWindow = 0;
    new DraggableDiv(this);
    hide();
    std::stringstream ss;
    ss << "$(function() {";
    ss << "$( '#" << id() << "').resizable();";
    ss << "});";
    doJavaScript(ss.str());
}
DialogWindowHolder::~DialogWindowHolder()
{
    BOOST_FOREACH(auto pair, mDialogWindows)
    {
        delete pair.second;
    }
    delete mpNewContactInfoRequest;
    delete mpSendMessageSignal;
}
void DialogWindowHolder::onTabClosed(int index)
{
    DialogWindow *tab = static_cast<DialogWindow*>(mpTabWidget->widget(index));
    tab->updateMenuItem(NULL);
    mpTabWidget->removeTab(tab);

    if(mpTabWidget->count())
    {
        if(mpTabWidget->count() == index)
        {
            mpTabWidget->setCurrentIndex(index-1);
            activeChanged(index-1);
        }
        else
        {
            mpTabWidget->setCurrentIndex(index);
            activeChanged(index);
        }
    }
    else
    {
        activeChanged(-1);
        hide();
    }

}

void DialogWindowHolder::activeChanged(int index)
{


    if(mCurrentlyActiveWindow != 0)
    {
        mCurrentlyActiveWindow->deactivated();
    }
    if(index == -1)
    {
        mCurrentlyActiveWindow = 0;
    }
    else
    {
        DialogWindow *activeWindow = static_cast<DialogWindow*>(mpTabWidget->widget(index));
        mCurrentlyActiveWindow = activeWindow;
        mCurrentlyActiveWindow->activated();
    }
}
bool DialogWindowHolder::isDialogWindowOpen(unsigned int targetUin)
{
    auto it = mDialogWindows.find(targetUin);// != mDialogWindows.end();
    if(it == mDialogWindows.end()) return false;

    return mpTabWidget->indexOf(it->second) != -1;
}

DialogWindow* DialogWindowHolder::getDialogWindowById(unsigned int targetUin)
{
    for(auto it = mDialogWindows.begin(); it != mDialogWindows.end(); ++it)
    {
        if(it->first == targetUin)
            return it->second;
    }
    throw std::runtime_error("No Dialog Window For Specified uin");
}

void DialogWindowHolder::messageReceived(MessageEvent *msg)
{
    if(!isDialogWindowOpen(msg->fromUin))
        mpNewContactInfoRequest->emit(msg->fromUin);

    unsigned int targetUin = msg->fromUin;
    DialogWindow *dialogWindow = getDialogWindowById(targetUin);
    dialogWindow->messageReceived(msg);
}
void DialogWindowHolder::typingNotificationReceived(TypingNotificationEvent *event)
{
    if(isDialogWindowOpen(event->fromUin))
    {
        DialogWindow* dialogWindow = getDialogWindowById(event->fromUin);
        dialogWindow->handleTypingNotificationEvent(event);
    }
}

Wt::Signal<unsigned int> &DialogWindowHolder::newContactInfoRequest()
{
    return *mpNewContactInfoRequest;
}

DialogWindow* DialogWindowHolder::openDialogWindow(ContactInfo contactinfo)
{
    auto it =mDialogWindows.begin();
    if((it = mDialogWindows.find(contactinfo.uin)) != mDialogWindows.end())
    {
        DialogWindow *dialogWindow = it->second;
        int index = mpTabWidget->indexOf(dialogWindow);
        if(index == -1)
        {
            auto menuItem = mpTabWidget->addTab(dialogWindow,contactinfo.getDisplayName(),Wt::WTabWidget::PreLoading);
            dialogWindow->updateMenuItem(menuItem);
            menuItem->setCloseable(true);
            return dialogWindow;
        }
        mpTabWidget->setCurrentIndex(index);
        return dialogWindow;
    }

    return createNewDialogWindow(contactinfo);


}
void DialogWindowHolder::enableJQuerySortable(std::string menuId)
{
    std::stringstream ss;

    ss << "$(\"#" << menuId << "\").sortable()";
    doJavaScript(ss.str());
}

void DialogWindowHolder::connectSignalsFromNewDialog(DialogWindow* newDialogWindow)
{
    newDialogWindow->sendMessageRequest().connect(this,&DialogWindowHolder::sendMessageForward);
    newDialogWindow->sendTypingNotificationRequest().connect(this, &DialogWindowHolder::forwardNotificationRequest);
    newDialogWindow->newUnreadMessage().connect(this,&DialogWindowHolder::forwardUnreadMessagesSignal);
    newDialogWindow->messagesRead().connect(this,&DialogWindowHolder::forwardMessagesReadSignal);
}

DialogWindow *DialogWindowHolder::createNewDialogWindow(ContactInfo contactInfo)
{
    DialogWindow* newDialogWindow = new DialogWindow(mUserUin,contactInfo.uin,contactInfo.getDisplayName());
    connectSignalsFromNewDialog(newDialogWindow);
    mDialogWindows.insert(std::make_pair(contactInfo.uin,newDialogWindow));
    auto menuItem = mpTabWidget->addTab(newDialogWindow,contactInfo.getDisplayName(),Wt::WTabWidget::PreLoading);
    std::string menuId = menuItem->parentMenu()->id();
    enableJQuerySortable(menuId);
    menuItem->setCloseable(true);
    newDialogWindow->updateMenuItem(menuItem);
    show();
    return newDialogWindow;
}
void DialogWindowHolder::forwardNotificationRequest(unsigned int targetUin, int length)
{
    mpSendTypingNotificationSignal->emit(targetUin,length);
}

void DialogWindowHolder::openDialogWindowRequest(ContactInfo contactinfo)
{
    openDialogWindow(contactinfo);
    if(mpTabWidget->count() == 1)
        activeChanged(0);
}

void DialogWindowHolder::openDialogWindowAndActivateRequest(ContactInfo contactInfo)
{
    DialogWindow *dialogWindow = openDialogWindow(contactInfo);
    if(mpTabWidget->count() == 1)
        activeChanged(0);
    dialogWindow->focusOnTextArea();
    mpTabWidget->setCurrentWidget(dialogWindow);
    //activeChanged(mpTabWidget->indexOf(dialogWindow));
}

Wt::Signal<unsigned int, std::string> &DialogWindowHolder::sendMessageRequest()
{
    return *mpSendMessageSignal;
}
Wt::Signal<unsigned int, int> &DialogWindowHolder::sendTypingNotificationRequest()
{
    return *mpSendTypingNotificationSignal;
}
Wt::Signal<ContactInfo,unsigned int>& DialogWindowHolder::newUnreadMessage()
{
    return *mpNewUnreadMessageSignal;
}

Wt::Signal<unsigned int> &DialogWindowHolder::messagesRead()
{
    return *mpMessagesReadSignal;
}

void DialogWindowHolder::sendMessageForward(unsigned int targetUin, const std::string &msg)
{
    mpSendMessageSignal->emit(targetUin,msg);
}

void DialogWindowHolder::forwardUnreadMessagesSignal(ContactInfo info, unsigned int msgCount)
{
    mpNewUnreadMessageSignal->emit(info,msgCount);
}
void DialogWindowHolder::forwardMessagesReadSignal(unsigned int uin)
{
    mpMessagesReadSignal->emit(uin);
}
