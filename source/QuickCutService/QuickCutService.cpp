
#include "QuickCutService.h"

#include "pch.h"

#include <QFile>
#include <QTextStream>

QuickCutService::QuickCutService(int argc, char * argv[])
    : QtService<QCoreApplication>(argc, argv, "QuickCut Service")
{
    setServiceDescription("QuickCut Service is responsible for activating the shortcut-keys "
                          "defined by the user using the QuickCut GUI. It starts "
                          "QuickCutConsole background process "
                          "as the currently signed-in user in order allow key messages "
                          "communication between processes.");

    setServiceFlags(QtServiceBase::CanBeSuspended);
    setStartupType(QtServiceController::StartupType::AutoStartup);
}

QuickCutService::~QuickCutService() {}

void QuickCutService::start() {}

void QuickCutService::pause()
{
    stop();
}

void QuickCutService::resume()
{
    start();
}

void QuickCutService::stop() {}

bool QuickCutService::isProcessRunning(const QString & szProc)
{
    return false;
}

void QuickCutService::log(const QString & szFilePath, const QString & szMessage)
{
    QFile file(szFilePath);
    file.open(QFile::WriteOnly);
    QTextStream text(&file);
    text << szMessage;
    file.flush();
    file.close();
}
