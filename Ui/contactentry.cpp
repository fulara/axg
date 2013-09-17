#include <Wt/WWidget>
#include <Wt/WText>
#include <Wt/WSignal>
#include <Wt/WImage>
#include <Wt/WHBoxLayout>
#include <boost/algorithm/string.hpp>
#include "logger.h"
#include "contactentry.h"

void ContactEntry::ensureProperTextLength(std::string& text)
{
    if(text.size() > 17)
    {
        text.resize(15);
        text.append("...");
    }
}

ContactEntry::ContactEntry(unsigned int uin, std::string name)
    : mUin(uin)
{
    ensureProperTextLength(name);
    setStyleClass("ContactEntry");
    Wt::WHBoxLayout *layout = new Wt::WHBoxLayout();

    Wt::WContainerWidget* imageHolder = new Wt::WContainerWidget();
    imageHolder->resize(20,"100%");
    layout->addWidget(imageHolder);
    Wt::WImage *image = new Wt::WImage("images/na.png");
    imageHolder->addWidget(image);
    mpShowText = new Wt::WText();
    if(name.empty())
    {
        mpShowText->setText(boost::lexical_cast<std::string>(uin));
    }
    else
    {
        mpShowText->setText(Wt::WString::fromUTF8(name));
    }

    layout->addWidget(mpShowText,1);

    setLayout(layout);
}


bool ContactEntry::matchesFilter(std::string filter) const
{
    std::string name = mpShowText->text().toUTF8();
    auto result = boost::algorithm::ifind_first(name,filter);
    if (!result.empty()) return true;

    std::string uin = (boost::lexical_cast<std::string>(mUin));
    result = boost::algorithm::ifind_first(uin,filter);

    return !result.empty();
}
