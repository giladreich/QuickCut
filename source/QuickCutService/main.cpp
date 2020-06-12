
#include <QtCore/QCoreApplication>

#ifdef Q_OS_WIN
#    include "QuickCutServiceWindows.h"
#elif Q_OS_UNIX
#    include "QuickCutServiceUnix.h"
#endif

int main(int argc, char * argv[])
{
#ifdef Q_OS_WIN
    QuickCutServiceWindows service(argc, argv);
#elif Q_OS_UNIX
    QuickCutServiceUnix service(argc, argv);
#endif

    return service.exec();
}
