#ifndef DELEGATION_HH
#define DELEGATION_HH


#include "nan.h"
#include "JsTypes.hh"

#include "SingleResultWorker.hh"

namespace AODBC {

using AODBC::SingleResultWorker;

template<typename ContextT, typename MethodT, typename ResultT>
NAN_METHOD(DelegateWork) {
    v8::Local<v8::Value> arg0 = info[0];
    if (!arg0->IsFunction()) {
        return Nan::ThrowTypeError("Illegal argument type at position 0");
    }
    auto js_callback = Nan::To<v8::Function>(arg0).ToLocalChecked();

    Nan::AsyncQueueWorker(
        new SingleResultWorker<
            ContextT,
            MethodT,
            ResultT
        >(
            new Nan::Callback(js_callback),
            JsContext<std::shared_ptr<ContextT>>::Unwrap(info.This())));
}

template<typename ContextT, typename MethodT, typename ResultT, typename Arg0>
NAN_METHOD(DelegateWork) {
    v8::Local<v8::Value> arg0 = info[0];
    if (!JsType<Arg0>::IsValidCppType(arg0)) {
        return Nan::ThrowTypeError("Illegal argument type at position 0");
    }

    v8::Local<v8::Value> arg1 = info[1];
    if (!arg1->IsFunction()) {
        return Nan::ThrowTypeError("Last argument at 1 should be callback");
    }
    auto js_callback = Nan::To<v8::Function>(arg1).ToLocalChecked();

    Nan::AsyncQueueWorker(
        new SingleResultWorker<
            ContextT,
            MethodT,
            ResultT,
            Arg0
        >(
            new Nan::Callback(js_callback),
            JsContext<std::shared_ptr<ContextT>>::Unwrap(info.This()),
            JsType<Arg0>::Convert(arg0)));
}

template<typename ContextT, typename MethodT, typename ResultT,
    typename Arg0, typename Arg1>
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

    auto js_callback = Nan::To<v8::Function>(arg2).ToLocalChecked();

     Nan::AsyncQueueWorker(
        new SingleResultWorker<
            ContextT,
            MethodT,
            ResultT,
            Arg0,
            Arg1
        >(
            new Nan::Callback(js_callback),
            JsContext<std::shared_ptr<ContextT>>::Unwrap(info.This()),
            JsType<Arg0>::Convert(arg0),
            JsType<Arg1>::Convert(arg1)));
}

}  // namespace AODBC

#endif /* DELEGATION_HH */

