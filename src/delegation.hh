#ifndef DELEGATION_HH
#define DELEGATION_HH


#include "nan.h"

#include "arguments.hh"
#include "js_to_cpp_converters.hh"
#include "SingleResultWorker.hh"

// This is the most awkward part of the project
// TODO(kko): carefully roll the loop back

namespace NC {

using NC::SingleResultWorker;

template<typename ContextT, typename MethodT, typename ResultT>
NAN_METHOD(delegate_work)
try {
    v8::Local<v8::Value> arg0 = info[0];
    if (!arg0->IsFunction()) {
        return Nan::ThrowTypeError("Illegal argument type at position 0");
    }

    auto js_callback = Nan::To<v8::Function>(arg0).ToLocalChecked();

    auto* worker = new SingleResultWorker<
        typename ContextT::value_type, MethodT, ResultT, std::tuple<>>(
            new Nan::Callback(js_callback),
            ContextT::Unwrap(info.This()),
            std::tuple<> {});

    Nan::AsyncQueueWorker(worker);
} catch (const std::exception& e) {
    Nan::ThrowError(e.what());
}

template<typename ContextT, typename MethodT, typename ResultT, typename Arg0>
NAN_METHOD(delegate_work)
try {
    auto&& arg0 = convert_js_type_to_cpp<Arg0>(info[0]);
    if (!arg0) {
        return Nan::ThrowTypeError("Illegal argument type at position 0");
    }

    v8::Local<v8::Value> arg1 = info[1];
    if (!arg1->IsFunction()) {
        return Nan::ThrowTypeError("Last argument at 1 should be callback");
    }

    auto js_callback = Nan::To<v8::Function>(arg1).ToLocalChecked();

    auto* worker = new SingleResultWorker<
        typename ContextT::value_type, MethodT, ResultT,
                std::tuple<Arg0>>(
            new Nan::Callback(js_callback),
            ContextT::Unwrap(info.This()),
            std::make_tuple(*arg0));

    Nan::AsyncQueueWorker(worker);
} catch (const std::exception& e) {
    Nan::ThrowError(e.what());
}

template<typename ContextT, typename MethodT, typename ResultT,
    typename Arg0, typename Arg1>
NAN_METHOD(delegate_work)
try {
    auto&& arg0 = convert_js_type_to_cpp<Arg0>(info[0]);
    if (!arg0) {
        return Nan::ThrowTypeError("Illegal argument type at position 0");
    }

    auto&& arg1 = convert_js_type_to_cpp<Arg1>(info[1]);
    if (!arg1) {
        return Nan::ThrowTypeError("Illegal argument type at position 1");
    }

    v8::Local<v8::Value> arg2 = info[2];
    if (!arg2->IsFunction()) {
        return Nan::ThrowTypeError("Last argument at 2 should be callback");
    }

    auto js_callback = Nan::To<v8::Function>(arg2).ToLocalChecked();

    auto* worker = new SingleResultWorker<
        typename ContextT::value_type, MethodT, ResultT,
        std::tuple<Arg0, Arg1>>(
            new Nan::Callback(js_callback),
            ContextT::Unwrap(info.This()),
            std::make_tuple(*arg0, *arg1));

    Nan::AsyncQueueWorker(worker);
} catch (const std::exception& e) {
    Nan::ThrowError(e.what());
}

}  // namespace NC

#endif /* DELEGATION_HH */

