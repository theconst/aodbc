#ifndef CONVERTERS_HH
#define CONVERTERS_HH

#include "nan.h"

#include "sqltypes.hh"

namespace AODBC {

using AODBC::sql_result_t;

template <typename T>
v8::Local<v8::Value> convert_cpp_type_to_js(const T& arg);

}  // namespace AODBC

#endif /* CONVERTERS_HH */

