#ifndef SITETITLEUPDATER_H
#define SITETITLEUPDATER_H
#include <WtForwards.h>
struct ContactInfo;
class AsyncUiTimer;
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

    void siteGainedFocus();
    void siteLostFocus();
    void messagesRead(unsigned int uin);
    void newUnreadMessage(ContactInfo ci,unsigned int messageCount);
private:
    bool mSiteHasFocus;
    unsigned int mCurrentIndex;
    Wt::Signal<std::string> *mpTitleUpdateRequest;
    std::vector<CIIntPair> mUpdateClients;
    AsyncUiTimer *mpTimer;

    void onTimeout();
    void clearZeroCountPairs();

};

#endif // SITETITLEUPDATER_H
