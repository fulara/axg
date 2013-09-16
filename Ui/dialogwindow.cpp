#include <Wt/WText>
#include <Wt/WTable>
#include <Wt/WLineEdit>
#include <Wt/WTextEdit>
#include <Wt/WBreak>
#include <Wt/WPushButton>
#include <Wt/WLabel>
#include <Wt/WVBoxLayout>
#include <Wt/WMenuItem>
#include <stdlib.h>

#include <boost/algorithm/string.hpp>

#include "dialogwindow.h"
#include "chathistory.h"
#include "messageevent.h"
#include "logger.h"
#include "TypingNotificationEvent.h"

DialogWindow::DialogWindow(unsigned int targetUin, std::string targetName, Wt::WContainerWidget *parent)
    :
      Wt::WContainerWidget(parent),
      mpMenuItem(NULL)

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

    mpTextAreaEnterSignal->connect(this,&DialogWindow::onTextAreaEnterPress);



    mpSendMessageButton = new Wt::WPushButton("Send Message To Da Guc");
    addWidget(mpSendMessageButton);
    mpSendMessageButton->clicked().connect(this,&DialogWindow::onSendButton);


    mpSendMessageSignal = new Wt::Signal<unsigned int, std::string>(this);
    mpSendTypingNotificationSignal = new Wt::Signal<unsigned int, int>(this);
    mpTextLengthUpdateSignal = new Wt::JSignal<int>(this,"TextLengthUpdateSignal");
    mpTextLengthUpdateSignal->connect(this,&DialogWindow::onTextLengthUpdate);
    initOnKeyUpJSTextArea();

    focusOnTextArea();
}
void DialogWindow::initOnKeyUpJSTextArea()
{
    std::stringstream ss;
    ss << "$(\"#" << mpTextArea->id() << "\").keydown(function(e) {";
    ss << " var textArea = $(e.target);";
    ss << " var text =  textArea.val();";
    ss << "if( e.which == 13 )    {";
    ss << " if(!ctrlPressed){";
    ss << R"(text=text.replace(/^\n+|\s+$/g,'');)";
    ss << " textArea.val(\"\");";
    ss << " if(text.length > 0)" << mpTextAreaEnterSignal->createCall("text");
    ss << " return false;";
    ss << "}";
    ss << "else textArea.val(text +'\\n');";
    ss << "}";
    ss << "else {";
    ss << "if( e.which == 9){ e.preventDefault();textArea.val(text + '\\t');} /*check if ctrl pressed .. */";
    ss << "";
    ss << "}});";
    ss << "$(\"#" << mpTextArea->id() << "\").keyup(function(){";
    ss << mpTextLengthUpdateSignal->createCall("$(this).val().length");
    ss << "});";
    setJavaScriptMember("onEnterPress",ss.str());

}
void DialogWindow::onTextLengthUpdate(int length)
{
    mpSendTypingNotificationSignal->emit(mTargetUin,length);
}

DialogWindow::~DialogWindow()
{
    delete mpChatHistory;
}
void DialogWindow::focusOnTextArea()
{
    mpTextArea->setFocus();
    std::stringstream ss;
    ss << "$(\"#" << mpTextArea->id() << "\").focus();";
    doJavaScript(ss.str());
}

void DialogWindow::onSendButton()
{
    if(mpTextArea->text().empty())
        return;
    auto narrowed = mpTextArea->text().toUTF8();
    mpSendMessageSignal->emit(mTargetUin,narrowed);
    mpChatHistory->addSentMessage(narrowed);
    mpTextArea->setText("");

}
void DialogWindow::onTextAreaEnterPress(std::string content)
{
    Logger::log(content);
    mpSendMessageSignal->emit(mTargetUin,content);
    mpChatHistory->addSentMessage(content);
}

void DialogWindow::messageReceived(MessageEvent *ev)
{
    Logger::log(std::string("Received: ") + ev->content);
    mpChatHistory->addRecvMessage(ev->fromUin,ev->content);
}
void DialogWindow::handleTypingNotificationEvent(TypingNotificationEvent *ev)
{
    if(mpMenuItem == NULL)
        return;
    const std::string typingStyleClass("DialogWindowMenuItemTyping");
    const std::string gotMsgStyleClass("DialogWindowMenuItemGotMsg");
    if(ev->length == 0)
    {
        mpMenuItem->removeStyleClass(typingStyleClass);
        return;
    }
    if(!mpMenuItem->hasStyleClass(typingStyleClass))
    {
        mpMenuItem->addStyleClass(typingStyleClass);
        return;
    }
    if(!mpMenuItem->hasStyleClass(gotMsgStyleClass))
    {
        return;
    }

    std::string styleClasses = mpMenuItem->styleClass().narrow();
    std::vector<std::string> styleClassesSplit;
    boost::split(styleClassesSplit,styleClasses, boost::is_any_of(" "));
    assert(styleClassesSplit.size() == 4);
    if(styleClassesSplit.at(2) == typingStyleClass)
    {
        mpMenuItem->removeStyleClass(typingStyleClass);
        mpMenuItem->addStyleClass(typingStyleClass);
    }


}

void DialogWindow::updateMenuItem(Wt::WMenuItem *newMenuItem)
{
    mpMenuItem = newMenuItem;
}

Wt::Signal<unsigned int,std::string> &DialogWindow::sendMessageRequest()
{
    return *mpSendMessageSignal;
}
Wt::Signal<unsigned int, int> &DialogWindow::sendTypingNotificationRequest()
{
    return *mpSendTypingNotificationSignal;
}
