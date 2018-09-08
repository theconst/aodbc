#ifndef JSTOCPPCONVERTERS_HH
#define JSTOCPPCONVERTERS_HH

#include <boost/optional.hpp>

#include "nan.h"

#include "arguments.hh"

#include "sqltypes.hh"

namespace AODBC {

using AODBC::QueryArguments;

const char* query_key_name = "query";
const char* batch_size_key_name = "batchSize";
const char* timeout_key_name = "timeout";


template<typename T>
boost::optional<T> convert_js_type_to_cpp(v8::Local<v8::Value> value);

template<>
boost::optional<sql_string_t>
        convert_js_type_to_cpp<sql_string_t>(v8::Local<v8::Value> local) {
    if (!local->IsString()) {
        return boost::none;
    }
    boost::optional<sql_string_t> result {};
    result.emplace(*v8::String::Utf8Value(local->ToString()));
    return result;
}

template<>
boost::optional<sql_long_t>
        convert_js_type_to_cpp<sql_long_t>(v8::Local<v8::Value> local) {
    if (!local->IsNumber()) {
        return boost::none;
    }
    return boost::make_optional(local->IntegerValue());
}

template<>
boost::optional<QueryArguments> convert_js_type_to_cpp<QueryArguments>(
        v8::Local<v8::Value> local) {
    if (local->IsString()) {
        boost::optional<QueryArguments> result {};
        result.emplace(*convert_js_type_to_cpp<sql_string_t>(local));

        return result;
    }

    if (!local->IsObject()) {
        return boost::none;
    }
    auto object = local->ToObject();

    Nan::HandleScope scope {};

    auto query_key = Nan::New<v8::String>(query_key_name).ToLocalChecked();
    auto query_prop = Nan::Get(object, query_key).ToLocalChecked();

    auto maybe_query = convert_js_type_to_cpp<sql_string_t>(query_prop);
    if (!maybe_query) {
        return boost::none;
    }

    auto batch_size_key = Nan::New<v8::String>(batch_size_key_name)
        .ToLocalChecked();
    auto batch_size_prop = Nan::Get(object, batch_size_key).ToLocalChecked();
    auto batch_size = convert_js_type_to_cpp<sql_long_t>(batch_size_prop)
        .get_value_or(1);

    auto timeout_key = Nan::New<v8::String>(timeout_key_name).ToLocalChecked();
    auto timeout_prop = Nan::Get(object, timeout_key).ToLocalChecked();
    auto timeout = convert_js_type_to_cpp<sql_long_t>(timeout_prop)
        .get_value_or(0);

    boost::optional<QueryArguments> result {};
    result.emplace(*maybe_query, batch_size, timeout);

    return result;
}


}  // namespace AODBC


#endif  /* JSTOCPPCONVERTERS_HH */
