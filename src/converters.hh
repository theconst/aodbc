#ifndef CONVERTERS_HH
#define CONVERTERS_HH

#include "nan.h"

#include "sqltypes.hh"

namespace AODBC {

using AODBC::sql_result_t;

v8::Local<v8::Array> ConvertNanodbcTypeToJsObject(const sql_result_t& result);

}  // namespace AODBC

#endif /* CONVERTERS_HH */

