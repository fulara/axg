#ifndef DRAGGABLEDIV_H
#define DRAGGABLEDIV_H

#include "WtForwards.h"

#include "Wt/WContainerWidget"
class DraggableDiv : public Wt::WContainerWidget
{
public:
    DraggableDiv(Wt::WContainerWidget *parent = 0);
};

#endif // DRAGGABLEDIV_H
