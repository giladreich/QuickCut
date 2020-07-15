
#pragma once

#include <QVector>
#include <boost/property_tree/json_parser.hpp>

// Namespaces
namespace bpt = boost::property_tree;

// Typedefs
class KeyData;

#if defined(UNICODE)
typedef bpt::wptree JSON;
#else
typedef bpt::ptree JSON;
#endif
typedef QVector<KeyData> KeyboardKeys;

// Macros
#if defined(_MSC_VER)
#    define QCInterface struct __declspec(novtable)
#else
#    define QCInterface struct
#endif
