
#include "pch.h"
#include "QCTheme.h"

QCTheme::QCTheme(QCTheme::ThemeType type)
    : m_Theme(type)
{
}

QCTheme::~QCTheme() = default;

QCTheme::ThemeType QCTheme::get() const
{
    return m_Theme;
}

void QCTheme::set(const QCTheme::ThemeType type)
{
    if (type != QCTheme::ThemeInvalid) m_Theme = type;
}

QString QCTheme::getResourcePath() const
{
    return getResourcePath(m_Theme);
}

QString QCTheme::getResourcePath(ThemeType type)
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
