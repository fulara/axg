#include <Wt/WPushButton>
#include <Wt/WLineEdit>
#include <Wt/WIntValidator>
#include <Wt/WBreak>
#include "loginwindow.h"



LoginWindow::LoginWindow(Wt::WContainerWidget *parent)
    : Wt::WContainerWidget(parent),
      mpLoginSignal(new Wt::Signal<unsigned int, std::string>(this))
{
    mpUinEdit = new Wt::WLineEdit(this);
    mpUinEdit->setText("48493448");
    mpUinEdit->setValidator(new Wt::WIntValidator());
    new Wt::WBreak(this);

    mpPassEdit = new Wt::WLineEdit(this);
    mpPassEdit->setText("k0;3ksiemixk0;3k");
    mpPassEdit->setEchoMode(Wt::WLineEdit::Password);
    new Wt::WBreak(this);

    mpLoginButton = new Wt::WPushButton("Log In", this);
    mpLoginButton->clicked().connect(this,&LoginWindow::onLoginButton);


    setStyleClass("LoginWindow");
}

LoginWindow::~LoginWindow()
{
    delete mpLoginSignal;
}


void LoginWindow::onLoginButton()
{

    unsigned int uin = boost::lexical_cast<int>(mpUinEdit->text());
    if(uin == 0 || mpPassEdit->text().empty())
    {
        return;
    }
    mpLoginButton->setEnabled(false);
    mpLoginSignal->emit(uin,mpPassEdit->text().narrow());

}


Wt::Signal<unsigned int,std::string> &LoginWindow::loginSignal()
{
    return *mpLoginSignal;
}
