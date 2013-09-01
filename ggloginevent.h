#ifndef GGLOGINEVENT_H
#define GGLOGINEVENT_H

#include <string>
#include "ggevent.h"
class ggLoginEvent
{
public:
    ggLoginEvent(unsigned int uin, const std::string& pass) :
        uin(uin), pass(pass)
    {

    }

    unsigned int uin;
    std::string pass;

};

#endif // GGLOGINEVENT_H
