#ifndef DELEGATION_HH
#define DELEGATION_HH


#include "nan.h"
#include "JsTypes.hh"

namespace AODBC {

template<typename ContextT, typename WorkerT>
NAN_METHOD(DelegateWork) {
    v8::Local<v8::Value> arg0 = info[0];
    if (!arg0->IsFunction()) {
        return Nan::ThrowTypeError("Illegal argument type at position 0");
    }
    v8::Local<v8::Function> js_callback = Nan::To<v8::Function>(arg0)
            .ToLocalChecked();

    Nan::AsyncQueueWorker(new WorkerT(
        JsContext<ContextT>::Unwrap(info.This()),
        new Nan::Callback(js_callback)));
}

template<typename ContextT, typename WorkerT, typename Arg0>
NAN_METHOD(DelegateWork) {
    v8::Local<v8::Value> arg0 = info[0];
    if (!JsType<Arg0>::IsValidCppType(arg0)) {
        return Nan::ThrowTypeError("Illegal argument type at position 0");
    }

    v8::Local<v8::Value> arg1 = info[1];
    if (!arg1->IsFunction()) {
        return Nan::ThrowTypeError("Last argument at 1 should be callback");
    }
    v8::Local<v8::Function> js_callback = Nan::To<v8::Function>(arg1)
            .ToLocalChecked();

    Nan::AsyncQueueWorker(new WorkerT(
        JsContext<ContextT>::Unwrap(info.This()),
        JsType<Arg0>::Convert(arg0),
        new Nan::Callback(js_callback)));
}

template<typename ContextT, typename WorkerT, typename Arg0, typename Arg1>
NAN_METHOD(DelegateWork) {
    v8::Local<v8::Value> arg0 = info[0];
    if (!JsType<Arg0>::IsValidCppType(arg0)) {
        return Nan::ThrowTypeError("Illegal argument type at position 0");
    }

    v8::Local<v8::Value> arg1 = info[1];
    if (!JsType<Arg1>::IsValidCppType(arg1)) {
        return Nan::ThrowTypeError("Illegal argument type at position 1");
    }

    v8::Local<v8::Value> arg2 = info[2];
    if (!arg2->IsFunction()) {
        return Nan::ThrowTypeError("Last argument at 2 should be callback");
    }

    v8::Local<v8::Function> js_callback = Nan::To<v8::Function>(arg2)
            .ToLocalChecked();

    Nan::AsyncQueueWorker(new WorkerT(
        JsContext<ContextT>::Unwrap(info.This()),
        JsType<Arg0>::Convert(arg0),
        JsType<Arg1>::Convert(arg1),
        new Nan::Callback(js_callback)));
}

}  // namespace AODBC

#endif /* DELEGATION_HH */

