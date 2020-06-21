
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
#include <QFile>
#include <QDir>
#include <QProcess>
#include <QTextStream>
#include <QStandardPaths>

// Project includes
#include "Types.h"

// Other includes
#if defined(Q_OS_WIN)
#    define WIN32_LEAN_AND_MEAN
#    include <Windows.h>
#endif
