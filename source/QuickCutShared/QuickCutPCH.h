
#pragma once

// STL includes
#include <string>
#include <vector>
#include <algorithm>
#include <utility>
#include <memory>
#include <iostream>

// Qt includes
#include <QtCore>
#include <QDebug>
#include <QString>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QtNetwork>
#include <QLocalSocket>
#include <QTextStream>
#include <QStandardPaths>

// Project includes
#include "Types.h"
#include "Utils/Utility.h"

// Other includes
#if defined(Q_OS_WIN)
#    define WIN32_LEAN_AND_MEAN
#    include <Windows.h>
#endif
