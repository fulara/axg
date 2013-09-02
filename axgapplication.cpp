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


#include "axgapplication.h"
#include "logger.h"
#include "ggwrapper.h"
#include "loginresultevent.h"
#include "Ui/loginwindow.h"
#include "alivechecker.h"
#include "Ui/dialogwindow.h"
#include "messageevent.h"

using namespace Wt;

AxgApplication::AxgApplication(const Wt::WEnvironment& env)
  : WApplication(env),
    mpWrapper(new GGWrapper()),
    mpLoginWindow(new LoginWindow(root())),
    mpAliveChecker(new AliveChecker(root())),
    mpWindowUnloadSignal(new Wt::JSignal<void>(this,"WindowUnloadSignal"))
{
    setTitle("AxG");
    initConnections();
    initJSScripts();






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
}


void AxgApplication::initConnections()
{
    mpWindowUnloadSignal->connect(this,&AxgApplication::onWindowUnload);
    mpWrapper->eventSignal().connect(this,&AxgApplication::onEvent);
    mpLoginWindow->loginSignal().connect(boost::bind(&GGWrapper::connect,mpWrapper,_1,_2));
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
    Logger::log("Got Event WorkEr Thread");
    Wt::WServer::instance()->post(sessionId(),
                                  boost::bind<void>(
                                      [this,event]()
    {
        onEventUIThread(event);

    }));
}
void AxgApplication::onEventUIThread(boost::shared_ptr<Event> event)
{
    switch(event->type)
    {
        case Event::LoginResult:
            onLoginResult(event);
            break;
        case Event::MessageRcv:
            onMessageRcv(event);
            break;
    }
}
void AxgApplication::onMessageRcv(boost::shared_ptr<Event> event)
{
    MessageEvent *msgEvent = static_cast<MessageEvent*>(event.get());
    this->mpDialogWindow->messageReceived(msgEvent);
}

void AxgApplication::onLoginResult(boost::shared_ptr<Event> event)
{
    LoginResultEvent* loginResultEvent = static_cast<LoginResultEvent*>(event.get());
    if(loginResultEvent->wasLoginSuccesfull)
    {
        root()->removeWidget(mpLoginWindow);
        mpDialogWindow = new DialogWindow(this->root());
        mpDialogWindow->sendMessageRequest().connect(boost::bind(&GGWrapper::sendMessage,mpWrapper,_1,_2));
    }
    else
        doJavaScript("alert(\"Failed Login attempt NYGGA\");");
}
