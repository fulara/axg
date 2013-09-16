#ifndef DIALOGWINDOWHOLDER_H
#define DIALOGWINDOWHOLDER_H

class ContactInfo;
class DialogWindow;
class MessageEvent;
class TypingNotificationEvent;
#include <map>
#include <Wt/WContainerWidget>
class DialogWindowHolder : public Wt::WContainerWidget
{
public:
    DialogWindowHolder(Wt::WContainerWidget *parent = 0);
    ~DialogWindowHolder();

    DialogWindow* openDialogWindow(ContactInfo contactinfo);
    void openDialogWindowRequest(ContactInfo contactinfo);
    void openDialogWindowAndActivateRequest(ContactInfo);
    void messageReceived(MessageEvent *msg);
    void typingNotificationReceived(TypingNotificationEvent* event);
    Wt::Signal<unsigned int> &newContactInfoRequest();
    Wt::Signal<unsigned int, std::string> &sendMessageRequest();
    Wt::Signal<unsigned int, int> &sendTypingNotificationRequest();

private:
    Wt::WTabWidget *mpTabWidget;
    Wt::Signal<unsigned int> *mpNewContactInfoRequest;
    Wt::Signal<unsigned int, std::string> *mpSendMessageSignal;
    Wt::Signal<unsigned int, int> *mpSendTypingNotificationSignal;
    std::map<unsigned int, DialogWindow*> mDialogWindows;

    DialogWindow *createNewDialogWindow(ContactInfo contactInfo);
    void activeChanged(int index);
    void onTabClosed(int index);
    bool isDialogWindowOpen(unsigned int uin);
    DialogWindow* getDialogWindowById(unsigned int targetUin);
    void sendMessageForward(unsigned int targetUin,const std::string& msg);
    void forwardNotificationRequest(unsigned int targetUin, int length);

};

#endif // DIALOGWINDOWHOLDER_H
