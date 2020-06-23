
#include "pch.h"
#include "PreferenceParser.h"

PreferenceParser::PreferenceParser(const QString & path)
    : BaseParser(path)
{
}

PreferenceParser::PreferenceParser(QString && path)
    : BaseParser(std::move(path))
{
}

bool PreferenceParser::parseImpl(Preference * outData)
{
    QString themeName = bpt::get(m_Content, "currentTheme", "");
    outData->getTheme().set(QuickCut::fromValue<QCTheme::ThemeType>(themeName));
    outData->setToolBarVisible(m_Content.get<bool>("toolBarVisible", true));
    outData->setStatusBarVisible(m_Content.get<bool>("statusBarVisible", true));

    return true;
}

bool PreferenceParser::saveImpl(const Preference & data)
{
    QString themeName = QuickCut::fromKey<QCTheme::ThemeType>(data.theme().get());
    bpt::put(m_Content, "currentTheme", themeName);
    m_Content.put("toolBarVisible", data.isToolBarVisible());
    m_Content.put("statusBarVisible", data.isStatusBarVisible());

    return true;
}
