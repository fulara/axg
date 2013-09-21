#ifndef CONTACTGROUP_H
#define CONTACTGROUP_H
#include <string>
#include <set>
#include "contactinfo.h"
struct ContactGroup
{
    ContactGroup(bool isExpanded, bool isRemovable, std::string name, std::multiset<ContactInfo>& contacts)
        : isExpanded(isExpanded), isRemovable(isRemovable), name(name), contacts(contacts)
    {

    }
    ContactGroup(const ContactGroup &other)
        : isExpanded(other.isExpanded), isRemovable(other.isRemovable), name(other.name), contacts(other.contacts)
    {

    }
    bool operator<(const ContactGroup &other) const
    {
        return name < other.name;
    }
    bool operator==(const ContactGroup &other) const
    {
        return name == other.name;
    }

    bool isExpanded;
    bool isRemovable;
    std::string name;
    std::multiset<ContactInfo> contacts;
};

#endif // CONTACTGROUP_H
