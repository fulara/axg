#ifndef HISTORYPACK_H
#define HISTORYPACK_H

#include <string>
#include <boost/date_time/posix_time/ptime.hpp>
class HistoryPack
{
public:
    HistoryPack(const std::string& cont,const boost::posix_time::ptime &timestamp,const unsigned int ownerUin, const unsigned int talkingWith, const bool isSent)
        : content(cont),
          timestamp(timestamp),
          ownerUin(ownerUin),
          talkingWith(talkingWith),
          isSent(isSent)
    {

    }
    HistoryPack(const std::string& cont,const unsigned int ownerUin, const unsigned int talkingWith, const bool isSent)
        : content(cont),
          timestamp(boost::posix_time::not_a_date_time),
          ownerUin(ownerUin),
          talkingWith(talkingWith),
          isSent(isSent)
    {

    }
    const boost::posix_time::ptime timestamp;
    const std::string content;
    const unsigned int ownerUin;
    const unsigned int talkingWith;
    const bool isSent;
};

#endif // HISTORYPACK_H
