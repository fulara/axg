#ifndef GGEVENT_H
#define GGEVENT_H

#include <boost/any.hpp>

struct ggEvent
{
    enum ggEventType { LoginEvent };
public:
    ggEvent(ggEventType eventType, boost::any content)
        : eventType(eventType),
          content(content)
    {

    }


    ggEventType eventType;
    boost::any content;
};

#endif // GGEVENT_H
