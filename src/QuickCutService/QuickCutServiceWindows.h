
#pragma once

#include "QuickCutService.h"

#include <Windows.h>


class QuickCutServiceWindows : public QuickCutService
{
public:
    QuickCutServiceWindows(int argc, char * argv[]);
    ~QuickCutServiceWindows();

    void start() override;
    void pause() override;
    void resume() override;
    void stop() override;

    bool isProcessRunning(const QString & szProc) override;

    bool RunProcessAsUserW(const std::wstring & szProc);
    bool RunProcessAsUserA(const std::string & szProc);
    bool GetTokenByName(HANDLE & hToken, const WCHAR * szProcName);

private:
    bool killHookIfRunning();

public:
    static QuickCutServiceWindows * s_pInstance;

};
