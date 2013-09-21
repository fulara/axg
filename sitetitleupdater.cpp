#include <Wt/WTimer>
#include <boost/lexical_cast.hpp>
#include "sitetitleupdater.h"
#include "contactinfo.h"
#include "logger.h"
SiteTitleUpdater::SiteTitleUpdater(Wt::WObject *parent)
    : Wt::WObject(parent)
{
    mpTitleUpdateRequest = new Wt::Signal<std::string>(this);
    mpTimer = new Wt::WTimer(this);
    mpTimer->setInterval(1000);
    mpTimer->setSingleShot(false);;
    mpTimer->timeout().connect(this,&SiteTitleUpdater::onTimeout);
    mpTimer->start();
    mCurrentIndex = 0;
}

void SiteTitleUpdater::messagesRead(unsigned int uin)
{
    for(unsigned int i = 0 ; i < mUpdateClients.size(); ++i)
    {
        CIIntPair pair = mUpdateClients[i];
        if(pair.first.uin == uin)
        {
            if(i <= mCurrentIndex && i > 0)
                --mCurrentIndex;
            auto it = mUpdateClients.begin();
            std::advance(it,i);
            mUpdateClients.erase(it);
            Logger::log("Did enter here");
        }
        Logger::log(std::string("This Uin") + boost::lexical_cast<std::string>(pair.first.uin) + " Checking Against " + boost::lexical_cast<std::string>(uin));
    }

    Logger::log(std::string("mUpdateClients size ..") + boost::lexical_cast<std::string>(mUpdateClients.size()));
}
void SiteTitleUpdater::newUnreadMessage(ContactInfo ci,unsigned int messageCount)
{
    for(unsigned int i = 0 ; i < mUpdateClients.size(); ++i)
    {
        CIIntPair &pair = mUpdateClients[i];
        if(pair.first == ci)
        {
            pair.second = messageCount;
            return;
        }
    }
    mUpdateClients.push_back(std::make_pair(ci,messageCount));
}

void SiteTitleUpdater::onTimeout()
{
    ++mCurrentIndex;
    if(mUpdateClients.size())
        mCurrentIndex = mCurrentIndex % mUpdateClients.size();
    else
        mCurrentIndex = 0;
    if(mUpdateClients.empty())
    {
        mpTitleUpdateRequest->emit("");
        return;
    }
    CIIntPair pair = mUpdateClients[mCurrentIndex];
    mpTitleUpdateRequest->emit(pair.first.getDisplayName() +"(" + boost::lexical_cast<std::string>(pair.second) + ")");
}

Wt::Signal<std::string> &SiteTitleUpdater::titleUpdateRequest()
{
    return *mpTitleUpdateRequest;
}
