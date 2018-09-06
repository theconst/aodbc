#ifndef JSTOCPPCONVERTERS_HH
#define JSTOCPPCONVERTERS_HH

#include "nan.h"

namespace AODBC {

template<typename T>
T unwrap_caller(v8::Local<v8::Object>);

template<typename T>
T convert_js_type_to_cpp(v8::Local<v8::Value> value);

template<typename T>
bool is_valid_cpp_type(v8::Local<v8::Value> value);

template<>
std::string convert_js_type_to_cpp<std::string>(v8::Local<v8::Value> local) {
    return std::string(*v8::String::Utf8Value(local->ToString()));
}

template<>
bool is_valid_cpp_type<std::string>(v8::Local<v8::Value> local) {
    return local->IsString();
}


template<>
long convert_js_type_to_cpp<long>  // NOLINT(runtime/int)
            (v8::Local<v8::Value> local) {
    return local->IntegerValue();
}

template<>
bool is_valid_cpp_type<long>  // NOLINT(runtime/int)
        (v8::Local<v8::Value> local) {
    return local->IsNumber();
}

}  // namespace AODBC



#endif  /* JSTOCPPCONVERTERS_HH */
