// Precompiled Header
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
#include "QuickCutShared/Types.h"
#include "QuickCutShared/Utils/Utility.h"

// Other includes
#if defined(Q_OS_WIN)
#    define WIN32_LEAN_AND_MEAN
#    include <Windows.h>
#endif

// Defines
#define QUICKCUT_VERSION     "2.0.0"
#define QUICKCUT_IPC         "QuickCut_IPC_" QUICKCUT_VERSION
#define QUICKCUT_NAME        "QuickCut"
#define QUICKCUTCONSOLE_NAME "QuickCutConsole"
#define QUICKCUTSERVICE_NAME "QuickCutService"

#if defined(Q_OS_WIN)
#    define QUICKCUT_BIN        QUICKCUT_NAME ".exe"
#    define QUICKCUTCONSOLE_BIN QUICKCUTCONSOLE_NAME ".exe"
#    define QUICKCUTSERVICE_BIN QUICKCUTSERVICE_NAME ".exe"
#else
#    define QUICKCUT_BIN        QUICKCUT_NAME
#    define QUICKCUTCONSOLE_BIN QUICKCUTCONSOLE_NAME
#    define QUICKCUTSERVICE_BIN QUICKCUTSERVICE_NAME
#endif