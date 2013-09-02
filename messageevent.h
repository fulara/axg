#ifndef MESSAGEEVENT_H
#define MESSAGEEVENT_H
#include <string>
#include "event.h"
class MessageEvent : public Event
{
public:
    MessageEvent(unsigned int fromUin, std::string& content) :
        Event(EventType::MessageRcv,0),
        fromUin(fromUin),
        content(content)
    {

    }

    unsigned int fromUin;
    std::string content;
};

#endif // MESSAGEEVENT_H
