#ifndef JSTOCPPCONVERTERS_HH
#define JSTOCPPCONVERTERS_HH

#include <boost/optional.hpp>

#include "nan.h"

#include "arguments.hh"

#include "nctypes.hh"

namespace NC {

using NC::QueryArguments;

const char* query_key_name = "query";
const char* batch_size_key_name = "batchSize";
const char* timeout_key_name = "timeout";

const int default_timeout = 0;
const int default_batch_size = 1;


template<typename T>
boost::optional<T> convert_js_type_to_cpp(v8::Local<v8::Value> value);

template<>
boost::optional<nc_string_t>
        convert_js_type_to_cpp<nc_string_t>(v8::Local<v8::Value> local) {
    if (!local->IsString()) {
        return boost::none;
    }
    boost::optional<nc_string_t> result {};
    result.emplace(*Nan::Utf8String(local));
    return result;
}

template<>
boost::optional<nc_long_t>
        convert_js_type_to_cpp<nc_long_t>(v8::Local<v8::Value> local) {
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
        result.emplace(*convert_js_type_to_cpp<nc_string_t>(local));

        return result;
    }

    if (!local->IsObject()) {
        return boost::none;
    }
    auto object = Nan::To<v8::Object>(local).ToLocalChecked();

    Nan::HandleScope scope {};

    auto query_key = Nan::New<v8::String>(query_key_name).ToLocalChecked();
    auto query_prop = Nan::Get(object, query_key).ToLocalChecked();

    auto maybe_query = convert_js_type_to_cpp<nc_string_t>(query_prop);
    if (!maybe_query) {
        return boost::none;
    }

    auto batch_size_key = Nan::New<v8::String>(batch_size_key_name)
        .ToLocalChecked();
    auto batch_size_prop = Nan::Get(object, batch_size_key).ToLocalChecked();
    auto batch_size = convert_js_type_to_cpp<nc_long_t>(batch_size_prop)
        .get_value_or(default_batch_size);

    auto timeout_key = Nan::New<v8::String>(timeout_key_name).ToLocalChecked();
    auto timeout_prop = Nan::Get(object, timeout_key).ToLocalChecked();
    auto timeout = convert_js_type_to_cpp<nc_long_t>(timeout_prop)
        .get_value_or(default_timeout);

    boost::optional<QueryArguments> result {};
    result.emplace(*maybe_query, batch_size, timeout);

    return result;
}

template<>
boost::optional<nc_column_t> convert_js_type_to_cpp<nc_column_t>(
        v8::Local<v8::Value> local) {
    if (auto number = convert_js_type_to_cpp<nc_number_t>(local)) {
        return nc_column_t {std::move(*number)};
    } else if (auto str = convert_js_type_to_cpp<nc_string_t>(local)) {
        return nc_column_t {std::move(*str)};
    } else if (auto blank = convert_js_type_to_cpp<nc_null_t>(local)) {
        return nc_column_t {std::move(*blank)};
    }
     // TODO(kko): add support for other types
    return boost::none;
}

template<>
boost::optional<std::vector<nc_column_t>>
convert_js_type_to_cpp<std::vector<nc_column_t>>(
        v8::Local<v8::Value> local) {
    if (!local->IsArray()) {
        return boost::none;
    }

    auto array = v8::Local<v8::Array>::Cast(local);
    auto len = array->Length();

    std::vector<nc_column_t> result(len);

    // TODO(kko): not sure if this is the best way to prevent compiler warnings
    for (decltype(len) i = 0; i < len; ++i) {
        auto bound_arg {convert_js_type_to_cpp<nc_column_t>(array->Get(i))};
        if (!bound_arg) {
            return boost::none;
        }
        result.push_back(std::move(*bound_arg));
    }
    return boost::make_optional(std::move(result));
}


}  // namespace NC


#endif  /* JSTOCPPCONVERTERS_HH */
