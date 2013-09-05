#ifndef CONTACTGROUP_H
#define CONTACTGROUP_H
#include <string>
#include <vector>
#include "contactinfo.h"
struct ContactGroup
{
    ContactGroup(bool isExpanded, bool isRemovable, std::string name, std::vector<ContactInfo>& contacts)
        : isExpanded(isExpanded), isRemovable(isRemovable), name(name), contacts(contacts)
    {

    }
    ContactGroup(const ContactGroup &other)
        : isExpanded(other.isExpanded), isRemovable(other.isRemovable), name(other.name), contacts(other.contacts)
    {

    }

    bool isExpanded;
    bool isRemovable;
    std::string name;
    std::vector<ContactInfo> contacts;
};

#endif // CONTACTGROUP_H
