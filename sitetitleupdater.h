#ifndef SITETITLEUPDATER_H
#define SITETITLEUPDATER_H
#include <WtForwards.h>
struct ContactInfo;
#include <vector>
#include <string>
#include <Wt/WObject>
#include <Wt/WSignal>

typedef std::pair<ContactInfo, int> CIIntPair;
class SiteTitleUpdater : public Wt::WObject
{
public:
    SiteTitleUpdater(Wt::WObject *parent);

    Wt::Signal<std::string> &titleUpdateRequest();

    void messagesRead(unsigned int uin);
    void newUnreadMessage(ContactInfo ci,unsigned int messageCount);
private:
    unsigned int mCurrentIndex;
    Wt::Signal<std::string> *mpTitleUpdateRequest;
    Wt::WTimer *mpTimer;
    std::vector<CIIntPair> mUpdateClients;

    void onTimeout();

};

#endif // SITETITLEUPDATER_H
