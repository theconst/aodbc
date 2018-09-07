#ifndef JSTOCPPCONVERTERS_HH
#define JSTOCPPCONVERTERS_HH

#include <boost/optional.hpp>

#include "nan.h"

#include "sqltypes.hh"

namespace AODBC {

// used instead of optional
template<typename T>
boost::optional<T> convert_js_type_to_cpp(v8::Local<v8::Value> value);

template<>
boost::optional<sql_string_t>
        convert_js_type_to_cpp<sql_string_t>(v8::Local<v8::Value> local) {
    if (!local->IsString()) {
        return boost::none;
    }
    return boost::make_optional(
        sql_string_t {*v8::String::Utf8Value(local->ToString())});
}

template<>
boost::optional<sql_long_t>
        convert_js_type_to_cpp<sql_long_t>(v8::Local<v8::Value> local) {
    if (!local->IsNumber()) {
        return boost::none;
    }
    return boost::make_optional(local->IntegerValue());
}


}  // namespace AODBC


#endif  /* JSTOCPPCONVERTERS_HH */
