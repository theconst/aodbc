#include <exception>

#include "nan.h"

#include <sql.h>
#include "nanodbc.h"

#include "converters.hh"

namespace AODBC {

    using namespace AODBC;

    struct IsTimeLike {
    };

    struct IsDateLike {
    };

    struct IsTimestampLike {
    };

    void ConvertNanodbcTypeToJsObject(v8::Local<v8::Object> result, const nanodbc::date& date);
    void ConvertNanodbcTypeToJsObject(v8::Local<v8::Object> result, const nanodbc::time& date);
    void ConvertNanodbcTypeToJsObject(v8::Local<v8::Object> result, const nanodbc::timestamp& date);

    struct SQLColumnVisitor : public boost::static_visitor<v8::Local<v8::Value>>
    {

        v8::Local<v8::Value> operator()(const boost::blank & blank) const {
            Nan::EscapableHandleScope handleScope;
            return handleScope.Escape(Nan::Null());
        }

        v8::Local<v8::Value> operator()(double doubleValue) const {
            Nan::EscapableHandleScope handleScope;
            return handleScope.Escape(Nan::New<v8::Number>(doubleValue));
        }

        v8::Local<v8::Value> operator()(const nanodbc::string & str) const {
            Nan::EscapableHandleScope handleScope;
            return handleScope.Escape(Nan::New<v8::String>(str).ToLocalChecked());
        }

        v8::Local<v8::Value> operator()(const binary_t & binary) const {
            //TODO: convert binary type

            throw std::runtime_error("Binary data not implemented yet");
        }

        v8::Local<v8::Value> operator()(const nanodbc::date & t) const {
            Nan::EscapableHandleScope handleScope;
            v8::Local<v8::Object> result = Nan::New<v8::Object>();
            ConvertNanodbcTypeToJsObject(result, t);
            return handleScope.Escape(result);
        }

        v8::Local<v8::Value> operator()(const nanodbc::time & t) const {
            Nan::EscapableHandleScope handleScope;
            v8::Local<v8::Object> result = Nan::New<v8::Object>();
            ConvertNanodbcTypeToJsObject(result, t);
            return handleScope.Escape(result);
        }

        v8::Local<v8::Value> operator()(const nanodbc::timestamp & t) const {
            Nan::EscapableHandleScope handleScope;
            v8::Local<v8::Object> result = Nan::New<v8::Object>();
            ConvertNanodbcTypeToJsObject(result, t);
            return handleScope.Escape(result);
        }

    };

    template<typename T>
    void ConvertNanodbcTypeToJsObject(v8::Local<v8::Object> time_result,
            const T& time, IsTimeLike) {
        Nan::HandleScope scope;

        Nan::Set(time_result,
                Nan::New<v8::String>("hour").ToLocalChecked(),
                Nan::New<v8::Number>(time.hour)
                );

        Nan::Set(time_result,
                Nan::New<v8::String>("minute").ToLocalChecked(),
                Nan::New<v8::Number>(time.min)
                );

        Nan::Set(time_result,
                Nan::New<v8::String>("second").ToLocalChecked(),
                Nan::New<v8::Number>(time.sec)
                );
    }

    template<typename T>
    void ConvertNanodbcTypeToJsObject(v8::Local<v8::Object> date_result,
            const T& date, IsDateLike) {
        Nan::HandleScope scope;

        Nan::Set(date_result,
                Nan::New<v8::String>("day").ToLocalChecked(),
                Nan::New<v8::Number>(date.day)
                );

        Nan::Set(date_result,
                Nan::New<v8::String>("month").ToLocalChecked(),
                Nan::New<v8::Number>(date.month)
                );

        Nan::Set(date_result,
                Nan::New<v8::String>("year").ToLocalChecked(),
                Nan::New<v8::Number>(date.year)
                );
    }

    template<typename T>
    void ConvertNanodbcTypeToJsObject(
            v8::Local<v8::Object> timestamp_result,
            const T& timestamp,
            IsTimestampLike) {
        ConvertNanodbcTypeToJsObject(timestamp_result, timestamp, IsDateLike{});
        ConvertNanodbcTypeToJsObject(timestamp_result, timestamp, IsTimeLike{});
    }

    void ConvertNanodbcTypeToJsObject(v8::Local<v8::Object> result, const nanodbc::date& date) {
        ConvertNanodbcTypeToJsObject(result, date, IsDateLike{});
    }

    void ConvertNanodbcTypeToJsObject(v8::Local<v8::Object> result, const nanodbc::time& date) {
        ConvertNanodbcTypeToJsObject(result, date, IsTimeLike{});
    }

    void ConvertNanodbcTypeToJsObject(v8::Local<v8::Object> result, const nanodbc::timestamp& date) {
        ConvertNanodbcTypeToJsObject(result, date, IsTimestampLike{});
    }

    v8::Local<v8::Array> ConvertNanodbcTypeToJsObject(const sql_result_t& sql_result) {
        Nan::EscapableHandleScope scope;
        SQLColumnVisitor visitor;

        v8::Local<v8::Array> js_result = Nan::New<v8::Array>(sql_result.size());
        int c = 0;
        for (const auto& row : sql_result) {
            v8::Local<v8::Object> result_row = Nan::New<v8::Object>();
            for (const auto& column : row) {
                const nanodbc::string& col_name = column.first;
                Nan::Set(result_row,
                        Nan::New<v8::String>(col_name).ToLocalChecked(),
                        column.second.apply_visitor(visitor)
                        );
            }
            js_result->Set(c++, result_row);
        }
        return scope.Escape(js_result);
    }

}