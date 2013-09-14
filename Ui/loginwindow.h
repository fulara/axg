#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H


#include "WtForwards.h"
#include <Wt/WContainerWidget>
#include <Wt/WSignal>
class LoginWindow : public Wt::WContainerWidget
{
public:
    LoginWindow(Wt::WContainerWidget *parent = 0);
    ~LoginWindow();
    Wt::Signal<unsigned int,std::string> &loginSignal();

    void reset();
private:
    Wt::WLineEdit *mpUinEdit;
    Wt::WLineEdit *mpPassEdit;
    Wt::WPushButton *mpLoginButton;
    Wt::Signal<unsigned int, std::string> *mpLoginSignal;

    void onLoginButton();
};

#endif // LOGINWINDOW_H
