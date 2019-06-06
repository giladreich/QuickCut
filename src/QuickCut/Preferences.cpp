
#include "Preferences.h"

Preferences::Preferences()
    : m_bViewToolBar(true)
    , m_bViewStatusBar(true)
    , m_eThemeType(ThemeUbuntu)
{
}

QString Preferences::getThemeFilePath() const
{
    return getThemeFilePath(m_eThemeType);
}

QString Preferences::getThemeFilePath(eThemeType eType)
{
    switch (eType)
    {
    case ThemeDefault:     return "";
    case ThemeDark:        return ":stylesheet/theme_dark.qss";
    case ThemeBreezeDark:  return ":stylesheet/theme_breezeDark.qss";
    case ThemeBreezeLight: return ":stylesheet/theme_breezeLight.qss";
    case ThemeConsoleDark: return ":stylesheet/theme_console.qss";
    case ThemeUbuntu:
    default:               return ":stylesheet/theme_ubuntu.qss";
    }
}

eThemeType Preferences::getThemeType() const
{
    return m_eThemeType;
}

void Preferences::setThemeType(eThemeType eType)
{
    m_eThemeType = eType;
}
