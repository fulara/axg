#include "Wt/WEnvironment"
#include "Wt/WText"
#include "Wt/WLineEdit"
#include "Wt/WContainerWidget"
#include "Wt/WPushButton"
#include "Wt/WJavaScript"
#include "Wt/WMessageBox"
#include "Wt/WLogger"
#include <boost/lexical_cast.hpp>
#include <boost/functional.hpp>
#include <sstream>


#include "axgapplication.h"
#include "logger.h"
#include "ggwrapper.h"

using namespace Wt;

AxgApplication::AxgApplication(const Wt::WEnvironment& env)
  : WApplication(env),
    mpWrapper(new GGWrapper()),
    mpWindowUnloadSignal(new Wt::JSignal<void>(this,"WindowUnloadSignal"))

{
    setTitle("AxG");
    initConnections();
    initJSScripts();
    //mpUinEdit = new Wt::WLineEdit(root());
    //mpPassEdit = new Wt::WLineEdit(root());
    //mpLoginButton = new Wt::WPushButton("Logg In", root());
    //mpLoginButton->clicked().connect(boost::bind<void>([=]() {
    //    mpWrapper->connect(boost::lexical_cast<unsigned int>(mpUinEdit->text().narrow()),mpPassEdit->text().narrow());
    //}));





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
    //ss << "window.addEventListener('resize',function(event) {" << mpWindowUnloadSignal->createCall() << "});";
    doJavaScript(ss.str());
}


void AxgApplication::initConnections()
{
    mpWindowUnloadSignal->connect(this,&AxgApplication::onWindowUnload);
    mpWrapper->loginResultSignal().connect(this,&AxgApplication::onLoginResult);
}


void AxgApplication::onWindowUnload()
{
    Logger::log("Unload called \n");
  //  quit();

}
void AxgApplication::onLoginResult(bool status)
{
    Wt::WMessageBox::show("Loggin Result",Wt::WString("Logged In?") + boost::lexical_cast<std::wstring>(status),Ok);
}
