
#pragma once

#include <QString>

class QCTheme
{
    Q_GADGET
public:
    enum ThemeType
    {
        ThemeInvalid = -1,
        ThemeDefault = 0,
        ThemeDark,
        ThemeBreezeDark,
        ThemeBreezeLight,
        ThemeConsoleDark,
        ThemeUbuntu,
        ThemeDarkOrange,

        ThemeCount
    };
    Q_ENUM(ThemeType)

    QCTheme(ThemeType type = ThemeUbuntu);
    ~QCTheme();

    ThemeType get() const;
    void      set(const ThemeType type);

    QString        getResourcePath() const;
    static QString getResourcePath(ThemeType type);

private:
    ThemeType m_Theme;
};