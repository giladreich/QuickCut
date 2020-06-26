
#pragma once

#include "QuickCutConsole.h"

class QuickCutConsoleUnix : public QuickCutConsole
{

public:
    QuickCutConsoleUnix(int argc, char * argv[]);
    ~QuickCutConsoleUnix();

    bool start() override;
    bool stop() override;

    void sendInput(const QStringList & dstKeys) override;
    void executeProcess(const QString & process, const QString & arguments) override;
};
