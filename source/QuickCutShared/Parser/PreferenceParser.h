
#pragma once

#include "BaseParser.h"
#include "Models/Preferences.h"

class PreferenceParser : public BaseParser<Preferences>
{
public:
    PreferenceParser(const std::string & path);
    PreferenceParser(std::string && path);
    virtual ~PreferenceParser() = default;

private:
    bool parseImpl(Preferences * outData) override;
    bool saveImpl(const Preferences & data) override;
};
