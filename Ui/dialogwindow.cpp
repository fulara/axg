#include "dialogwindow.h"

#include <Wt/WText>
#include <Wt/WTable>
#include <Wt/WLineEdit>
#include <Wt/WTextEdit>
#include <Wt/WBreak>
#include <Wt/WPushButton>
#include <Wt/WLabel>
//#include <Wt/WK
#include "chathistory.h"
#include "messageevent.h"
#include "logger.h"

DialogWindow::DialogWindow(Wt::WContainerWidget *parent)
    :
      Wt::WContainerWidget(parent)

{
    mpTargetInfo = new Wt::WLabel("Guc",this);
    mpTargetInfo->setStyleClass("DialogWindowTargetInfo");
    new Wt::WBreak(this);
    mpChatHistory = new ChatHistory(this);
    new Wt::WBreak(this);
    mTargetUin = 3788407;
    //mTargetUin = 2577961;
    //mTargetUin = 1067760;
    mpTextArea = new Wt::WTextArea(this);
    mpTextArea->setStyleClass("DialogWindowTextArea");

    mpTextArea->keyWentUp().connect(
                [this](Wt::WKeyEvent &keyEvent)
    {
        if(keyEvent.key() == Wt::Key_Enter)
        {
            onSendRequest();
        }
    });
    new Wt::WBreak(this);
    mpSendMessageButton = new Wt::WPushButton("Send Message To Da Guc",this);

    mpSendMessageButton->clicked().connect(this,&DialogWindow::onSendRequest);
    setStyleClass("DialogWindow");

    mpSendMessageSignal = new Wt::Signal<unsigned int, std::string>(this);



}

DialogWindow::~DialogWindow()
{
    delete mpChatHistory;
}

void DialogWindow::onSendRequest()
{
    if(mpTextArea->text().empty())
        return;
    auto narrowed = mpTextArea->text().narrow();
    if(narrowed.back() == '\n')
        narrowed.erase(--narrowed.end());
    mpSendMessageSignal->emit(mTargetUin,narrowed);
    mpChatHistory->addSentMessage(narrowed);
    mpTextArea->setText("");

}

void DialogWindow::messageReceived(MessageEvent *ev)
{
    mpChatHistory->addRecvMessage(ev->fromUin,ev->content);
}

 Wt::Signal<unsigned int,std::string> &DialogWindow::sendMessageRequest()
{
    return *mpSendMessageSignal;
}
