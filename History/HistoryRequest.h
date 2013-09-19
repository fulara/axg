#ifndef HISTORYREQUEST_H
#define HISTORYREQUEST_H
#include <string>
class ChatHistory;
struct HistoryRequest
{
    HistoryRequest(ChatHistory * const requester,const unsigned int ownerUin, const unsigned int talkingWith,const std::string& owningSessionId)
        : owningSessionId(owningSessionId),
          ownerUin(ownerUin),
          talkingWith(talkingWith),
          requester(requester)

    {

    }
    const std::string owningSessionId;
    const unsigned int ownerUin;
    const unsigned int talkingWith;
    ChatHistory * const requester;
};

#endif // HISTORYREQUEST_H
