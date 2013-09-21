#ifndef CONTACTINFO_H
#define CONTACTINFO_H

#include <map>
#include <string>
#include <boost/algorithm/string.hpp>

struct ContactInfo
{
public:
    ContactInfo(unsigned int uin, const std::string &showName, const std::string nickName, const bool isBuddy, const bool isNormal, const bool isFriend)
        : uin(uin), showName(showName), nickName(nickName), isBuddy(isBuddy), isNormal(isNormal), isFriend(isFriend)
    {

    }
    std::string getDisplayName() const
    {
        if(showName.empty())
        {
            if(nickName.empty())
            {
                return boost::lexical_cast<std::string>(uin);
            }
            else
            {
                return nickName;
            }
        }
        return showName;
    }
    bool operator==(const ContactInfo& other) const
    {
        return uin == other.uin;
    }
    bool operator<(const ContactInfo& other) const
    {
        std::string c1 = boost::algorithm::to_lower_copy(getDisplayName());
        std::string c2 = boost::algorithm::to_lower_copy(other.getDisplayName());
        return c1 < c2;
    }

    unsigned int uin;
    std::string showName;
    std::string nickName;
    bool isBuddy;
    bool isNormal;
    bool isFriend;

};

#endif // CONTACTINFO_H
