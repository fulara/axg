#include <Wt/WApplication>
#include <boost/lexical_cast.hpp>
#include "Utils/asynuitimer.h"
#include "sitetitleupdater.h"
#include "contactinfo.h"
#include "logger.h"
SiteTitleUpdater::SiteTitleUpdater(Wt::WObject *parent)
    : Wt::WObject(parent)
{
    mpTitleUpdateRequest = new Wt::Signal<std::string>(this);
    mpTimer = new AsyncUiTimer(500,Wt::WApplication::instance()->sessionId(),boost::bind(&SiteTitleUpdater::onTimeout,this),this);
    mCurrentIndex = 0;
    mSiteHasFocus = true;
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
        }
    }
}
void SiteTitleUpdater::newUnreadMessage(ContactInfo ci,unsigned int messageCount)
{
    if(messageCount == 0 && mSiteHasFocus)
        return;
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
void SiteTitleUpdater::clearZeroCountPairs()
{
    for(unsigned int i = 0 ; i < mUpdateClients.size(); ++i)
    {
        CIIntPair pair = mUpdateClients[i];
        if(pair.second == 0)
        {
            if(i <= mCurrentIndex && i > 0)
                --mCurrentIndex;
            auto it = mUpdateClients.begin();
            std::advance(it,i);
            mUpdateClients.erase(it);
        }
    }
}
void SiteTitleUpdater::siteGainedFocus()
{
    mSiteHasFocus = true;
    clearZeroCountPairs();
}
void SiteTitleUpdater::siteLostFocus()
{
    mSiteHasFocus = false;

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
    std::string newTitle = "(";
    newTitle += boost::lexical_cast<std::string>(mUpdateClients.size());
    newTitle += ") ";
    newTitle += pair.first.getDisplayName();

    if(pair.second)
    {
        mpTitleUpdateRequest->emit(newTitle +" [" + boost::lexical_cast<std::string>(pair.second) + "]");
    }
    else
    {
        mpTitleUpdateRequest->emit(newTitle);
    }

}
Wt::Signal<std::string> &SiteTitleUpdater::titleUpdateRequest()
{
    return *mpTitleUpdateRequest;
}
