#pragma once

#include "WtForwards.h"
#include "Wt/WApplication"

class GGWrapper;

class AxgApplication : public Wt::WApplication
{
public:
    AxgApplication(const Wt::WEnvironment& env);
    ~AxgApplication();
private:

    GGWrapper *mpWrapper;
    Wt::WLineEdit *mpUinEdit;
    Wt::WLineEdit *mpPassEdit;
    Wt::WPushButton *mpLoginButton;
    void initJSScripts();
    void initConnections();
    Wt::JSignal<void> *mpWindowUnloadSignal;
/////////////////////////////////////////////
///// SLOTS
/////////////////////////////////////////////
    void onWindowUnload();
    void onLoginResult(bool status);

};
