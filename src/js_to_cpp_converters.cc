#include "js_to_cpp_converters.hh"

namespace NC {

using NC::QueryArguments;

const char* bindings_key_name = "bindings";

const char* query_key_name = "query";

const char* batch_size_key_name = "batchSize";
const char* timeout_key_name = "timeout";

const std::int16_t default_fractional_seconds = 0;

template <typename T>
inline boost::optional<T> get_opt(
        v8::Local<v8::Object> object, const char* key) {
    Nan::HandleScope scope {};

    // TODO(kko): add constant pool
    auto key_prop = Nan::New<v8::String>(key).ToLocalChecked();

    auto maybe_prop = Nan::Get(object, key_prop);

    if (maybe_prop.IsEmpty()) {
        return boost::none;
    }

    return convert_js_type_to_cpp<T>(maybe_prop.ToLocalChecked());
}

template<>
boost::optional<nc_string_t> convert_js_type_to_cpp(
        v8::Local<v8::Value> local) {
    if (!local->IsString()) {
        return boost::none;
    }
    boost::optional<nc_string_t> result {};
    result.emplace(*Nan::Utf8String(local));
    return result;
}

template<>
boost::optional<nc_long_t> convert_js_type_to_cpp(v8::Local<v8::Value> local) {
    if (!local->IsNumber()) {
        return boost::none;
    }
    return boost::make_optional(local->IntegerValue());
}

template<>
boost::optional<std::int16_t> convert_js_type_to_cpp(
        v8::Local<v8::Value> local) {
    auto n = convert_js_type_to_cpp<nc_long_t>(local);

    // TODO(kko): make optional util with map function
    if (n) {
        return static_cast<std::int16_t>(*n);
    }
    return boost::none;
}

template<>
boost::optional<QueryArguments> convert_js_type_to_cpp(
        v8::Local<v8::Value> local) {
    boost::optional<QueryArguments> result {};
    if (auto query_string { convert_js_type_to_cpp<nc_string_t>(local) }) {
        result.emplace(
            QueryStringArg { std::move(*query_string) },
            BatchSizeArg::DefaultValue(),
            TimeoutArg::DefaultValue());

        return result;
    }

    if (!local->IsObject()) {
        return boost::none;
    }
    auto object = Nan::To<v8::Object>(local).ToLocalChecked();

    auto maybe_query = get_opt<nc_string_t>(object, query_key_name);

    if (!maybe_query) {
        return boost::none;
    }

    result.emplace(
        QueryStringArg { std::move(*maybe_query) },
        BatchSizeArg { get_opt<nc_long_t>(object, batch_size_key_name) },
        TimeoutArg { get_opt<nc_long_t>(object, timeout_key_name) });

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
boost::optional<nc_number_t> convert_js_type_to_cpp(
        v8::Local<v8::Value> local) {
    if (local->IsNumber()) {
        return nc_number_t {local->NumberValue()};
    }
    return boost::none;
}

boost::optional<nc_variant_t> convert_js_date_to_cpp(
        v8::Local<v8::Value> local) {
    if (!local->IsObject()) {
        return boost::none;
    }

    boost::optional<nc_variant_t> result;

    auto object = v8::Local<v8::Object>::Cast(local);

    // TODO(kko): move common constants from cpp_to_js_converters
    // TODO(kko): remove hardcoded values

    auto day { get_opt<std::int16_t>(object, "day") };
    auto month { get_opt<std::int16_t>(object, "month") };
    auto year { get_opt<std::int16_t>(object, "year") };
    auto seconds { get_opt<std::int16_t>(object, "seconds") };
    auto minutes { get_opt<std::int16_t>(object, "minutes") };
    auto hours { get_opt<std::int16_t>(object, "hours") };

    bool is_date = day && month && year;
    bool is_time = seconds && minutes && hours;

    if (is_date && is_time) {
        auto fract { get_opt<std::int16_t>(object, "fractionalSeconds")
            .value_or(default_fractional_seconds) };
        nc_timestamp_t ts = { *year, *month, *day, *hours,
            *minutes, *seconds, fract };
        result.emplace(ts);
    }

    if (is_date) {
        nc_date_t date = { *year, *month, *day };
        result.emplace(date);
    }

    if (is_time) {
        nc_time_t time = { *hours, *minutes, *seconds };
        result.emplace(time);
    }

    return result;
}

template<>
boost::optional<nc_variant_t> convert_js_type_to_cpp(
        v8::Local<v8::Value> local) {
    boost::optional<nc_variant_t> result {};

    // TODO(kko): recurse over type list ?
    if (auto number = convert_js_type_to_cpp<nc_number_t>(local)) {
        result.emplace(*number);
    } else if (auto str = convert_js_type_to_cpp<nc_string_t>(local)) {
        result.emplace(std::move(*str));
    } else if (auto blank = convert_js_type_to_cpp<nc_null_t>(local)) {
        result.emplace(*blank);
    } else if (auto date = convert_js_date_to_cpp(local)) {
        result.emplace(std::move(*date));
    }

    return result;
}

template<>
boost::optional<nc_bindings_t> convert_js_type_to_cpp(
        v8::Local<v8::Value> local) {
    if (!local->IsArray()) {
        return boost::none;
    }

    auto array = v8::Local<v8::Array>::Cast(local);
    int len = array->Length();

    nc_bindings_t result {};
    result.reserve(len);

    for (int i = 0; i < len; ++i) {
        boost::optional<nc_variant_t> bound_arg {
            convert_js_type_to_cpp<nc_variant_t>(array->Get(i))};
        if (!bound_arg) {
            return boost::none;
        }
        result.emplace_back(std::move(*bound_arg));
    }
    boost::optional<nc_bindings_t> definitely_variant {};
    definitely_variant.emplace(std::move(result));
    return definitely_variant;
}

template<>
boost::optional<PreparedStatementArguments> convert_js_type_to_cpp(
        v8::Local<v8::Value> local) {
    boost::optional<PreparedStatementArguments> result {};

    auto bindings { convert_js_type_to_cpp<nc_bindings_t>(local) };
    if (bindings) {
        result.emplace(
            BindingsArg { std::move(*bindings) },
            BatchSizeArg::DefaultValue(),
            TimeoutArg::DefaultValue());
        return result;
    }

    // TODO(kko): it will treat all jiberish as defaults
    if (!local->IsObject()) {
        result.emplace(BindingsArg::DefaultValue(),
            BatchSizeArg::DefaultValue(),
            TimeoutArg::DefaultValue());
    } else {
        auto object = Nan::To<v8::Object>(local).ToLocalChecked();
        auto bindings =
            get_opt<nc_bindings_t>(object, bindings_key_name)
            .value_or(BindingsArg::DefaultValue());

        result.emplace(
            BindingsArg { std::move(bindings) },
            BatchSizeArg { get_opt<nc_long_t>(object, timeout_key_name) },
            TimeoutArg { get_opt<nc_long_t>(object, batch_size_key_name) });
    }

    return result;
}

}  // namespace NC
