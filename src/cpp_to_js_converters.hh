#ifndef CPPTOJSCONVERTERS_HH
#define CPPTOJSCONVERTERS_HH

#include "nan.h"

#include "nctypes.hh"

namespace NC {

v8::Local<v8::Value> convert_cpp_type_to_js(const nc_string_t&);
v8::Local<v8::Value> convert_cpp_type_to_js(const nc_result_t&);
v8::Local<v8::Value> convert_cpp_type_to_js(bool);
v8::Local<v8::Value> convert_cpp_type_to_js(nc_null_t);

}  // namespace NC

#endif /* CPPTOJSCONVERTERS_HH */

