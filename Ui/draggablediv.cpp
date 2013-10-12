#include "draggablediv.h"
DraggableDiv::DraggableDiv(Wt::WContainerWidget *parent)
    : Wt::WContainerWidget(parent)
{
    setStyleClass("DraggableDiv");

    std::stringstream ss;
    ss << "$('#" << id() << "').attr('parentId','#" << parent->id() <<"');";
    setJavaScriptMember("setParentId", ss.str());
}

