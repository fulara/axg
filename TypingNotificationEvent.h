#ifndef TYPINGNOTIFICATIONEVENT_H
#define TYPINGNOTIFICATIONEVENT_H
#include "event.h"
class TypingNotificationEvent : public Event
{
public:
    TypingNotificationEvent(unsigned int fromUin,int length)
        : fromUin(fromUin),
          length(length)
    {
    }

    EventType getType()
    {
        return TypingNotification;
    }

    unsigned int fromUin;
    int length;
};

#endif // TYPINGNOTIFICATIONEVENT_H
