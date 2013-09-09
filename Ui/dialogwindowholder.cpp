#include <Wt/WTabWidget>
#include <Wt/WMenuItem>
#include <boost/functional.hpp>
#include <boost/foreach.hpp>
#include <exception>

#include "logger.h"
#include "contactinfo.h"
#include "dialogwindow.h"
#include "dialogwindowholder.h"
#include "messageevent.h"

DialogWindowHolder::DialogWindowHolder(Wt::WContainerWidget *parent)
    : Wt::WContainerWidget(parent),
      mpTabWidget(new Wt::WTabWidget(this)),
      mpNewContactInfoRequest(new Wt::Signal<unsigned int>(this)),
      mpSendMessageSignal(new Wt::Signal<unsigned int, std::string>(this))
{
    setStyleClass("DialogWindowHolder");
    mpTabWidget->tabClosed().connect(this,&DialogWindowHolder::onTabClosed);
    mpTabWidget->currentChanged().connect(this,&DialogWindowHolder::activeChanged);
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
    auto tab = mpTabWidget->widget(index);
    mpTabWidget->removeTab(tab);

    if(mpTabWidget->count())
    {
        if(mpTabWidget->count() == index)
            mpTabWidget->setCurrentIndex(index-1);
        else
            mpTabWidget->setCurrentIndex(index);
    }

}

void DialogWindowHolder::activeChanged(int index)
{
    Logger::log("Index changed..");
    DialogWindow *window = static_cast<DialogWindow*>(mpTabWidget->widget(index));
    window->reinitJS();
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
Wt::Signal<unsigned int> &DialogWindowHolder::newContactInfoRequest()
{
    return *mpNewContactInfoRequest;
}

void DialogWindowHolder::openDialogWindowRequest(ContactInfo contactinfo)
{
    auto it =mDialogWindows.begin();
    if((it = mDialogWindows.find(contactinfo.uin)) != mDialogWindows.end())
    {
        auto dialogWindow = it->second;
        int index = mpTabWidget->indexOf(dialogWindow);
        if(index == -1)
        {
            auto menuItem = mpTabWidget->addTab(dialogWindow,contactinfo.getDisplayName(),Wt::WTabWidget::PreLoading);
            menuItem->setCloseable(true);
            dialogWindow->reinitJS();
            return;
        }
        mpTabWidget->setCurrentIndex(index);
        return;
    }
    DialogWindow* newDialogWindow = new DialogWindow(contactinfo.uin,contactinfo.getDisplayName());
    newDialogWindow->sendMessageRequest().connect(this,&DialogWindowHolder::sendMessageForward);
    mDialogWindows.insert(std::make_pair(contactinfo.uin,newDialogWindow));
    auto menuItem = mpTabWidget->addTab(newDialogWindow,contactinfo.getDisplayName(),Wt::WTabWidget::PreLoading);
    menuItem->setCloseable(true);
    newDialogWindow->reinitJS();

}
Wt::Signal<unsigned int, std::string> &DialogWindowHolder::sendMessageRequest()
{
    return *mpSendMessageSignal;
}

void DialogWindowHolder::sendMessageForward(unsigned int targetUin, const std::string &msg)
{
    mpSendMessageSignal->emit(targetUin,msg);
}
