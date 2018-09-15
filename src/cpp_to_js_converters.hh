#ifndef CPPTOJSCONVERTERS_HH
#define CPPTOJSCONVERTERS_HH

#include "nan.h"

#include "nctypes.hh"

namespace NC {

template<typename T>
v8::Local<v8::Value> convert_cpp_type_to_js(const T&);


template<>
v8::Local<v8::Value> convert_cpp_type_to_js<nc_string_t>(
    const nc_string_t& arg);

template<>
v8::Local<v8::Value> convert_cpp_type_to_js(const nc_result_t& sql_result);

template<>
v8::Local<v8::Value> convert_cpp_type_to_js(const bool& boolean_value);

template<>
v8::Local<v8::Value> convert_cpp_type_to_js(const nc_null_t&);

}  // namespace NC

#endif /* CPPTOJSCONVERTERS_HH */

