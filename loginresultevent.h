#ifndef LOGINRESULTEVENT_H
#define LOGINRESULTEVENT_H

#include <event.h>
class LoginResultEvent : public Event
{
public:
    LoginResultEvent(bool state) :
        Event(EventType::LoginResult,0),
        wasLoginSuccesfull(state)
    {

    }

    bool wasLoginSuccesfull;
};

#endif // LOGINRESULTEVENT_H
