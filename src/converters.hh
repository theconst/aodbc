
/* 
 * File:   converters.hh
 * Author: kko
 * 
 * 
 * Converters for common nanodbc types
 * 
 * Javascript parameters are passed by reference, ie. handle should be created
 * Really do not see benefit of implementing other than this will repeat
 * nan factory
 * Created on August 29, 2018, 10:57 PM
 */

#ifndef CONVERTERS_HH
#define CONVERTERS_HH

#include "nan.h"

#include "sqltypes.hh"

namespace AODBC {
    
using namespace AODBC;

v8::Local<v8::Array> ConvertNanodbcTypeToJsObject(const sql_result_t& result);

}

#endif /* CONVERTERS_HH */

