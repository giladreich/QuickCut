
#pragma once

#include <boost/property_tree/json_parser.hpp>

namespace bpt = boost::property_tree;
typedef bpt::ptree JSON;

#if defined(_MSC_VER)
#    define QCInterface struct __declspec(novtable)
#else
#    define QCInterface struct
#endif
