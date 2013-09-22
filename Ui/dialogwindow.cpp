#include <Wt/WText>
#include <Wt/WTable>
#include <Wt/WLineEdit>
#include <Wt/WTextEdit>
#include <Wt/WBreak>
#include <Wt/WPushButton>
#include <Wt/WLabel>
#include <Wt/WVBoxLayout>
#include <Wt/WMenuItem>
#include <Wt/WTimer>
#include <stdlib.h>

#include <boost/algorithm/string.hpp>

#include "dialogwindow.h"
#include "chathistory.h"
#include "messageevent.h"
#include "logger.h"
#include "TypingNotificationEvent.h"
#include "menuitemupdater.h"
#include "History/historymanager.h"
#include "contactinfo.h"
DialogWindow::DialogWindow(unsigned int userUin, unsigned int targetUin, std::string targetName, Wt::WContainerWidget *parent)
    :
      Wt::WContainerWidget(parent),
      mIsActive(false),
      mUserUin(userUin),
      mUnreadMsgCount(0),
      mTargetUin(targetUin),
      mTargetName(targetName),
      mpMenuItemUpdater(new MenuItemUpdater(mTargetName,this))

{
    setStyleClass("DialogWindow");

    initWidgets();

    initAndConnectSignals();
    initOnKeyUpJSTextArea();

    focusOnTextArea();
}
DialogWindow::~DialogWindow()
{
    delete mpChatHistory;
    delete mpMenuItemUpdater;
}

void DialogWindow::initWidgets()
{
    mpTargetInfo = new Wt::WLabel(mTargetName,this);
    mpTargetInfo->setStyleClass("DialogWindowTargetInfo");
    addWidget(mpTargetInfo);
    mpChatHistory = new ChatHistory(mUserUin,mTargetUin,mTargetName,this);

    mpTextArea = new Wt::WTextArea();
    addWidget(mpTextArea);
    mpTextArea->setStyleClass("DialogWindowTextArea");
    mpSendMessageButton = new Wt::WPushButton("Send Message To Da Guc");
    addWidget(mpSendMessageButton);
}

void DialogWindow::initAndConnectSignals()
{
    mpTextAreaEnterSignal = new Wt::JSignal<std::string>(this,"TextAreaEnterSignal");
    mpTextAreaEnterSignal->connect(this,&DialogWindow::onTextAreaEnterPress);
    mpSendMessageButton->clicked().connect(this,&DialogWindow::onSendButton);
    mpSendMessageSignal = new Wt::Signal<unsigned int, std::string>(this);
    mpSendTypingNotificationSignal = new Wt::Signal<unsigned int, int>(this);
    mpNewUnreadMessageSignal = new Wt::Signal<ContactInfo, unsigned int>(this);
    mpMessagesReadSignal = new Wt::Signal<unsigned int>(this);
    mpTextLengthUpdateSignal = new Wt::JSignal<int>(this,"TextLengthUpdateSignal");
    mpTextLengthUpdateSignal->connect(this,&DialogWindow::onTextLengthUpdate);
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


void DialogWindow::deactivated()
{

    mIsActive = false;
    mpMenuItemUpdater->parentLostFocus();
}
void DialogWindow::activated()
{
    mIsActive = true;
    mpMenuItemUpdater->parentGainedFocus();
    if(mUnreadMsgCount)
        mpMessagesReadSignal->emit(mTargetUin);
    mUnreadMsgCount = 0;

}

void DialogWindow::focusOnTextArea()
{
    mpTextArea->setFocus();
    std::stringstream ss;
    ss << "$(\"#" << mpTextArea->id() << "\").focus();";
    doJavaScript(ss.str());
}

void DialogWindow::onSendRequest(std::string toSend)
{
    mpSendMessageSignal->emit(mTargetUin,toSend);
    mpChatHistory->addSentMessage(toSend);

    HistoryManager::saveSendEntry(toSend,mUserUin,mTargetUin);
}

void DialogWindow::onSendButton()
{
    if(mpTextArea->text().empty())
        return;
    auto narrowed = mpTextArea->text().toUTF8();
    onSendRequest(narrowed);
    mpTextArea->setText("");

}
void DialogWindow::onTextAreaEnterPress(std::string content)
{
    onSendRequest(content);
}

void DialogWindow::messageReceived(MessageEvent *ev)
{
    HistoryManager::saveRcvEntry(ev->content, mUserUin, ev->fromUin);
    mpChatHistory->addRecvMessage(ev->fromUin,ev->content,ev->time);

    if(!mIsActive)
    {
        ++mUnreadMsgCount;
        mpNewUnreadMessageSignal->emit(ContactInfo(mTargetUin,mTargetName,"",false,false,false),mUnreadMsgCount);
        mpMenuItemUpdater->onMessageRcv(mUnreadMsgCount);
    }
    else
    {
        mpNewUnreadMessageSignal->emit(ContactInfo(mTargetUin,mTargetName,"",false,false,false),0);
    }
}
void DialogWindow::handleTypingNotificationEvent(TypingNotificationEvent *ev)
{
    mpMenuItemUpdater->onNotificationEvent(ev);
}


void DialogWindow::updateMenuItem(Wt::WMenuItem *newMenuItem)
{
    mpMenuItemUpdater->updateMenuItemReference(newMenuItem);
}

Wt::Signal<unsigned int,std::string> &DialogWindow::sendMessageRequest()
{
    return *mpSendMessageSignal;
}
Wt::Signal<unsigned int, int> &DialogWindow::sendTypingNotificationRequest()
{
    return *mpSendTypingNotificationSignal;
}
Wt::Signal<ContactInfo,unsigned int> &DialogWindow::newUnreadMessage()
{
    return *mpNewUnreadMessageSignal;
}
Wt::Signal<unsigned int> &DialogWindow::messagesRead()
{
    return *mpMessagesReadSignal;
}
