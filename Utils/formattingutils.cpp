#include <boost/date_time/posix_time/posix_time.hpp>

#include "formattingutils.h"

namespace FormattingUtils
{
std::string dateToStr(const boost::posix_time::ptime &time)
{
    boost::posix_time::time_facet* facet = new boost::posix_time::time_facet("%Y-%m-%d %H:%M:%S");
    std::stringstream ss;
    ss.imbue(std::locale(ss.getloc(), facet));
    ss << time;
    return ss.str();
}
std::string dateToTimeStr(const boost::posix_time::ptime &time)
{
    boost::posix_time::time_facet* facet = new boost::posix_time::time_facet("%H:%M:%S");
    std::stringstream ss;
    ss.imbue(std::locale(ss.getloc(), facet));
    ss << time;
    return ss.str();
}
std::string dateToDayStr(const boost::posix_time::ptime &time)
{
    boost::posix_time::time_facet* facet = new boost::posix_time::time_facet("%Y-%m-%d");
    std::stringstream ss;
    ss.imbue(std::locale(ss.getloc(), facet));
    ss << time;
    return ss.str();
}
};
