#ifndef EVENT_H
#define EVENT_H

class Event
{
public:
    enum EventType { LoginResult,MessageRcv,ContactImport};
    //fake arg because of amiguity ..
    Event(EventType type,bool fakeArg) : type(type)
    {

    }
    Event(const EventType&) = delete;
    Event& operator=(const EventType&) = delete;

    EventType type;
};

#endif // EVENT_H
