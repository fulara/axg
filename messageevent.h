#ifndef MESSAGEEVENT_H
#define MESSAGEEVENT_H
#include <string>
#include <boost/date_time/posix_time/ptime.hpp>
#include "event.h"
class MessageEvent : public Event
{
public:
    MessageEvent(unsigned int fromUin, std::string& content,const boost::posix_time::ptime& time) :
        fromUin(fromUin),
        content(content),
        time(time)
    {

    }
    virtual ~MessageEvent(){}
    unsigned int fromUin;
    std::string content;
    boost::posix_time::ptime time;
    EventType getType()
    {
        return MessageRcv;
    }
};

#endif // MESSAGEEVENT_H
