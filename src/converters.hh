#ifndef CONVERTERS_HH
#define CONVERTERS_HH

#include "nan.h"

#include "sqltypes.hh"

namespace AODBC {

using AODBC::sql_result_t;

v8::Local<v8::Array> convert_cpp_type_to_js(const sql_result_t& result);

void convert_cpp_type_to_js(
    v8::Local<v8::Object> result,
    const nanodbc::date& date);
void convert_cpp_type_to_js(
    v8::Local<v8::Object> result,
    const nanodbc::time& date);
void convert_cpp_type_to_js(
    v8::Local<v8::Object> result,
    const nanodbc::timestamp& date);

}  // namespace AODBC

#endif /* CONVERTERS_HH */

