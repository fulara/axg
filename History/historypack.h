#ifndef HISTORYPACK_H
#define HISTORYPACK_H

#include <string>
class HistoryPack
{
public:
    HistoryPack(const std::string& cont,const unsigned int ownerUin, const unsigned int talkingWith, const bool isSent)
        : content(cont),
          ownerUin(ownerUin),
          talkingWith(talkingWith),
          isSent(isSent)
    {

    }

    const std::string content;
    const unsigned int ownerUin;
    const unsigned int talkingWith;
    const bool isSent;
};

#endif // HISTORYPACK_H
