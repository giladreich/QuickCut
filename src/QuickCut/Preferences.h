
#pragma once

#include <QString>


enum eThemeType
{
    ThemeDefault = 0,
    ThemeDark,
    ThemeBreezeDark,
    ThemeBreezeLight,
    ThemeConsoleDark,
    ThemeUbuntu,
    ThemeDarkOrange,

    THEME_COUNT
};

class Preferences
{
public:
    Preferences();

    QString getThemeFilePath() const;
    static QString getThemeFilePath(eThemeType eType);
    eThemeType getThemeType() const;
    void setThemeType(eThemeType eType);

public:
    bool       m_bViewToolBar;
    bool       m_bViewStatusBar;

    eThemeType m_eThemeType;

};