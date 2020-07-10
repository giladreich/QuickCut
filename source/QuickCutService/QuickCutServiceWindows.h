
#pragma once

#include "QuickCutShared/QuickCutPCH.h"
#include "QuickCutService.h"

class QuickCutServiceWindows : public QuickCutService
{
public:
    QuickCutServiceWindows(int argc, char * argv[]);
    ~QuickCutServiceWindows();

    void start() override;
    void stop() override;
    void pause() override;
    void resume() override;

    bool startHook() override;
    bool isHookRunning() override;

    bool isProcessRunning(const QString & process);
    bool RunProcessAsUserW(const std::wstring & process);
    bool RunProcessAsUserA(const std::string & process);
    bool GetTokenByName(HANDLE & token, const WCHAR * process);

private:
    void killHookIfRunning();

public:
    static QuickCutServiceWindows * s_Instance;
};
