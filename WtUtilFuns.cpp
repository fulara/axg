#include "Wt/WContainerWidget"
#include "WtUtilFuns.h"


namespace WtUtilFuns
{
void removeChildWidgets(Wt::WContainerWidget *container)
{
    while(container->count())
        container->removeWidget(container->widget(0));
}
}
