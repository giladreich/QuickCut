
#pragma once

#include <boost/property_tree/json_parser.hpp>

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

namespace bpt = boost::property_tree;
typedef bpt::ptree JSON;

#if defined(_MSC_VER)
#    define QCInterface struct __declspec(novtable)
#else
#    define QCInterface struct
#endif
