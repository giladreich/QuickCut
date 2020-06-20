
#pragma once

#include "BaseParser.h"
#include "Models/Profile.h"

class ProfileParser : public BaseParser<std::vector<Profile *>>
{
public:
    ProfileParser(std::string && path);
    ProfileParser(const std::string & path);
    virtual ~ProfileParser();

private:
    bool parseImpl(std::vector<Profile *> * outData) override;
    bool saveImpl(const std::vector<Profile *> & data) override;
};
