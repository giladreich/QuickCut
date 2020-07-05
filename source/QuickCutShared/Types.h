
#pragma once

#include <QVector>
#include <boost/property_tree/json_parser.hpp>

// Namespaces
namespace bpt = boost::property_tree;

// Typedefs
class KeyData;

typedef bpt::ptree       JSON;
typedef QVector<KeyData> KeyboardKeys;

// Macros
#if defined(_MSC_VER)
#    define QCInterface struct __declspec(novtable)
#else
#    define QCInterface struct
#endif
