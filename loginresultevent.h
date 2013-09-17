#ifndef LOGINRESULTEVENT_H
#define LOGINRESULTEVENT_H

#include <event.h>
class LoginResultEvent : public Event
{
public:
    LoginResultEvent(bool state, const unsigned int uin = 0) :
        wasLoginSuccesfull(state),
        uin(uin)
    {

    }

    bool wasLoginSuccesfull;
    unsigned int uin;
    EventType getType()
    {
        return LoginResult;
    }
};

#endif // LOGINRESULTEVENT_H
