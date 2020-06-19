
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

    QString        getThemeFilePath() const;
    static QString getThemeFilePath(eThemeType type);
    eThemeType     getThemeType() const;
    void           setThemeType(eThemeType type);

public:
    bool m_ViewToolBar;
    bool m_ViewStatusBar;

    eThemeType m_ThemeType;
};