#include "Wt/WEnvironment"
#include "Wt/WText"
#include "Wt/WLineEdit"
#include "Wt/WContainerWidget"
#include "Wt/WPushButton"
#include "Wt/WJavaScript"
#include "Wt/WMessageBox"
#include "Wt/WLogger"
#include "Wt/WServer"
#include <boost/lexical_cast.hpp>
#include <boost/functional.hpp>
#include <sstream>


#include "Ui/dialogwindowholder.h"
#include "axgapplication.h"
#include "logger.h"
#include "ggwrapper.h"
#include "loginresultevent.h"
#include "Ui/loginwindow.h"
#include "alivechecker.h"
#include "Ui/dialogwindow.h"
#include "messageevent.h"
#include "ContactImportEvent.h"
#include "Ui/contactlist.h"
#include "Ui/contactwindow.h"

using namespace Wt;


#define LAYOUT_TEST1
AxgApplication::AxgApplication(const Wt::WEnvironment& env)
  : WApplication(env),
    mpWrapper(new GGWrapper()),
    #ifndef LAYOUT_TEST
    mpLoginWindow(new LoginWindow(root())),
    mpDialogWindowHolder(new DialogWindowHolder(root())),
    #else
    mpDialogWindowHolder(new DialogWindowHolder(root())),
    mpContactWindow(new ContactWindow(root())),
    #endif

    mpAliveChecker(new AliveChecker(root())),

    mpWindowUnloadSignal(new Wt::JSignal<void>(this,"WindowUnloadSignal"))
{
    enableUpdates();
    setTitle("AxG");
    initConnections();
    initJSScripts();

#ifdef LAYOUT_TEST
    mpContactWindow->windowOpenRequest().connect(mpDialogWindowHolder,&DialogWindowHolder::openDialogWindowRequest);
#endif


    this->useStyleSheet("style/style.css");
}
AxgApplication::~AxgApplication()
{
    delete mpWindowUnloadSignal;
    delete mpWrapper;
}

void AxgApplication::initJSScripts()
{
    std::stringstream ss;

    ss << "window.onbeforeunload =function(){" <<mpWindowUnloadSignal->createCall() << "};";
    doJavaScript(ss.str());

    require("./scripts/globals.js");
}


void AxgApplication::initConnections()
{
    mpWindowUnloadSignal->connect(this,&AxgApplication::onWindowUnload);
    mpWrapper->eventSignal().connect(this,&AxgApplication::onEvent);
    #ifndef LAYOUT_TEST
    mpLoginWindow->loginSignal().connect(boost::bind(&GGWrapper::connect,mpWrapper,_1,_2));
    #endif
    mpAliveChecker->died().connect(this,&AxgApplication::onQuitRequested);

}


void AxgApplication::onWindowUnload()
{
    //Logger::log("Unload called \n");
    //quit();

}
void AxgApplication::onQuitRequested()
{
    Logger::log("Quit Requested.. pushing onto UI");
    Wt::WServer::instance()->post(sessionId(),boost::bind(&AxgApplication::quit,this));
}

//called on wrong thread has to be pushed to UI.
void AxgApplication::onEvent(boost::shared_ptr<Event> event)
{
    //push Event To UI
    Logger::log("Got Event on Worker Thread");
    Wt::WServer::instance()->post(sessionId(),
                                  boost::bind<void>(
                                      [this,event]()
    {
        onEventUIThread(event);

    }));

}
void AxgApplication::onEventUIThread(boost::shared_ptr<Event> event)
{
    //auto lock = UpdateLock(this);
    switch(event->getType())
    {
        case Event::LoginResult:
            onLoginResult(event);
            break;
        case Event::MessageRcv:
            onMessageRcv(event);
            break;
        case Event::ContactImport:
            onContactImport(event);
            break;
    }
    triggerUpdate();
}
void AxgApplication::onContactImport(boost::shared_ptr<Event> event)
{
    ContactImportEvent *contactsEvent = static_cast<ContactImportEvent*>(event.get());
    this->mpContactWindow->contactsReceived(contactsEvent);
}

void AxgApplication::onMessageRcv(boost::shared_ptr<Event> event)
{
    MessageEvent *msgEvent = static_cast<MessageEvent*>(event.get());
    this->mpDialogWindowHolder->messageReceived(msgEvent);
}

void AxgApplication::onLoginResult(boost::shared_ptr<Event> event)
{
    LoginResultEvent* loginResultEvent = static_cast<LoginResultEvent*>(event.get());
    if(loginResultEvent->wasLoginSuccesfull)
    {
        root()->removeWidget(mpLoginWindow);
        mpDialogWindowHolder = new DialogWindowHolder(this->root());
        mpContactWindow = new ContactWindow(root());
        mpContactWindow->windowOpenRequest().connect(mpDialogWindowHolder,&DialogWindowHolder::openDialogWindowRequest);
        mpContactWindow->windowOpenRequestForceActivate().connect(mpDialogWindowHolder,&DialogWindowHolder::openDialogWindowAndActivateRequest);
        mpDialogWindowHolder->newContactInfoRequest().connect(mpContactWindow,&ContactWindow::onNewContactInfoRequest);
        mpDialogWindowHolder->sendMessageRequest().connect(mpWrapper,&GGWrapper::sendMessage);

    }
    else
        doJavaScript("alert(\"Failed Login attempt NYGGA\");");
}
