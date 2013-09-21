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
    DialogWindowHolder(const unsigned int userUin,Wt::WContainerWidget *parent = 0);
    ~DialogWindowHolder();

    DialogWindow* openDialogWindow(ContactInfo contactinfo);
    void openDialogWindowRequest(ContactInfo contactinfo);
    void openDialogWindowAndActivateRequest(ContactInfo);
    void messageReceived(MessageEvent *msg);
    void typingNotificationReceived(TypingNotificationEvent* event);
    Wt::Signal<unsigned int> &newContactInfoRequest();
    Wt::Signal<unsigned int, std::string> &sendMessageRequest();
    Wt::Signal<unsigned int, int> &sendTypingNotificationRequest();
    Wt::Signal<ContactInfo,unsigned int>& newUnreadMessage();
    Wt::Signal<unsigned int> &messagesRead();
    void enableJQuerySortable(std::string menuId);
    void connectSignalsFromNewDialog(DialogWindow* newDialogWindow);
private:

    const unsigned int mUserUin;
    Wt::WTabWidget *mpTabWidget;
    Wt::Signal<unsigned int> *mpNewContactInfoRequest;
    Wt::Signal<unsigned int, std::string> *mpSendMessageSignal;
    Wt::Signal<unsigned int, int> *mpSendTypingNotificationSignal;
    Wt::Signal<ContactInfo, unsigned int> *mpNewUnreadMessageSignal;
    Wt::Signal<unsigned int> *mpMessagesReadSignal;
    std::map<unsigned int, DialogWindow*> mDialogWindows;
    DialogWindow *mCurrentlyActiveWindow;

    DialogWindow *createNewDialogWindow(ContactInfo contactInfo);
    void activeChanged(int index);
    void onTabClosed(int index);
    bool isDialogWindowOpen(unsigned int uin);
    DialogWindow* getDialogWindowById(unsigned int targetUin);
    void sendMessageForward(unsigned int targetUin,const std::string& msg);
    void forwardNotificationRequest(unsigned int targetUin, int length);
    void forwardUnreadMessagesSignal(ContactInfo info, unsigned int msgCount);
    void forwardMessagesReadSignal(unsigned int uin);

};

#endif // DIALOGWINDOWHOLDER_H
