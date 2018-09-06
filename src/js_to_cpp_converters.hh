#ifndef JSTOCPPCONVERTERS_HH
#define JSTOCPPCONVERTERS_HH

#include "nan.h"

#include "sqltypes.hh"

namespace AODBC {

template<typename T>
T unwrap_caller(v8::Local<v8::Object>);

template<typename T>
T convert_js_type_to_cpp(v8::Local<v8::Value> value);

template<typename T>
bool is_valid_cpp_type(v8::Local<v8::Value> value);

template<>
sql_string_t convert_js_type_to_cpp<std::string>(v8::Local<v8::Value> local) {
    return std::string(*v8::String::Utf8Value(local->ToString()));
}

template<>
bool is_valid_cpp_type<std::string>(v8::Local<v8::Value> local) {
    return local->IsString();
}


template<>
sql_long_t convert_js_type_to_cpp<sql_long_t>(v8::Local<v8::Value> local) {
    return local->IntegerValue();
}

template<>
bool is_valid_cpp_type<sql_long_t>(v8::Local<v8::Value> local) {
    return local->IsNumber();
}

}  // namespace AODBC



#endif  /* JSTOCPPCONVERTERS_HH */
