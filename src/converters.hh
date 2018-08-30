
/* 
 * File:   converters.hh
 * Author: kko
 * 
 * 
 * Converters for common nanodbc types
 * 
 * Javascript parameters are passed by reference, ie. handle should be created
 * Really do not see benefit of implementing other than this will repeat
 * nan factory
 * Created on August 29, 2018, 10:57 PM
 */

#ifndef CONVERTERS_HH
#define CONVERTERS_HH

#include <exception>

#include "nan.h"
#include "nanodbc.h"


namespace AODBC {
    
    using namespace AODBC;
    
    struct IsTimeLike {};
    
    struct IsDateLike {};
    
    struct IsTimestampLike {};
    
    
    void ConvertNanodbcTypeToJsObject(v8::Local<v8::Object> result, const nanodbc::date& date);
    void ConvertNanodbcTypeToJsObject(v8::Local<v8::Object> result, const nanodbc::time& date);
    void ConvertNanodbcTypeToJsObject(v8::Local<v8::Object> result, const nanodbc::timestamp& date);


    //TODO: move this to another

    
    //TODO: generify
    struct SQLColumnVisitor : public boost::static_visitor<v8::Local<v8::Value>>
    {

        v8::Local<v8::Value> operator()(const boost::blank& blank) const {
            Nan::EscapableHandleScope handleScope;
            return handleScope.Escape(Nan::Null());
        }
        
        v8::Local<v8::Value> operator()(double doubleValue) const {
            Nan::EscapableHandleScope handleScope;
            return handleScope.Escape(Nan::New<v8::Number>(doubleValue));
        }

        v8::Local<v8::Value> operator()(const nanodbc::string& str) const {
            Nan::EscapableHandleScope handleScope;
            return handleScope.Escape(Nan::New<v8::String>(str).ToLocalChecked());
        }
        
        v8::Local<v8::Value> operator()(const binary_t& binary) const {
//            Nan::EscapableHandleScope handleScope;
//            
//            v8::Local<v8::Object> value = Nan::NewBuffer().ToLocalChecked();
//            
            //some byte manipulation will be done here, so special care should be taken
//            
//            binary.data();
            
            throw std::runtime_error("Binary data not implemented yet");
            
//            return handleScope.Escape()
        }
        
        v8::Local<v8::Value> operator()(const nanodbc::date& t) const {
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
        ConvertNanodbcTypeToJsObject(timestamp_result, timestamp, IsDateLike {});
        ConvertNanodbcTypeToJsObject(timestamp_result, timestamp, IsTimeLike {});
    }
    
    void ConvertNanodbcTypeToJsObject(v8::Local<v8::Object> result, const nanodbc::date& date) {
        ConvertNanodbcTypeToJsObject(result, date, IsDateLike {});
    }

    void ConvertNanodbcTypeToJsObject(v8::Local<v8::Object> result, const nanodbc::time& date) {
        ConvertNanodbcTypeToJsObject(result, date, IsTimeLike {});
    }

    void ConvertNanodbcTypeToJsObject(v8::Local<v8::Object> result, const nanodbc::timestamp& date) {
        ConvertNanodbcTypeToJsObject(result, date, IsTimestampLike {});
    }

}

#endif /* CONVERTERS_HH */

