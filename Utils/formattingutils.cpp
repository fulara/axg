#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <boost/chrono.hpp>
#include "formattingutils.h"

#include "logger.h"
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

boost::posix_time::ptime parseTime(const std::string &timeAsStr)
{
    boost::posix_time::time_input_facet* facet = new boost::posix_time::time_input_facet("%Y-%m-%d %H:%M:%S");
    std::stringstream ss;
    boost::posix_time::ptime time;
    ss.imbue(std::locale(ss.getloc(), facet));
    ss.str(timeAsStr);
    ss >> time;
    return time;
}
boost::posix_time::ptime time_tToPtime(const time_t &time)
{
    boost::posix_time::ptime ptime(boost::posix_time::from_time_t(time));
    ptime += boost::posix_time::hours(2);
    return boost::move(ptime);
}
};
