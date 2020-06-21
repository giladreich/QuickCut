#pragma once

#include "Types.h"
#include <string>

QCInterface IParserOperations
{
    virtual bool load() = 0;
    virtual bool save() = 0;

    virtual bool                isLoadSucceed()     = 0;
    virtual const std::string & getConfigFilePath() = 0;

protected:
    IParserOperations()          = default;
    virtual ~IParserOperations() = default;
};
