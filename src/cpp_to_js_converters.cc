#include "cpp_to_js_converters.hh"

#include <cstdint>
#include <ctime>

#include "nan.h"

#include "js_keys.hh"

namespace NC {

namespace {

typedef struct std::tm TM;

template<typename T>
struct TimeConverter {
    static void Convert(TM* time_result, const T& time) {
        time_result->tm_hour = time.hour;
        time_result->tm_min = time.min;
        time_result->tm_sec = time.sec;

        time_result->tm_isdst = -1;
    }
};

template<typename T>
struct DateConverter {
    static void Convert(TM* date_result, const T& date) {
        static constexpr const int16_t start_year = 1900;
        static constexpr const int16_t start_month = 1;

        date_result->tm_mday = date.day;
        date_result->tm_mon = date.month - start_month;
        date_result->tm_year = date.year - start_year;
    }
};

inline void convert_cpp_type_to_js(TM* res, const nc_date_t& date) {
    DateConverter<nc_date_t>::Convert(res, date);
}

inline void convert_cpp_type_to_js(TM* res, const nc_time_t& time) {
    TimeConverter<nc_time_t>::Convert(res, time);
}

inline void convert_cpp_type_to_js(TM* res, const nc_timestamp_t& ts) {
    DateConverter<nc_timestamp_t>::Convert(res, ts);
    TimeConverter<nc_timestamp_t>::Convert(res, ts);
}

template <typename T>
struct Fraction {
    static inline int16_t Get(const T&) {
        return 0;
    }
};

template<>
struct Fraction<nc_timestamp_t> {
    static inline int16_t Get(const nc_timestamp_t& ts) {
        return ts.fract;
    }
};

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

    template<typename T>
    v8::Local<v8::Value> operator()(const T& t) const {
        static constexpr const int millis_in_seconds = 1000;

        TM time {};
        convert_cpp_type_to_js(&time, t);

        std::time_t ts = std::mktime(&time);
        nc_number_t millis = ts * millis_in_seconds;
        nc_number_t total = millis + Fraction<T>::Get(t);

        Nan::EscapableHandleScope scope {};
        auto result { Nan::New<v8::Date>(total).ToLocalChecked() };
        return scope.Escape(result);
    }
};


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
