
#pragma once

#include "QuickCutPCH.h"
#include "QuickCutService.h"

class QuickCutServiceWindows : public QuickCutService
{
public:
    QuickCutServiceWindows(int argc, char * argv[]);
    ~QuickCutServiceWindows();

    void start() override;
    void pause() override;
    void resume() override;
    void stop() override;

    bool isProcessRunning(const QString & process) override;

    bool RunProcessAsUserW(const std::wstring & process);
    bool RunProcessAsUserA(const std::string & process);
    bool GetTokenByName(HANDLE & token, const WCHAR * process);

private:
    bool killHookIfRunning();

public:
    static QuickCutServiceWindows * s_Instance;
};
