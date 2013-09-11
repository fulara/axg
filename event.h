#ifndef EVENT_H
#define EVENT_H

class Event
{
public:
    enum EventType { LoginResult,MessageRcv,ContactImport};
    Event(){}
    Event(const EventType&) = delete;
    Event& operator=(const EventType&) = delete;

    virtual EventType getType() = 0;
};

#endif // EVENT_H
