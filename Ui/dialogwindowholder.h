#ifndef DIALOGWINDOWHOLDER_H
#define DIALOGWINDOWHOLDER_H

class ContactInfo;
class DialogWindow;
class MessageEvent;
#include <map>
#include <Wt/WContainerWidget>
class DialogWindowHolder : public Wt::WContainerWidget
{
public:
    DialogWindowHolder(Wt::WContainerWidget *parent = 0);
    ~DialogWindowHolder();

    void openDialogWindowRequest(ContactInfo contactinfo);
    void messageReceived(MessageEvent *msg);
    Wt::Signal<unsigned int> &newContactInfoRequest();
    Wt::Signal<unsigned int, std::string> &sendMessageRequest();

private:
    Wt::WTabWidget *mpTabWidget;
    Wt::Signal<unsigned int> *mpNewContactInfoRequest;
    Wt::Signal<unsigned int, std::string> *mpSendMessageSignal;
    std::map<unsigned int, DialogWindow*> mDialogWindows;

    void activeChanged(int index);
    void onTabClosed(int index);
    bool isDialogWindowOpen(unsigned int uin);
    DialogWindow* getDialogWindowById(unsigned int targetUin);
    void sendMessageForward(unsigned int targetUin,const std::string& msg);

};

#endif // DIALOGWINDOWHOLDER_H
