
#include "pch.h"
#include "Preference.h"

Preference::Preference()
    : m_ToolBarVisible(true)
    , m_StatusBarVisible(true)
    , m_CurrTheme(ThemeUbuntu)
{
}

bool Preference::isToolBarVisible() const
{
    return m_ToolBarVisible;
}

void Preference::setToolBarVisible(bool visible)
{
    m_ToolBarVisible = visible;
}

bool Preference::isStatusBarVisible() const
{
    return m_StatusBarVisible;
}

void Preference::setStatusBarVisible(bool visible)
{
    m_StatusBarVisible = visible;
}

ThemeType Preference::getCurrentTheme() const
{
    return m_CurrTheme;
}

void Preference::setCurrentTheme(ThemeType type)
{
    m_CurrTheme = type;
}

QString Preference::getCurrentThemeResourcePath() const
{
    return getThemeResourcePath(m_CurrTheme);
}

QString Preference::getThemeResourcePath(ThemeType type)
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
