
#include "pch.h"
#include "PreferenceParser.h"

PreferenceParser::PreferenceParser(const std::string & path)
    : BaseParser(path)
{
}

PreferenceParser::PreferenceParser(std::string && path)
    : BaseParser(std::move(path))
{
}

bool PreferenceParser::parseImpl(Preference * outData)
{
    int iTheme = m_Content.get<int>("uiThemeType", static_cast<int>(ThemeUbuntu));
    outData->setThemeType(static_cast<ThemeType>(iTheme));
    outData->m_ViewToolBar   = m_Content.get<bool>("viewToolBar", true);
    outData->m_ViewStatusBar = m_Content.get<bool>("viewStatusBar", true);

    return true;
}

bool PreferenceParser::saveImpl(const Preference & data)
{
    m_Content.put("uiThemeType", data.getThemeType());
    m_Content.put("viewToolBar", data.m_ViewToolBar);
    m_Content.put("viewStatusBar", data.m_ViewStatusBar);

    return true;
}
