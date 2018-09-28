#ifndef DELEGATION_HH
#define DELEGATION_HH


#include "nan.h"

#include <stdexcept>
#include <sstream>

#include "errors.hh"
#include "arguments.hh"
#include "js_to_cpp_converters.hh"
#include "SingleResultWorker.hh"

namespace NC {

using NC::SingleResultWorker;

using NC::TypeError;

template <std::size_t position, typename T>
inline std::tuple<T> convert_or_else_throw(Nan::NAN_METHOD_ARGS_TYPE info) {
    auto&& maybe_value = convert_js_type_to_cpp<T>(info[position]);
    if (!maybe_value) {
        std::string msg {"Illegal argument type at position "};
        msg += std::to_string(position);
        throw TypeError(msg);
    }
    return std::make_tuple(std::move(*maybe_value));
}

template <std::size_t size>
inline std::tuple<> convert_args(Nan::NAN_METHOD_ARGS_TYPE info) {
    return std::tuple<> {};
}

template <std::size_t size, typename Arg0, typename... Args>
inline std::tuple<Arg0, Args...> convert_args(Nan::NAN_METHOD_ARGS_TYPE info) {
    constexpr std::size_t index = size - sizeof...(Args) - 1;
    return std::tuple_cat(
        convert_or_else_throw<index, Arg0>(info),
        convert_args<size, Args...>(info));
}

template<typename ContextT, typename MethodT,
    typename ResultT, typename... Args>
NAN_METHOD(delegate_work)
try {
    using CVT = typename ContextT::value_type;
    using M = MethodT;
    using R = ResultT;
    using SRW = SingleResultWorker<CVT, M, R, Args...>;

    constexpr std::size_t args_size = sizeof...(Args);

    v8::Local<v8::Value> cb = info[args_size];
    if (!cb->IsFunction()) {
        throw TypeError("Last argument should be a callback");
    }

    auto js_callback = Nan::To<v8::Function>(cb).ToLocalChecked();

    std::unique_ptr<Nan::Callback> nan_cb { new Nan::Callback(js_callback) };
    std::tuple<Args...> args { convert_args<args_size, Args...>(info) };
    std::shared_ptr<CVT> context { ContextT::Unwrap(info.This()) };

    std::unique_ptr<SRW> worker {
        new SRW(nan_cb.get(), context, std::move(args)) };

    Nan::AsyncQueueWorker(worker.get());

    // resources will be freed by async worker when callback finishes
    // see AsyncWorker::~AsyncWorker()
    (void) nan_cb.release();
    // see AsyncExecuteComplete and AsyncWorker::Destroy()
    (void) worker.release();
} catch(...) {
    handle_error();
}

}  // namespace NC

#endif /* DELEGATION_HH */

