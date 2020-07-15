
#include "QuickCutShared/QuickCutPCH.h"
#include "PreferenceParser.h"

PreferenceParser::PreferenceParser(const QString & path)
    : BaseParser(path)
{
}

PreferenceParser::PreferenceParser(QString && path)
    : BaseParser(std::move(path))
{
}

bool PreferenceParser::saveImpl(const Preference & data)
{
    QString themeName = QuickCut::fromKey<QCTheme::ThemeType>(data.theme().get());
    bpt::put(m_Content, "currentTheme", themeName);
    bpt::put(m_Content, "toolBarVisible", data.isToolBarVisible());
    bpt::put(m_Content, "statusBarVisible", data.isStatusBarVisible());

    return true;
}

bool PreferenceParser::parseImpl(Preference * outData)
{
    QString themeName = bpt::get(m_Content, "currentTheme", QString());
    outData->getTheme().set(QuickCut::fromValue<QCTheme::ThemeType>(themeName));
    outData->setToolBarVisible(bpt::get(m_Content, "toolBarVisible", true));
    outData->setStatusBarVisible(bpt::get(m_Content, "statusBarVisible", true));

    return true;
}
