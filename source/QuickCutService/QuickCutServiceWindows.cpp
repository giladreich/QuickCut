
#pragma comment(lib, "userenv.lib")

#include "QuickCutShared/QuickCutPCH.h"
#include "QuickCutServiceWindows.h"

#include <TlHelp32.h>

QuickCutServiceWindows * QuickCutServiceWindows::s_Instance = nullptr;

QuickCutServiceWindows::QuickCutServiceWindows(int argc, char * argv[])
    : QuickCutService(argc, argv)
{
    s_Instance = this;
}

QuickCutServiceWindows::~QuickCutServiceWindows()
{
    killHookIfRunning();
}

void QuickCutServiceWindows::start()
{
    QuickCutService::start();

    startHook();
}

void QuickCutServiceWindows::stop()
{
    QuickCutService::stop();

    killHookIfRunning();
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

bool QuickCutServiceWindows::startHook()
{
    QString      appPath = QCoreApplication::applicationDirPath();
    std::wstring process = QDir(appPath).filePath(QUICKCUTCONSOLE_BIN).toStdWString();

    return RunProcessAsUserW(process);
}

bool QuickCutServiceWindows::isHookRunning()
{
    return isProcessRunning(QUICKCUTCONSOLE_BIN);
}

void QuickCutServiceWindows::killHookIfRunning()
{
    if (isHookRunning()) WinExec("taskkill /f /t /im " QUICKCUTCONSOLE_BIN, SW_HIDE);
}

bool QuickCutServiceWindows::isProcessRunning(const QString & process)
{
    PROCESSENTRY32 procEntry;
    memset(&procEntry, 0, sizeof(PROCESSENTRY32));

    if (process.isEmpty()) return false;

    HANDLE procSnap = nullptr;
    procSnap        = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (procSnap == INVALID_HANDLE_VALUE) return false;

    procEntry.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(procSnap, &procEntry))
    {
        CloseHandle(procSnap);
        return false;
    }

    wchar_t * processW         = new wchar_t[process.length() + 1];
    processW[process.length()] = '\0';
    process.toWCharArray(processW);
    do
    {
        if (_wcsicmp(procEntry.szExeFile, processW) == 0) // equals
        {
            delete[] processW;
            CloseHandle(procSnap);
            return true;
        }
    } while (Process32Next(procSnap, &procEntry));

    delete[] processW;

    CloseHandle(procSnap);
    return false;
}

bool QuickCutServiceWindows::RunProcessAsUserA(const std::string & process)
{
    if (process.empty()) return false;

    HANDLE token = nullptr;
    if (!OpenThreadToken(GetCurrentThread(), TOKEN_QUERY, true, &token))
    {
        qDebug() << "[QuickCutServiceWindows::RunProcessAsUserA] - Failed to retrieve token "
                    "handle for user.";
        return false;
    }

    STARTUPINFOA        startInfo;
    PROCESS_INFORMATION procInfo;
    memset(&startInfo, 0, sizeof(startInfo));
    memset(&procInfo, 0, sizeof(procInfo));

    char siDesktop[]      = "winsta0\\default";
    startInfo.cb          = sizeof(STARTUPINFOA);
    startInfo.lpDesktop   = siDesktop;
    startInfo.wShowWindow = SW_HIDE;

    bool succeed =
        CreateProcessAsUserA(token, process.c_str(), nullptr, nullptr, nullptr, false,
                             CREATE_NO_WINDOW, nullptr, nullptr, &startInfo, &procInfo);

    if (succeed)
    {
        qDebug() << "[QuickCutServiceWindows::RunProcessAsUserA] - Successfully created "
                    "process as user.";
    }
    else
    {
        qDebug() << "[QuickCutServiceWindows::RunProcessAsUserA] - Failed to create process "
                    "as user.";
    }

    CloseHandle(token);
    return succeed;
}

bool QuickCutServiceWindows::RunProcessAsUserW(const std::wstring & process)
{
    if (process.empty()) return false;

    HANDLE token = nullptr;
    if (!GetTokenByName(token, L"explorer.exe"))
    {
        qDebug() << "[QuickCutServiceWindows::RunProcessAsUserW] - Failed to retrieve "
                    "explorer token handle for user.";
        return false;
    }

    STARTUPINFO         startInfo;
    PROCESS_INFORMATION procInfo;
    memset(&startInfo, 0, sizeof(STARTUPINFO));
    memset(&procInfo, 0, sizeof(PROCESS_INFORMATION));

    wchar_t siDesktop[]   = L"winsta0\\default";
    startInfo.cb          = sizeof(STARTUPINFO);
    startInfo.lpDesktop   = siDesktop;
    startInfo.wShowWindow = SW_HIDE;

    bool succeed =
        CreateProcessAsUser(token, process.c_str(), nullptr, nullptr, nullptr, false,
                            CREATE_NO_WINDOW, nullptr, nullptr, &startInfo, &procInfo);
    if (succeed)
    {
        qDebug() << "[QuickCutServiceWindows::RunProcessAsUserW] - Successfully created "
                    "process as user.";
    }
    else
    {
        qDebug() << "[QuickCutServiceWindows::RunProcessAsUserW] - Failed to create process "
                    "as user.";
    }

    CloseHandle(token);
    return succeed;
}

bool QuickCutServiceWindows::GetTokenByName(HANDLE & token, const WCHAR * processName)
{
    if (!processName) return false;

    HANDLE procSnap = nullptr;
    procSnap        = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (procSnap == INVALID_HANDLE_VALUE) return false;

    PROCESSENTRY32 procEntry;
    memset(&procEntry, 0, sizeof(PROCESSENTRY32));
    procEntry.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(procSnap, &procEntry))
    {
        CloseHandle(procSnap);
        return false;
    }

    do
    {
        if (_wcsicmp(procEntry.szExeFile, processName) == 0) // equals
        {
            HANDLE hProcess =
                OpenProcess(PROCESS_QUERY_INFORMATION, false, procEntry.th32ProcessID);
            bool succeed = OpenProcessToken(hProcess, TOKEN_ALL_ACCESS, &token);
            CloseHandle(procSnap);
            return succeed;
        }
    } while (Process32Next(procSnap, &procEntry));

    CloseHandle(procSnap);
    return true;
}
