#ifndef CONTACTIMPORTEVENT_H
#define CONTACTIMPORTEVENT_H

#include <list>
#include "ContactGroup.h"
#include "event.h"
class ContactImportEvent : public Event
{
public:
    ContactImportEvent(std::list<ContactGroup> &contactGroups)
        :
          contactGroups(contactGroups)
    {

    }

    const std::list<ContactGroup> contactGroups;

    EventType getType()
    {
        return ContactImport;
    }
};

#endif // CONTACTIMPORTEVENT_H
