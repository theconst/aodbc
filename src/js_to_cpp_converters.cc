#include "js_to_cpp_converters.hh"

#include <limits>

#include "js_keys.hh"

namespace NC {

using NC::QueryArguments;

template <typename T>
inline boost::optional<T> get_opt(
        v8::Local<v8::Object> object, const char* key) {
    Nan::HandleScope scope {};

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
    return boost::make_optional(Nan::To<nc_long_t>(local).FromJust());
}


template<>
boost::optional<int16_t> convert_js_type_to_cpp(v8::Local<v8::Value> local) {
    if (!local->IsNumber()) {
        return boost::none;
    }
    auto value { Nan::To<int32_t>(local).FromJust() };
    if (value >= std::numeric_limits<int16_t>::max()) {
        return boost::none;
    }
    if (value <= std::numeric_limits<int16_t>::min()) {
        return boost::none;
    }
    return boost::make_optional(static_cast<int16_t>(value));
}

template<>
boost::optional<int32_t> convert_js_type_to_cpp(v8::Local<v8::Value> local) {
    // strict checks are used to prevent implicit conversions
    if (!local->IsNumber()) {
        return boost::none;
    }
    return boost::make_optional(Nan::To<int32_t>(local).FromJust());
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
    auto maybe_object { Nan::To<v8::Object>(local) };
    if (maybe_object.IsEmpty()) {
        return boost::none;
    }
    auto object = maybe_object.ToLocalChecked();

    auto maybe_query = get_opt<nc_string_t>(object, ArgumentKeys::query);

    if (!maybe_query) {
        return boost::none;
    }

    result.emplace(
        QueryStringArg { std::move(*maybe_query) },
        BatchSizeArg { get_opt<nc_long_t>(object, ArgumentKeys::batch_size) },
        TimeoutArg { get_opt<nc_long_t>(object, ArgumentKeys::timeout) });

    return result;
}

template<>
boost::optional<bool> convert_js_type_to_cpp(v8::Local<v8::Value> local) {
    if (local->IsBoolean()) {
        return Nan::To<bool>(local).FromJust();
    }
    return boost::none;
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
boost::optional<TimeoutArg> convert_js_type_to_cpp(v8::Local<v8::Value> local) {
    return TimeoutArg { convert_js_type_to_cpp<nc_long_t>(local) };
}

template<>
boost::optional<nc_number_t> convert_js_type_to_cpp(
        v8::Local<v8::Value> local) {
    if (!local->IsNumber()) {
        return boost::none;
    }
    return nc_number_t { Nan::To<nc_number_t>(local).FromJust() };
}


// use string or the object to execute queries
// TODO(kko): remove this feature
boost::optional<nc_variant_t> convert_js_date_to_cpp(
        v8::Local<v8::Value> local) {
    auto maybe_object { Nan::To<v8::Object>(local) };
    if (maybe_object.IsEmpty()) {
        return boost::none;
    }

    boost::optional<nc_variant_t> result;

    auto object { maybe_object.ToLocalChecked() };

    auto day { get_opt<int16_t>(object, DateKeys::day) };
    auto month { get_opt<int16_t>(object, DateKeys::month) };
    auto year { get_opt<int16_t>(object, DateKeys::year) };
    auto seconds { get_opt<int16_t>(object, DateKeys::seconds) };
    auto minutes { get_opt<int16_t>(object, DateKeys::minutes) };
    auto hours { get_opt<int16_t>(object, DateKeys::hours) };

    bool is_date = day && month && year;
    bool is_time = seconds && minutes && hours;

    if (is_date && is_time) {
        constexpr int32_t default_fractional_seconds = 0;
        auto fract {
            get_opt<int32_t>(object, DateKeys::fractionalSeconds)
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

    if (auto bindings { convert_js_type_to_cpp<nc_bindings_t>(local) }) {
        result.emplace(
            BindingsArg { std::move(*bindings) },
            BatchSizeArg::DefaultValue(),
            TimeoutArg::DefaultValue());

        return result;
    }

    auto maybe_object { Nan::To<v8::Object>(local) };

    if (maybe_object.IsEmpty()) {
        result.emplace(BindingsArg::DefaultValue(),
            BatchSizeArg::DefaultValue(),
            TimeoutArg::DefaultValue());

        return result;
    }

    auto object { maybe_object.ToLocalChecked() };
    auto bindings = get_opt<nc_bindings_t>(object, ArgumentKeys::bindings)
            .value_or(BindingsArg::DefaultValue());

    result.emplace(
        BindingsArg { std::move(bindings) },
        BatchSizeArg { get_opt<nc_long_t>(object, ArgumentKeys::batch_size) },
        TimeoutArg { get_opt<nc_long_t>(object, ArgumentKeys::timeout) });

    return result;
}

}  // namespace NC
