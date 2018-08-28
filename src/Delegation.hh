/* 
 * File:   Delegation.hh
 * Author: kko
 *
 * Created on August 28, 2018, 12:57 AM
 */

//TODO : generalize to more than 2 args??
//TODO : be less restrictive? this is javascript not python
// make undefined not war


//TODO: wrap my mind around variadic templates
//TODO: manage string constant


#ifndef DELEGATION
#define DELEGATION


#include "nan.h"


#include <iostream>

namespace AODBC {
    
using namespace AODBC;

// unwraps this
template<typename T>
struct Context {
    inline static T Unwrap(const v8::Local<v8::Object>&);
};

// convert and validate common js types
template<typename T>
struct JsToCpp {
    inline static T Convert(const v8::Local<v8::Value>& local);

    inline static bool IsValidCppType(const v8::Local<v8::Value>& local);
};

template<>
struct JsToCpp<std::string> {

    inline static std::string Convert(const v8::Local<v8::Value>& local) {
        return std::string(*v8::String::Utf8Value(local->ToString()));
    }

    inline static bool IsValidCppType(const v8::Local<v8::Value>& local) {
        return local->IsString();
    }
};

template<>
struct JsToCpp<long> {
    
    inline static long Convert(const v8::Local<v8::Value>& local) {
        return local->IntegerValue();
    }
    
    inline static bool IsValidCppType(const v8::Local<v8::Value>& local) {
        bool is = local->IsNumber();
        if (is) {
            std::cout << "TRUE";
        } else {
            std::cout << "FALSE";
        }
        return is;
    }
};

//TODO: should I handle callbacks in the same fashion ?

template<typename ContextT, typename WorkerT>
NAN_METHOD(DelegateWork) {
    v8::Local<v8::Value> arg0 = info[0];
    if (!arg0->IsFunction()) {
        return Nan::ThrowTypeError("Illegal argument type at position 0");
    }
    v8::Local<v8::Function> js_callback = Nan::To<v8::Function>(arg0)
            .ToLocalChecked();

    Nan::AsyncQueueWorker(new WorkerT(
            Context<ContextT>::Unwrap(info.This()),
            new Nan::Callback(js_callback)
        )
    );
}

template<typename ContextT, typename WorkerT, typename Arg0>
NAN_METHOD(DelegateWork) {
    v8::Local<v8::Value> arg0 = info[0];
    if (!JsToCpp<Arg0>::IsValidCppType(arg0)) {
        return Nan::ThrowTypeError("Illegal argument type at position 0");
    }

    v8::Local<v8::Value> arg1 = info[1];
    if (!arg1->IsFunction()) {
        return Nan::ThrowTypeError("Last argument at 1 should be callback");
    }
    v8::Local<v8::Function> js_callback = Nan::To<v8::Function>(arg1)
            .ToLocalChecked();

    Nan::AsyncQueueWorker(new WorkerT(
        Context<ContextT>::Unwrap(info.This()),
        JsToCpp<Arg0>::Convert(arg0),
        new Nan::Callback(js_callback))
    );
}

template<typename ContextT, typename WorkerT, typename Arg0, typename Arg1>
NAN_METHOD(DelegateWork) {
    v8::Local<v8::Value> arg0 = info[0];
    if (!JsToCpp<Arg0>::IsValidCppType(arg0)) {
        return Nan::ThrowTypeError("Illegal argument type at position 0");
    }

    v8::Local<v8::Value> arg1 = info[1];
    if (!JsToCpp<Arg1>::IsValidCppType(arg1)) {
        return Nan::ThrowTypeError("Illegal argument type at position 1");
    }

    v8::Local<v8::Value> arg2 = info[2];
    if (!arg2->IsFunction()) {
        return Nan::ThrowTypeError("Last argument at 2 should be callback");
    }
    
    v8::Local<v8::Function> js_callback = Nan::To<v8::Function>(arg2)
            .ToLocalChecked();

    Nan::AsyncQueueWorker(new WorkerT(
            Context<ContextT>::Unwrap(info.This()),
            JsToCpp<Arg0>::Convert(arg0),
            JsToCpp<Arg1>::Convert(arg1),
            new Nan::Callback(js_callback)
        )
    );
}

}

#endif

