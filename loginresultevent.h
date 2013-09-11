#ifndef LOGINRESULTEVENT_H
#define LOGINRESULTEVENT_H

#include <event.h>
class LoginResultEvent : public Event
{
public:
    LoginResultEvent(bool state) :
        wasLoginSuccesfull(state)
    {

    }

    bool wasLoginSuccesfull;

    EventType getType()
    {
        return LoginResult;
    }
};

#endif // LOGINRESULTEVENT_H
