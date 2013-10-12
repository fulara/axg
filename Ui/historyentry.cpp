#include <Wt/WText>
#include <Wt/WBreak>
#include <Wt/WAnchor>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/regex.hpp>
#include <vector>
#include "historyentry.h"

static std::vector<std::wstring> splitByWhitespaceAndKeepDelimiters(const std::wstring str)
{
    enum MODE { WORD, WHITESPACE};
    MODE mode = isspace(str[0]) ? WHITESPACE : WORD;
    std::wstring currentWord;
    std::vector<std::wstring> words;
    BOOST_FOREACH(const wchar_t &c, str)
    {
        MODE currMode = isspace(c) ? WHITESPACE : WORD;
        if(currMode != mode)
        {
            words.push_back(currentWord);
            currentWord.clear();
        }
        currentWord.push_back(c);
        mode = currMode;
    }
    words.push_back(currentWord);
    return std::move(words);
}
/* leading scheme:// or "www."

   + ')', 'gi');
   */
bool isLink(const std::wstring& wstr)
{

    boost::wregex re(
                std::wstring() +
                + L"\\b([a-z][-a-z0-9+.]+://|www\\.)"
                // everything until non-URL character
                + L"[^\\s\'\"<>()]+"
                + L"|"
                // email
                + L"\\b[a-z0-9._%+-]+@[a-z0-9.-]+\\.[a-z]{2,4}\\b"
                    ,boost::regex::icase);
    boost::wsmatch match;
    return boost::regex_search(wstr, match, re);
    //return boost::regex_search(wstr.begin(),wstr.end(), wstr, re);

}
Wt::WString convertToLink(const std::wstring& wstr)
{
    return   Wt::WString("<a href=\"{1}\">{1}</a>").arg(wstr);
}



HistoryEntry::HistoryEntry(Wt::WContainerWidget *parent)
    : Wt::WContainerWidget(parent)
{
}

HistoryEntry *HistoryEntry::createMyEntryDateNickname(const std::string date, const std::string nickname, const std::string &msg, Wt::WContainerWidget *parent)
{
    HistoryEntry *newEntry = new HistoryEntry(parent);
    Wt::WText* dateWidget = new Wt::WText(std::string("(") + date + ")",newEntry);
    Wt::WText* nicknameWidget = new Wt::WText(nickname,newEntry);
    nicknameWidget->setInline(false);
    Wt::WText*  msgWidget = createMsgWidget(msg,newEntry);
    msgWidget->setInline(false);
    newEntry->addWidget(nicknameWidget);
    nicknameWidget->setStyleClass("ChatHistoryNick");
    newEntry->addWidget(dateWidget);
    dateWidget->setStyleClass("ChatHistoryTimeStamp");
    newEntry->addWidget(msgWidget);
    newEntry->setStyleClass("ChatHistoryEntryMine");
    return newEntry;
}

HistoryEntry *HistoryEntry::createMyEntryDate(const std::string date, const std::string &msg, Wt::WContainerWidget *parent)
{
    HistoryEntry *newEntry = new HistoryEntry(parent);
    Wt::WText* dateWidget = new Wt::WText(std::string("(") + date + ")",newEntry);
    Wt::WText*  msgWidget = createMsgWidget(msg,newEntry);
    msgWidget->setInline(false);
    newEntry->addWidget(dateWidget);
    dateWidget->setStyleClass("ChatHistoryTimeStamp");
    newEntry->addWidget(msgWidget);
    newEntry->setStyleClass("ChatHistoryEntryMine");
    return newEntry;
}

HistoryEntry *HistoryEntry::createMyEntry(const std::string &msg, Wt::WContainerWidget *parent)
{
    HistoryEntry *newEntry = new HistoryEntry(parent);
    Wt::WText*  msgWidget = createMsgWidget(msg,newEntry);
    msgWidget->setInline(false);
    newEntry->addWidget(msgWidget);
    newEntry->setStyleClass("ChatHistoryEntryMine");
    return newEntry;
}

HistoryEntry *HistoryEntry::createTargetEntryDateNickname(const std::string date, const std::string nickname, const std::string &msg, Wt::WContainerWidget *parent)
{
    HistoryEntry *newEntry = new HistoryEntry(parent);
    Wt::WText* dateWidget = new Wt::WText(std::string("(") + date + ")",newEntry);
    Wt::WText* nicknameWidget = new Wt::WText(nickname,newEntry);
    nicknameWidget->setInline(false);
    Wt::WText*  msgWidget = createMsgWidget(msg,newEntry);
    msgWidget->setInline(false);
    newEntry->addWidget(nicknameWidget);
    nicknameWidget->setStyleClass("ChatHistoryNick");
    newEntry->addWidget(dateWidget);
    dateWidget->setStyleClass("ChatHistoryTimeStamp");
    newEntry->addWidget(msgWidget);
    newEntry->setStyleClass("ChatHistoryEntryTarget");
    return newEntry;
}


HistoryEntry *HistoryEntry::createTargetEntryDate(const std::string date, const std::string &msg, Wt::WContainerWidget *parent)
{
    HistoryEntry *newEntry = new HistoryEntry(parent);
    Wt::WText* dateWidget = new Wt::WText(std::string("(") + date + ")",newEntry);
    Wt::WText*  msgWidget = createMsgWidget(msg,newEntry);
    msgWidget->setInline(false);
    newEntry->addWidget(dateWidget);
    dateWidget->setStyleClass("ChatHistoryTimeStamp");
    newEntry->addWidget(msgWidget);
    newEntry->setStyleClass("ChatHistoryEntryTarget");
    return newEntry;
}

Wt::WText* HistoryEntry::wrapLinks(const Wt::WString& msgContent)
{
    std::list<Wt::WWidget*> widgets;
    const std::wstring& contentAsWstr = msgContent.value();
    std::vector<std::wstring> tokens = splitByWhitespaceAndKeepDelimiters(contentAsWstr);
    Wt::WString content;
    BOOST_FOREACH(const std::wstring& wstr,tokens)
    {
        if(isLink(wstr))
        {
            content += convertToLink(wstr);
        }
        else
        {
            content += wstr;
        }
    }
    return new Wt::WText(content);
}

Wt::WText* HistoryEntry::createMsgWidget(const std::string &msg, HistoryEntry *newEntry)
{
    Wt::WString msgContent(Wt::WString::fromUTF8(msg));
    Wt::WText* widget = wrapLinks(msgContent);
    newEntry->addWidget(widget);
    return widget;
}

HistoryEntry *HistoryEntry::createTargetEntry(const std::string &msg, Wt::WContainerWidget *parent)
{
    HistoryEntry *newEntry = new HistoryEntry(parent);
    Wt::WAnchor* link = new Wt::WAnchor("http://google.pl",newEntry);
    Wt::WText*  msgWidget = createMsgWidget(msg,newEntry);
    msgWidget->setInline(false);
    newEntry->addWidget(msgWidget);
    newEntry->setStyleClass("ChatHistoryEntryTarget");
    return newEntry;
}
