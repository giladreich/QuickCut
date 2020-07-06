#pragma once

#include "QuickCutShared/Types.h"
#include <QString>

QCInterface IParserOperations
{
    virtual bool save() = 0;
    virtual bool load() = 0;

    virtual bool            isLoadSucceed()     = 0;
    virtual const QString & getConfigFilePath() = 0;

protected:
    IParserOperations()          = default;
    virtual ~IParserOperations() = default;
};
