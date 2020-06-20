
#include "pch.h"
#include "Preferences.h"

Preferences::Preferences()
    : m_ViewToolBar(true)
    , m_ViewStatusBar(true)
    , m_ThemeType(ThemeUbuntu)
{
}

QString Preferences::getThemeFilePath() const
{
    return getThemeFilePath(m_ThemeType);
}

QString Preferences::getThemeFilePath(ThemeType type)
{
    switch (type)
    {
        case ThemeDefault:
            return "";
        case ThemeDark:
            return ":stylesheet/theme_dark.qss";
        case ThemeBreezeDark:
            return ":stylesheet/theme_breezeDark.qss";
        case ThemeBreezeLight:
            return ":stylesheet/theme_breezeLight.qss";
        case ThemeConsoleDark:
            return ":stylesheet/theme_console.qss";
        case ThemeDarkOrange:
            return ":stylesheet/theme_darkorange.qss";
        case ThemeUbuntu:
        default:
            return ":stylesheet/theme_ubuntu.qss";
    }
}

ThemeType Preferences::getThemeType() const
{
    return m_ThemeType;
}

void Preferences::setThemeType(ThemeType type)
{
    m_ThemeType = type;
}
