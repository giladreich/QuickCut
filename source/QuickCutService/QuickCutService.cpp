
#include "QuickCutShared/QuickCutPCH.h"
#include "QuickCutService.h"

#include <QTimer>

QuickCutService::QuickCutService(int argc, char * argv[])
    : QtService<QCoreApplication>(argc, argv, QUICKCUTSERVICE_NAME)
    , m_CheckInterval(3 * 60 * 1000)
{
    setServiceDescription(
        "QuickCutService is responsible for making sure QuickCutConsole background process is "
        "running. The user-defined shortcut keys will only be activated, if QuickCutConsole "
        "background process is running. User defined shortcut keys are specified using the "
        "QuickCut - GUI.");

    setServiceFlags(QtServiceBase::CanBeSuspended);
    setStartupType(QtServiceController::StartupType::AutoStartup);
}

QuickCutService::~QuickCutService() {}

void QuickCutService::start()
{
    if (!m_Timer.isActive())
    {
        connect(&m_Timer, &QTimer::timeout, this, &QuickCutService::verifyHookRunning);
        m_Timer.start(m_CheckInterval);
    }
}

void QuickCutService::stop()
{
    disconnect(&m_Timer, &QTimer::timeout, this, &QuickCutService::verifyHookRunning);
    m_Timer.stop();
}

void QuickCutService::pause()
{
    QuickCutService::stop();
}

void QuickCutService::resume()
{
    QuickCutService::start();
}

void QuickCutService::verifyHookRunning()
{
    if (!isHookRunning()) startHook();
}