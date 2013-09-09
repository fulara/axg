#ifndef DIALOGWINDOW_H
#define DIALOGWINDOW_H

class ChatHistory;
class MessageEvent;
class ContactImportEvent;
#include "WtForwards.h"

#include <Wt/WContainerWidget>
#include <Wt/WSignal>
#include <Wt/WJavaScript>
class DialogWindow : public Wt::WContainerWidget
{
public:
    DialogWindow(unsigned int targetUin, std::string targetName, Wt::WContainerWidget *parent = 0);
    ~DialogWindow();

    Wt::Signal<unsigned int,std::string> &sendMessageRequest();

    void messageReceived(MessageEvent *ev);
    void reinitJS();
private:
    unsigned int mTargetUin;
    std::string mTargetName;

    Wt::WLabel *mpTargetInfo;
    ChatHistory *mpChatHistory;
    Wt::WTextArea *mpTextArea;
    Wt::WPushButton *mpSendMessageButton;

    Wt::Signal<unsigned int, std::string> *mpSendMessageSignal;
    Wt::JSignal<std::string> *mpTextAreaEnterSignal;

    void initOnKeyUpJSTextArea();
    void onTextAreaEnterPress(std::string content);
    void onSendButton();

};

#endif // DIALOGWINDOW_H
