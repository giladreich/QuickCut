
#pragma once

#include "QuickCutService.h"

class QuickCutServiceUnix : public QuickCutService
{
public:
    QuickCutServiceUnix(int argc, char * argv[]);
    ~QuickCutServiceUnix();

    void start() override;
    void stop() override;
    void pause() override;
    void resume() override;

    bool startHook() override;
    bool isHookRunning() override;

private:
    void killHookIfRunning();

public:
};
