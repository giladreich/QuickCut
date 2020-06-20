
#include "pch.h"
#include "QuickCutService.h"

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

bool QuickCutService::isProcessRunning(const QString & process)
{
    return false;
}

void QuickCutService::log(const QString & filePath, const QString & text)
{
    QFile file(filePath);
    file.open(QFile::WriteOnly);
    QTextStream ts(&file);
    ts << text;
    file.flush();
    file.close();
}
