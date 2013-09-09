#include "dialogwindow.h"

#include <Wt/WText>
#include <Wt/WTable>
#include <Wt/WLineEdit>
#include <Wt/WTextEdit>
#include <Wt/WBreak>
#include <Wt/WPushButton>
#include <Wt/WLabel>
#include <Wt/WVBoxLayout>
#include <stdlib.h>
#include "chathistory.h"
#include "messageevent.h"
#include "logger.h"

DialogWindow::DialogWindow(unsigned int targetUin, std::string targetName, Wt::WContainerWidget *parent)
    :
      Wt::WContainerWidget(parent)

{
    setStyleClass("DialogWindow");
    //Wt::WTable *table = new Wt::WTable();
    mpTargetInfo = new Wt::WLabel(targetName,this);
    mpTargetInfo->setStyleClass("DialogWindowTargetInfo");
    addWidget(mpTargetInfo);
    mpChatHistory = new ChatHistory(this);
    mTargetUin = targetUin;
    mTargetName = targetName;
    mpTextArea = new Wt::WTextArea();
    addWidget(mpTextArea);
    mpTextArea->setStyleClass("DialogWindowTextArea");

    mpTextAreaEnterSignal = new Wt::JSignal<std::string>(this,"TextAreaEnterSignal");
    initOnKeyUpJSTextArea();
    mpTextAreaEnterSignal->connect(this,&DialogWindow::onTextAreaEnterPress);



    mpSendMessageButton = new Wt::WPushButton("Send Message To Da Guc");
    addWidget(mpSendMessageButton);
    mpSendMessageButton->clicked().connect(this,&DialogWindow::onSendButton);


    mpSendMessageSignal = new Wt::Signal<unsigned int, std::string>(this);


}
void DialogWindow::initOnKeyUpJSTextArea()
{
    std::stringstream ss;
    ss << "$(\"#" << mpTextArea->id() << "\").keydown(function(e) {";
    ss << "if( e.which == 13 )    {";
    ss << " var textArea = $(e.target);";
    ss << " var text =  textArea.val();";
    ss << " if(!ctrlPressed){";
    ss << R"(text=text.replace(/^\n+|\s+$/g,'');)";
    ss << " textArea.val(\"\");";
    ss << " if(text.length > 0)" << mpTextAreaEnterSignal->createCall("text");
    ss << " return false;";
    ss << "}";
    ss << "else textArea.val(text +'\\n');";
    ss << "}});";
    doJavaScript(ss.str());

}
void DialogWindow::reinitJS()
{
    initOnKeyUpJSTextArea();
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
