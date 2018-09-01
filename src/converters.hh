#ifndef CONVERTERS_HH
#define CONVERTERS_HH

#include "nan.h"

#include "sqltypes.hh"

namespace AODBC {
    
using namespace AODBC;

v8::Local<v8::Array> ConvertNanodbcTypeToJsObject(const sql_result_t& result);

}

#endif /* CONVERTERS_HH */

