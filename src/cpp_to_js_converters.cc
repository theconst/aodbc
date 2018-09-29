#include "cpp_to_js_converters.hh"

#include <cstdint>

#include "nan.h"

#include "js_keys.hh"

namespace NC {

namespace {

void convert_cpp_type_to_js(
    v8::Local<v8::Object> result,
    const nc_date_t& date);
void convert_cpp_type_to_js(
    v8::Local<v8::Object> result,
    const nc_time_t& date);
void convert_cpp_type_to_js(
    v8::Local<v8::Object> result,
    const nc_timestamp_t& date);

struct SQLColumnVisitor : public boost::static_visitor<v8::Local<v8::Value>> {

    v8::Local<v8::Value> operator()(const nc_null_t& blank) const {
        Nan::EscapableHandleScope scope {};
        return scope.Escape(Nan::Null());
    }

    v8::Local<v8::Value> operator()(const nc_number_t& doubleValue) const {
        Nan::EscapableHandleScope scope {};
        return scope.Escape(Nan::New<v8::Number>(doubleValue));
    }

    v8::Local<v8::Value> operator()(const nc_long_t& longValue) const {
        Nan::EscapableHandleScope scope {};
        return scope.Escape(Nan::New<v8::Number>(longValue));
    }

    v8::Local<v8::Value> operator()(const nc_string_t& str) const {
        Nan::EscapableHandleScope scope {};
        return scope.Escape(
            Nan::New<v8::String>(str).ToLocalChecked());
    }

    v8::Local<v8::Value> operator()(const nc_binary_t& binary) const {
        Nan::EscapableHandleScope scope {};

        std::size_t raw_size = binary.size() * sizeof(nc_binary_t::value_type);

        // node::Encode copies the buffer and hands it off to GC
        return scope.Escape(
            Nan::Encode(binary.data(), raw_size, Nan::Encoding::BINARY));
    }

    template <typename T>
    v8::Local<v8::Value> operator()(const T& t) const {
        Nan::EscapableHandleScope scope {};
        v8::Local<v8::Object> result = Nan::New<v8::Object>();
        convert_cpp_type_to_js(result, t);
        return scope.Escape(result);
    }
};

template<typename T>
void convert_cpp_type_to_js(
        v8::Local<v8::Object> time_result,
        const T& time,
        DateTag<DateTypes::time>) {
    Nan::HandleScope scope {};

    Nan::Set(time_result,
        Nan::New<v8::String>(DateKeys::hours).ToLocalChecked(),
        Nan::New<v8::Number>(time.hour));

    Nan::Set(time_result,
        Nan::New<v8::String>(DateKeys::minutes).ToLocalChecked(),
        Nan::New<v8::Number>(time.min));

    Nan::Set(time_result,
        Nan::New<v8::String>(DateKeys::seconds).ToLocalChecked(),
        Nan::New<v8::Number>(time.sec));
}

template<typename T>
void convert_cpp_type_to_js(
        v8::Local<v8::Object> date_result,
        const T& date,
        DateTag<DateTypes::date>) {
    Nan::HandleScope scope {};

    Nan::Set(date_result,
        Nan::New<v8::String>(DateKeys::day).ToLocalChecked(),
        Nan::New<v8::Number>(date.day));

    Nan::Set(date_result,
        Nan::New<v8::String>(DateKeys::month).ToLocalChecked(),
        // TODO(kko): should I index from 0 like in javascript
        Nan::New<v8::Number>(date.month));

    Nan::Set(date_result,
        Nan::New<v8::String>(DateKeys::year).ToLocalChecked(),
        Nan::New<v8::Number>(date.year));
}

template<typename T>
void convert_cpp_type_to_js(
        v8::Local<v8::Object> timestamp_result,
        const T& timestamp,
        DateTag<DateTypes::datetime>) {
    convert_cpp_type_to_js(timestamp_result, timestamp,
        DateTag<DateTypes::date>{});
    convert_cpp_type_to_js(timestamp_result, timestamp,
        DateTag<DateTypes::time>{});
    Nan::Set(timestamp_result,
        Nan::New<v8::String>(DateKeys::fractionalSeconds).ToLocalChecked(),
        Nan::New<v8::Number>(timestamp.fract));
}

void convert_cpp_type_to_js(
        v8::Local<v8::Object> result,
        const nc_date_t& date) {
    convert_cpp_type_to_js(result, date, DateTag<DateTypes::date>{});
}

void convert_cpp_type_to_js(
        v8::Local<v8::Object> result,
        const nc_time_t& date) {
    convert_cpp_type_to_js(result, date, DateTag<DateTypes::time>{});
}

void convert_cpp_type_to_js(
        v8::Local<v8::Object> result,
        const nc_timestamp_t& date) {
    convert_cpp_type_to_js(result, date, DateTag<DateTypes::datetime>{});
}

}  // namespace

v8::Local<v8::Value> convert_cpp_type_to_js(const nc_string_t& arg) {
    Nan::EscapableHandleScope scope {};
    return scope.Escape(Nan::New<v8::String>(arg).ToLocalChecked());
}

v8::Local<v8::Value> convert_cpp_type_to_js(const nc_result_t& sql_result) {
    Nan::EscapableHandleScope scope {};
    SQLColumnVisitor visitor {};

    auto js_result = Nan::New<v8::Array>(sql_result.size());
    uint32_t c = 0U;
    for (const auto& row : sql_result) {
        v8::Local<v8::Object> result_row = Nan::New<v8::Object>();
        for (const auto& column : row) {
            const nanodbc::string& col_name = column.first;
            Nan::Set(result_row,
                Nan::New<v8::String>(col_name).ToLocalChecked(),
                column.second.apply_visitor(visitor));
        }
        js_result->Set(c++, result_row);
    }
    return scope.Escape(js_result);
}

v8::Local<v8::Value> convert_cpp_type_to_js(bool value) {
    Nan::EscapableHandleScope scope {};
    return scope.Escape(Nan::New<v8::Boolean>(value));
}

v8::Local<v8::Value> convert_cpp_type_to_js(nc_null_t) {
    Nan::EscapableHandleScope scope {};
    return scope.Escape(Nan::Null());
}

}  // namespace NC
