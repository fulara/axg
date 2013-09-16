#pragma once


class GGWrapper;
class LoginWindow;
class AliveChecker;
class DialogWindowHolder;
class DialogWindow;
class ContactList;
class ContactWindow;
#include "WtForwards.h"
#include "Wt/WApplication"

#include <boost/shared_ptr.hpp>

#include "event.h"


class AxgApplication : public Wt::WApplication
{
public:
    AxgApplication(const Wt::WEnvironment& env);
    ~AxgApplication();

    void finalize();
private:

    GGWrapper *mpWrapper;
    LoginWindow *mpLoginWindow;
    DialogWindowHolder *mpDialogWindowHolder;
    ContactWindow *mpContactWindow;
    AliveChecker *mpAliveChecker;
    Wt::JSignal<void> *mpWindowUnloadSignal;
    void initJSScripts();
    void initConnections();
    static std::clock_t begin;

/////////////////////////////////////////////
///// SLOTS
/////////////////////////////////////////////
    void onWindowUnload();
    void onQuitRequested();
    void onEvent(boost::shared_ptr<Event>);
    void onEventUIThread(boost::shared_ptr<Event>);
    void onLoginResult(boost::shared_ptr<Event> event);
    void onMessageRcv(boost::shared_ptr<Event> event);
    void onContactImport(boost::shared_ptr<Event> event);
    void onTypingNotification(boost::shared_ptr<Event> event);


};
