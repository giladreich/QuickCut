
#pragma once

#include "QuickCutService.h"


class QuickCutServiceUnix : public QuickCutService
{
public:
    QuickCutServiceUnix(int argc, char * argv[]);
    ~QuickCutServiceUnix();

    void start() override;
    void pause() override;
    void resume() override;
    void stop() override;

    bool isProcessRunning(const QString & szProc) override;

public:

};
