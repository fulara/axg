#ifndef MESSAGEEVENT_H
#define MESSAGEEVENT_H
#include <string>
#include "event.h"
class MessageEvent : public Event
{
public:
    MessageEvent(unsigned int fromUin, std::string& content) :
        fromUin(fromUin),
        content(content)
    {

    }

    unsigned int fromUin;
    std::string content;
    EventType getType()
    {
        return MessageRcv;
    }
};

#endif // MESSAGEEVENT_H
