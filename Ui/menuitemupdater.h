#ifndef MENUITEMUPDATER_H
#define MENUITEMUPDATER_H

struct TypingNotificationEvent;
class DialogWindow;
#include "WtForwards.h"

#include <string>

class MenuItemUpdater
{
public:
    MenuItemUpdater(const std::string &targetName,DialogWindow *parent);

    void updateMenuItemReference(Wt::WMenuItem *menuItem);

    void parentLostFocus();
    void parentGainedFocus();
    void onNotificationEvent(TypingNotificationEvent *ev);
    void onMessageRcv(const int totalUnreadMessages);
private:
    int mUnreadMsgCount;
    std::string mTargetName;
    Wt::WMenuItem *mpMenuItem;
    Wt::WTimer *mpNotificationFadeOutTimer;

    void onNotificationFadeOutTimeuot();

    static const std::string TYPING_STYLE_CLASS;
    static const std::string GOT_MSG_STYLE_CLASS;
};

#endif // MENUITEMUPDATER_H
