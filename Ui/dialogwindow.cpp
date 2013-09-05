#include "dialogwindow.h"

#include <Wt/WText>
#include <Wt/WTable>
#include <Wt/WLineEdit>
#include <Wt/WTextEdit>
#include <Wt/WBreak>
#include <Wt/WPushButton>
#include <Wt/WLabel>
#include <Wt/WVBoxLayout>
#include "chathistory.h"
#include "messageevent.h"
#include "logger.h"

DialogWindow::DialogWindow(Wt::WContainerWidget *parent)
    :
      Wt::WContainerWidget(parent)

{
    setStyleClass("DialogWindow");
    resize("400px","430px");
    mpTargetInfo = new Wt::WLabel("Guc",this);
    mpTargetInfo->setStyleClass("DialogWindowTargetInfo");
    Wt::WVBoxLayout *layout = new Wt::WVBoxLayout(this);
    layout->addWidget(mpTargetInfo);
    mpChatHistory = new ChatHistory(this,layout);
    mTargetUin = 3788407;
    mTargetUin = 2577961;
    //mTargetUin = 1067760;
    mpTextArea = new Wt::WTextArea();
    layout->addWidget(mpTextArea);
    mpTextArea->setStyleClass("DialogWindowTextArea");

    mpTextAreaEnterSignal = new Wt::JSignal<std::string>(this,"TextAreaEnterSignal");
    initOnKeyUpJSTextArea();
    mpTextAreaEnterSignal->connect(this,&DialogWindow::onTextAreaEnterPress);

    mpTextArea->keyWentUp().connect(
                [this](Wt::WKeyEvent &keyEvent)
    {
        if(keyEvent.key() == Wt::Key_Enter)
        {
            //onSendRequest();
        }
    });


    mpSendMessageButton = new Wt::WPushButton("Send Message To Da Guc");
    layout->addWidget(mpSendMessageButton);
    mpSendMessageButton->clicked().connect(this,&DialogWindow::onSendButton);


    mpSendMessageSignal = new Wt::Signal<unsigned int, std::string>(this);


    setLayout(layout);
}
void DialogWindow::initOnKeyUpJSTextArea()
{
    std::stringstream ss;
    ss << "$(\"#" << mpTextArea->id() << "\").keydown(function(e) {";
    ss << "if( e.which == 13 )    {";
    ss << " var textArea = $(e.target);";
    ss << " var text =  textArea.val();";
    ss << " if(!ctrlPressed){";

    ss << " textArea.val(\"\");";
    ss << " if(text.length > 0)" << mpTextAreaEnterSignal->createCall("text");
    ss << " return false;";
    ss << "}";
    ss << "else textArea.val(text +'\\n');";
    ss << "}});";
    doJavaScript(ss.str());

}

DialogWindow::~DialogWindow()
{
    delete mpChatHistory;
}

void DialogWindow::onSendButton()
{
    if(mpTextArea->text().empty())
        return;
    auto narrowed = mpTextArea->text().narrow();
    mpSendMessageSignal->emit(mTargetUin,narrowed);
    mpChatHistory->addSentMessage(narrowed);
    mpTextArea->setText("");

}
void DialogWindow::onTextAreaEnterPress(std::string content)
{
    mpSendMessageSignal->emit(mTargetUin,content);
    mpChatHistory->addSentMessage(content);
}

void DialogWindow::messageReceived(MessageEvent *ev)
{
    Logger::log(std::string("Received: ") + ev->content);
    mpChatHistory->addRecvMessage(ev->fromUin,ev->content);
}


 Wt::Signal<unsigned int,std::string> &DialogWindow::sendMessageRequest()
{
    return *mpSendMessageSignal;
}
