
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
    int iTheme = m_Content.get<int>("currentTheme", static_cast<int>(ThemeUbuntu));
    outData->setCurrentTheme(static_cast<ThemeType>(iTheme));
    outData->setToolBarVisible(m_Content.get<bool>("toolBarVisible", true));
    outData->setStatusBarVisible(m_Content.get<bool>("statusBarVisible", true));

    return true;
}

bool PreferenceParser::saveImpl(const Preference & data)
{
    m_Content.put("currentTheme", data.getCurrentTheme());
    m_Content.put("toolBarVisible", data.isToolBarVisible());
    m_Content.put("statusBarVisible", data.isStatusBarVisible());

    return true;
}
