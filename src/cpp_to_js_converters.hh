#ifndef CPPTOJSCONVERTERS_HH
#define CPPTOJSCONVERTERS_HH

#include "nan.h"

#include <exception>

#include "nan.h"

#include <sql.h>
#include "nanodbc.h"

#include "nctypes.hh"

namespace NC {

template <typename T>
v8::Local<v8::Value> convert_cpp_type_to_js(const T& arg);


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
    // primitive types are optimized, so the code is somewhat duplicated

    v8::Local<v8::Value> operator()(const nc_null_t& blank) const {
        Nan::EscapableHandleScope scope {};
        return scope.Escape(Nan::Null());
    }

    v8::Local<v8::Value> operator()(const nc_number_t& doubleValue) const {
        Nan::EscapableHandleScope scope {};
        return scope.Escape(Nan::New<v8::Number>(doubleValue));
    }

    v8::Local<v8::Value> operator()(const nc_string_t& str) const {
        Nan::EscapableHandleScope scope {};
        return scope.Escape(
            Nan::New<v8::String>(str).ToLocalChecked());
    }

    v8::Local<v8::Value> operator()(const nc_binary_t & binary) const {
        // TODO(kko): convert binary type

        throw std::runtime_error("Binary data not implemented yet");
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
        Nan::New<v8::String>("hour").ToLocalChecked(),
        Nan::New<v8::Number>(time.hour));

    Nan::Set(time_result,
        Nan::New<v8::String>("minute").ToLocalChecked(),
        Nan::New<v8::Number>(time.min));

    Nan::Set(time_result,
        Nan::New<v8::String>("second").ToLocalChecked(),
        Nan::New<v8::Number>(time.sec));
}

template<typename T>
void convert_cpp_type_to_js(
        v8::Local<v8::Object> date_result,
        const T& date,
        DateTag<DateTypes::date>) {
    Nan::HandleScope scope {};

    Nan::Set(date_result,
        Nan::New<v8::String>("day").ToLocalChecked(),
        Nan::New<v8::Number>(date.day));

    Nan::Set(date_result,
        Nan::New<v8::String>("month").ToLocalChecked(),
        Nan::New<v8::Number>(date.month));

    Nan::Set(date_result,
        Nan::New<v8::String>("year").ToLocalChecked(),
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

template<>
v8::Local<v8::Value> convert_cpp_type_to_js<nanodbc::string>(
        const nc_string_t& arg) {
    Nan::EscapableHandleScope scope {};
    return scope.Escape(Nan::New<v8::String>(arg).ToLocalChecked());
}

template<>
v8::Local<v8::Value> convert_cpp_type_to_js(const nc_result_t& sql_result) {
    Nan::EscapableHandleScope scope {};
    SQLColumnVisitor visitor {};

    auto js_result = Nan::New<v8::Array>(sql_result.size());
    int c = 0;
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

template<>
v8::Local<v8::Value> convert_cpp_type_to_js(const bool& boolean_value) {
    Nan::EscapableHandleScope scope {};
    return scope.Escape(Nan::New<v8::Boolean>(boolean_value));
}

template<>
v8::Local<v8::Value> convert_cpp_type_to_js(const nc_null_t&) {
    Nan::EscapableHandleScope scope {};
    return scope.Escape(Nan::Null());
}

}  // namespace NC

#endif /* CPPTOJSCONVERTERS_HH */

