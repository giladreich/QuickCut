
#pragma once

#include "BaseParser.h"

#include "QuickCutShared/Models/Preference.h"

class PreferenceParser : public BaseParser<Preference>
{
public:
    PreferenceParser(const QString & path);
    PreferenceParser(QString && path);
    virtual ~PreferenceParser() = default;

private:
    bool parseImpl(Preference * outData) override;
    bool saveImpl(const Preference & data) override;
};
