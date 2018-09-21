#include "js_to_cpp_converters.hh"

namespace NC {

using NC::QueryArguments;

const char* bindings_key_name = "bindings";

const char* query_key_name = "query";

const char* batch_size_key_name = "batchSize";
const char* timeout_key_name = "timeout";

const int default_timeout = 0;
const int default_batch_size = 1;

// TODO(kko): add constant pool
// DO NOT USE WITHOUT HANDLE SCOPE
nc_long_t get_batch_size(v8::Local<v8::Object> object) {
    auto batch_size_key = Nan::New<v8::String>(batch_size_key_name)
        .ToLocalChecked();
    auto batch_size_prop = Nan::Get(object, batch_size_key).ToLocalChecked();
    return convert_js_type_to_cpp<nc_long_t>(batch_size_prop)
        .get_value_or(default_batch_size);
}

// DO NOT USE WITHOUT HANDLE SCOPE
nc_long_t get_timeout(v8::Local<v8::Object> object) {
    auto timeout_key = Nan::New<v8::String>(timeout_key_name).ToLocalChecked();
    auto timeout_prop = Nan::Get(object, timeout_key).ToLocalChecked();
    return convert_js_type_to_cpp<nc_long_t>(timeout_prop)
        .get_value_or(default_timeout);
}

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
    boost::optional<QueryArguments> result {};
    if (local->IsString()) {
        result.emplace(std::move(*convert_js_type_to_cpp<nc_string_t>(local)));

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

    result.emplace(
        std::move(*maybe_query),
        get_batch_size(object),
        get_timeout(object));

    return result;
}


template<>
boost::optional<nc_null_t>
        convert_js_type_to_cpp<nc_null_t>(v8::Local<v8::Value> local) {
    if (local->IsNull() || local->IsUndefined()) {
        return nc_null_t {};
    }
    return boost::none;
}

template<>
boost::optional<nc_number_t>
        convert_js_type_to_cpp<nc_number_t>(v8::Local<v8::Value> local) {
    if (local->IsNumber()) {
        return nc_number_t {local->NumberValue()};
    }
    return boost::none;
}

template<>
boost::optional<nc_variant_t> convert_js_type_to_cpp<nc_variant_t>(
        v8::Local<v8::Value> local) {
    boost::optional<nc_variant_t> result {};
    if (auto number = convert_js_type_to_cpp<nc_number_t>(local)) {
        result.emplace(*number);
    } else if (auto str = convert_js_type_to_cpp<nc_string_t>(local)) {
        result.emplace(std::move(*str));
    } else if (auto blank = convert_js_type_to_cpp<nc_null_t>(local)) {
        result.emplace(*blank);
    }
     // TODO(kko): add support for other types
    return result;
}

template<>
boost::optional<std::vector<nc_variant_t>>
convert_js_type_to_cpp<std::vector<nc_variant_t>>(
        v8::Local<v8::Value> local) {
    if (!local->IsArray()) {
        return boost::none;
    }

    auto array = v8::Local<v8::Array>::Cast(local);
    int len = array->Length();

    std::vector<nc_variant_t> result {};
    result.reserve(len);

    for (int i = 0; i < len; ++i) {
        boost::optional<nc_variant_t> bound_arg {
            convert_js_type_to_cpp<nc_variant_t>(array->Get(i))};
        if (!bound_arg) {
            return boost::none;
        }
        result.emplace_back(std::move(*bound_arg));
    }
    boost::optional<std::vector<nc_variant_t>> definitely_variant {};
    definitely_variant.emplace(std::move(result));
    return definitely_variant;
}

// TODO(kko): remove copy-pasted code
template<>
boost::optional<PreparedStatementArguments> convert_js_type_to_cpp<
        PreparedStatementArguments>(v8::Local<v8::Value> local) {
    boost::optional<PreparedStatementArguments> result {};
    if (local->IsArray()) {
        result.emplace(std::move(
            *convert_js_type_to_cpp<std::vector<nc_variant_t>>(local)));

        return result;
    }

    if (!local->IsObject()) {
        return boost::none;
    }
    auto object = Nan::To<v8::Object>(local).ToLocalChecked();

    Nan::HandleScope scope {};

    auto bindings_key =
        Nan::New<v8::String>(bindings_key_name).ToLocalChecked();
    auto bindings_args = Nan::Get(object, bindings_key).ToLocalChecked();
    auto bindings =
        convert_js_type_to_cpp<std::vector<nc_variant_t>>(bindings_args)
        .value_or(std::vector<nc_variant_t>(0));

    result.emplace(
        std::move(bindings),
        get_batch_size(object),
        get_timeout(object));

    return result;
}

}  // namespace NC
