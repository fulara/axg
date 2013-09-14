#ifndef GGTYPINGEVENT_H
#define GGTYPINGEVENT_H

struct ggTypingEvent
{
    ggTypingEvent(unsigned int recipient, int length)
        : recipientUin(recipient),
          length(length)
    {

    }

    unsigned int recipientUin;
    int length;
};

#endif // GGTYPINGEVENT_H
