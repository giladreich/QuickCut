
#pragma once

#include "BaseParser.h"

#include "QuickCutShared/Models/Profile.h"

class ProfileParser : public BaseParser<std::vector<Profile *>>
{
public:
    ProfileParser(QString && path);
    ProfileParser(const QString & path);
    virtual ~ProfileParser() = default;

private:
    bool saveImpl(const std::vector<Profile *> & data) override;
    bool parseImpl(std::vector<Profile *> * outData) override;
};
