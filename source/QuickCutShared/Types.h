
#pragma once

#include <boost/property_tree/json_parser.hpp>

#define QUICKCUT_VERSION "2.0.0"
#define QUICKCUT_IPC     "QuickCut_IPC_" QUICKCUT_VERSION

namespace bpt = boost::property_tree;
typedef bpt::ptree JSON;

#if defined(_MSC_VER)
#    define QCInterface struct __declspec(novtable)
#else
#    define QCInterface struct
#endif
