#ifndef CHATHISTORY_H
#define CHATHISTORY_H


#include "WtForwards.h"
#include <string>
#include <Wt/WJavaScript>
class ChatHistory
{
public:
    ChatHistory(Wt::WContainerWidget *parent);

    void addSentMessage(std::string&);
    void addRecvMessage(unsigned int fromUin,std::string&);
private:
    Wt::WTable *mpTable;
    Wt::WContainerWidget *mpParent;
    Wt::WScrollArea *mpScrollArea;

};

#endif // CHATHISTORY_H
