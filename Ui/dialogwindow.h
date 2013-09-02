#ifndef DIALOGWINDOW_H
#define DIALOGWINDOW_H

class ChatHistory;
class MessageEvent;
#include "WtForwards.h"

#include <Wt/WContainerWidget>
#include <Wt/WSignal>
#include <Wt/WJavaScript>
class DialogWindow : public Wt::WContainerWidget
{
public:
    DialogWindow(Wt::WContainerWidget *parent);
    ~DialogWindow();

    Wt::Signal<unsigned int,std::string> &sendMessageRequest();

    void messageReceived(MessageEvent *ev);
private:
    unsigned int mTargetUin;
    std::string mTargetName;

    Wt::WLabel *mpTargetInfo;
    ChatHistory *mpChatHistory;
    Wt::WTextArea *mpTextArea;
    Wt::WPushButton *mpSendMessageButton;

    Wt::Signal<unsigned int, std::string> *mpSendMessageSignal;



    void onSendRequest();

};

#endif // DIALOGWINDOW_H
