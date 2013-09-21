#ifndef DIALOGWINDOW_H
#define DIALOGWINDOW_H

class ChatHistory;
class MessageEvent;
class TypingNotificationEvent;
class ContactImportEvent;
class MenuItemUpdater;
struct ContactInfo;
#include "WtForwards.h"

#include <Wt/WContainerWidget>
#include <Wt/WSignal>
#include <Wt/WJavaScript>
class DialogWindow : public Wt::WContainerWidget
{
public:
    DialogWindow(unsigned int userUin,unsigned int targetUin, std::string targetName, Wt::WContainerWidget *parent = 0);
    ~DialogWindow();

    Wt::Signal<unsigned int,std::string> &sendMessageRequest();
    Wt::Signal<unsigned int, int> &sendTypingNotificationRequest();
    Wt::Signal<ContactInfo,unsigned int>& newUnreadMessage();
    Wt::Signal<unsigned int> &messagesRead();

    void activated();
    void deactivated();
    void updateMenuItem(Wt::WMenuItem *newMenuItem);
    void focusOnTextArea();
    void messageReceived(MessageEvent *ev);
    void handleTypingNotificationEvent(TypingNotificationEvent *ev);
    void onSendRequest(std::string narrowed);

private:
    bool mIsActive;
    unsigned int mUserUin;
    unsigned int mUnreadMsgCount;
    unsigned int mTargetUin;
    std::string mTargetName;

    MenuItemUpdater *mpMenuItemUpdater;

    Wt::WLabel *mpTargetInfo;
    ChatHistory *mpChatHistory;
    Wt::WTextArea *mpTextArea;
    Wt::WPushButton *mpSendMessageButton;



    Wt::Signal<unsigned int, std::string> *mpSendMessageSignal;
    Wt::Signal<unsigned int, int> *mpSendTypingNotificationSignal;
    Wt::Signal<ContactInfo, unsigned int> *mpNewUnreadMessageSignal;
    Wt::Signal<unsigned int> *mpMessagesReadSignal;
    Wt::JSignal<std::string> *mpTextAreaEnterSignal;
    Wt::JSignal<int> *mpTextLengthUpdateSignal;

    void initOnKeyUpJSTextArea();
    void initWidgets();
    void initAndConnectSignals();
    void onTextLengthUpdate(int length);
    void onTextAreaEnterPress(std::string content);
    void onSendButton();




};

#endif // DIALOGWINDOW_H
