
#pragma once

#include "BaseParser.h"
#include "Models/Preference.h"

class PreferenceParser : public BaseParser<Preference>
{
public:
    PreferenceParser(const std::string & path);
    PreferenceParser(std::string && path);
    virtual ~PreferenceParser() = default;

private:
    bool parseImpl(Preference * outData) override;
    bool saveImpl(const Preference & data) override;
};
