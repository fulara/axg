#ifndef CONTACTIMPORTEVENT_H
#define CONTACTIMPORTEVENT_H

#include <list>;
#include "ContactGroup.h"
#include "event.h"
class ContactImportEvent : public Event
{
public:
    ContactImportEvent(std::list<ContactGroup> &contactGroups)
        : Event(ContactImport,false),
          contactGroups(contactGroups)
    {

    }

    const std::list<ContactGroup> contactGroups;
};

#endif // CONTACTIMPORTEVENT_H
