#ifndef GGMESSAGEEVENT_H
#define GGMESSAGEEVENT_H

#include <string>
class ggMessageEvent
{
public:
    ggMessageEvent(unsigned int uin, const std::string msg) :
        uin(uin), msg(msg)
    {

    }

    unsigned int uin;
    std::string msg;
};


#endif // GGMESSAGEEVENT_H
