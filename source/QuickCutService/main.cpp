
#include "pch.h"
#include <QtCore/QCoreApplication>

#if defined(Q_OS_WIN)
#    include "QuickCutServiceWindows.h"
#elif defined(Q_OS_UNIX)
#    include "QuickCutServiceUnix.h"
#endif

int main(int argc, char * argv[])
{
    QCoreApplication::setApplicationName(QUICKCUTSERVICE_NAME);
    QCoreApplication::setApplicationVersion(QUICKCUT_VERSION);

#if defined(Q_OS_WIN)
    QuickCutServiceWindows service(argc, argv);
#elif defined(Q_OS_UNIX)
    QuickCutServiceUnix service(argc, argv);
#endif

    return service.exec();
}
