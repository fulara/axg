#include <Wt/WMenuItem>
#include <Wt/WTimer>
#include "menuitemupdater.h"
#include "dialogwindow.h"
#include "TypingNotificationEvent.h"

MenuItemUpdater::MenuItemUpdater(const std::string &targetName, DialogWindow *parent)
    :
    mTargetName(targetName)
{
    mpMenuItem = 0;
    mUnreadMsgCount = 0;
    mpNotificationFadeOutTimer = new Wt::WTimer(parent);
    mpNotificationFadeOutTimer->timeout().connect(boost::bind(&MenuItemUpdater::onNotificationFadeOutTimeuot,this));
    mpNotificationFadeOutTimer->setSingleShot(true);
    mpNotificationFadeOutTimer->setInterval(5000);
}
void MenuItemUpdater::updateMenuItemReference(Wt::WMenuItem *menuItem)
{
    mpMenuItem = menuItem;
}

void MenuItemUpdater::parentLostFocus()
{

}
void MenuItemUpdater::parentGainedFocus()
{
    if(mpMenuItem != 0)
    {
        mpMenuItem->removeStyleClass(GOT_MSG_STYLE_CLASS);
        mpMenuItem->setText(mTargetName);
    }
    mUnreadMsgCount = 0;
}

void MenuItemUpdater::onMessageRcv(const int totalUnreadMessages)
{
    if(mpMenuItem != 0)
    {
        mpMenuItem->addStyleClass(GOT_MSG_STYLE_CLASS);
        mpMenuItem->setText(mTargetName + "(" + boost::lexical_cast<std::string>(totalUnreadMessages) + ")");
    }


    mUnreadMsgCount = totalUnreadMessages;
}

void MenuItemUpdater::onNotificationEvent(TypingNotificationEvent *ev)
{
    if(mpMenuItem == NULL)
        return;
    mpNotificationFadeOutTimer->stop();
    if(ev->length == 0)
    {
        mpMenuItem->removeStyleClass(TYPING_STYLE_CLASS);
        if(this->mUnreadMsgCount > 0)
            mpMenuItem->addStyleClass(GOT_MSG_STYLE_CLASS);
        return;
    }
    mpNotificationFadeOutTimer->start();
    mpMenuItem->removeStyleClass(GOT_MSG_STYLE_CLASS);
    mpMenuItem->addStyleClass(TYPING_STYLE_CLASS);
}
void MenuItemUpdater::onNotificationFadeOutTimeuot()
{
    if(mpMenuItem)
    {
        mpMenuItem->removeStyleClass(TYPING_STYLE_CLASS);
        if(mUnreadMsgCount)
            mpMenuItem->addStyleClass(GOT_MSG_STYLE_CLASS);
    }
}
const std::string MenuItemUpdater::TYPING_STYLE_CLASS("DialogWindowMenuItemTyping");
const std::string MenuItemUpdater::GOT_MSG_STYLE_CLASS("DialogWindowMenuItemGotMsg");
