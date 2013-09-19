#ifndef PARSEDHISTORYENTRY_H
#define PARSEDHISTORYENTRY_H

#include <boost/date_time/posix_time/ptime.hpp>

struct ParsedHistoryEntry
{
public:
    ParsedHistoryEntry(bool isReceived, const boost::posix_time::ptime &time, const std::string &content)
        : isReceived(isReceived),
          time(time),
          content(content)
    {

    }

    const bool isReceived;
    const boost::posix_time::ptime time;
    const std::string content;
};

#endif // PARSEDHISTORYENTRY_H
