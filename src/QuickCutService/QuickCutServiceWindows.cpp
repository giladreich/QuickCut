
#pragma comment(lib, "userenv.lib")

#include "QuickCutServiceWindows.h"
#include "pch.h"

#include <QDebug>
#include <QDir>
#include <QProcess>

#include <TlHelp32.h>

#undef UNICODE

#define QUICKCUT_CONSOLE "QuickCutConsole.exe"

QuickCutServiceWindows * QuickCutServiceWindows::s_pInstance = nullptr;


QuickCutServiceWindows::QuickCutServiceWindows(int argc, char * argv[])
    : QuickCutService(argc, argv)
{
    s_pInstance = this;
}

QuickCutServiceWindows::~QuickCutServiceWindows()
{
    killHookIfRunning();
}

void QuickCutServiceWindows::start()
{
    killHookIfRunning();

    QuickCutService::start();

    std::wstring szProc = (QCoreApplication::applicationDirPath() + "/" + QUICKCUT_CONSOLE).toStdWString();
    RunProcessAsUserW(szProc);
}

void QuickCutServiceWindows::pause()
{
    QuickCutService::pause();

    stop();
}

void QuickCutServiceWindows::resume()
{
    QuickCutService::resume();

    start();
}

void QuickCutServiceWindows::stop()
{
    QuickCutService::stop();

    killHookIfRunning();
}


void QuickCutServiceWindows::killHookIfRunning()
{
    if (isProcessRunning(QUICKCUT_CONSOLE))
        QProcess::execute("taskkill /im " QUICKCUT_CONSOLE " /f");
}

bool QuickCutServiceWindows::isProcessRunning(const QString & szProc)
{
    PROCESSENTRY32 procEntry;
    memset(&procEntry, 0, sizeof(PROCESSENTRY32));

    if (szProc.isEmpty())
        return false;

    HANDLE hProcSnap = nullptr;
    hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcSnap == INVALID_HANDLE_VALUE)
        return false;

    procEntry.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hProcSnap, &procEntry))
    {
        CloseHandle(hProcSnap);
        return false;
    }


    wchar_t * szProcW = new wchar_t[szProc.length()+1];
    szProcW[szProc.length()] = '\0';
    szProc.toWCharArray(szProcW);
    do
    {
        if (_wcsicmp(procEntry.szExeFile, szProcW) == 0) // equals
        {
            delete [] szProcW;
            CloseHandle(hProcSnap);
            return true;
        }
    } while (Process32Next(hProcSnap, &procEntry));

    delete [] szProcW;

    CloseHandle(hProcSnap);
    return false;
}


bool QuickCutServiceWindows::RunProcessAsUserA(const std::string & szProc)
{
    if (szProc.empty())
        return false;

    HANDLE hToken = nullptr;
    if (!OpenThreadToken(GetCurrentThread(), TOKEN_QUERY, true, &hToken))
    {
        qDebug() << "[QuickCutServiceWindows::RunProcessAsUserA] - Failed to retrieve token handle for user.";
        return false;
    }

    STARTUPINFOA startInfo;
    PROCESS_INFORMATION procInfo;
    memset(&startInfo, 0, sizeof(startInfo));
    memset(&procInfo, 0, sizeof(procInfo));

    startInfo.cb = sizeof(STARTUPINFOA);
    startInfo.lpDesktop = "winsta0\\default";

    bool bSucceed = CreateProcessAsUserA(hToken, szProc.c_str(), nullptr, nullptr, nullptr,
                                        false, 0, nullptr, nullptr,
                                        &startInfo, &procInfo);

    if (bSucceed)
    {
        qDebug() << "[QuickCutServiceWindows::RunProcessAsUserA] - Successfully created process as user.";
    }
    else
    {
        qDebug() << "[QuickCutServiceWindows::RunProcessAsUserA] - Failed to create process as user.";
    }

    CloseHandle(hToken);
    return bSucceed;
}

bool QuickCutServiceWindows::RunProcessAsUserW(const std::wstring & szProc)
{
    if (szProc.empty())
        return false;

    HANDLE hToken = nullptr;
    if (!GetTokenByName(hToken, L"explorer.exe"))
    {
        qDebug() << "[QuickCutServiceWindows::RunProcessAsUserW] - Failed to retrieve explorer token handle for user.";
        return false;
    }
     
    STARTUPINFO startInfo;
    PROCESS_INFORMATION procInfo;
    memset(&startInfo, 0, sizeof(STARTUPINFO));
    memset(&procInfo, 0, sizeof(PROCESS_INFORMATION));

    startInfo.cb = sizeof(STARTUPINFO);
    startInfo.lpDesktop = L"winsta0\\default";

    bool bSucceed = CreateProcessAsUser(hToken, szProc.c_str(), nullptr, nullptr, nullptr,
                                       false, NORMAL_PRIORITY_CLASS, nullptr, nullptr,
                                       &startInfo, &procInfo);
    if (bSucceed)
    {
        qDebug() << "[QuickCutServiceWindows::RunProcessAsUserW] - Successfully created process as user.";
    }
    else
    {
        qDebug() << "[QuickCutServiceWindows::RunProcessAsUserW] - Failed to create process as user.";
    }

    CloseHandle(hToken);
    return bSucceed;
}

bool QuickCutServiceWindows::GetTokenByName(HANDLE & hToken, const WCHAR * szProcName)
{
    if (!szProcName)
        return false;

    HANDLE hProcSnap = nullptr;
    hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcSnap == INVALID_HANDLE_VALUE)
        return false;

    PROCESSENTRY32 procEntry;
    memset(&procEntry, 0, sizeof(PROCESSENTRY32));
    procEntry.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hProcSnap, &procEntry))
    {
        CloseHandle(hProcSnap);
        return false;
    }

    do
    {
        if (_wcsicmp(procEntry.szExeFile, szProcName) == 0) // equals
        {
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, false, procEntry.th32ProcessID);
            bool bSucceed = OpenProcessToken(hProcess, TOKEN_ALL_ACCESS, &hToken);
            CloseHandle(hProcSnap);
            return bSucceed;
        }
    } while (Process32Next(hProcSnap, &procEntry));

    CloseHandle(hProcSnap);
    return true;
}
