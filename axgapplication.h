#pragma once

class GGWrapper;
class LoginWindow;
class AliveChecker;
#include "WtForwards.h"
#include "Wt/WApplication"

#include <boost/shared_ptr.hpp>

#include "event.h"



class AxgApplication : public Wt::WApplication
{
public:
    AxgApplication(const Wt::WEnvironment& env);
    ~AxgApplication();
private:

    GGWrapper *mpWrapper;
    LoginWindow *mpLoginWindow;
    AliveChecker *mpAliveChecker;

    void initJSScripts();
    void initConnections();
    Wt::JSignal<void> *mpWindowUnloadSignal;
/////////////////////////////////////////////
///// SLOTS
/////////////////////////////////////////////
    void onWindowUnload();
    void onQuitRequested();
    void onEvent(boost::shared_ptr<Event>);
    void onEventUIThread(boost::shared_ptr<Event>);
    void onLoginResult(boost::shared_ptr<Event> event);


};
