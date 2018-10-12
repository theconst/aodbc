#ifndef SINGLERESULTWORKER_HH
#define SINGLERESULTWORKER_HH

#include "nan.h"

#include "cpp_to_js_converters.hh"

namespace NC {

template <
    typename OwnerT,
    typename CommandT,
    typename ResultT,
    typename... Args
>
class SingleResultWorker : public Nan::AsyncWorker {
    using ArgsT = std::tuple<Args...>;

    // number of arguments in standard nodejs callback
    // (error + return value)
    static constexpr const int number_of_arguments = 2;

    std::shared_ptr<OwnerT> owner_ptr;
    ArgsT arguments_tuple;
    ResultT result;

 public:
    template <typename T>
    explicit SingleResultWorker(
        Nan::Callback* callback,
        std::shared_ptr<OwnerT> owner_ptr,
        T arguments)
            : AsyncWorker(callback, "NC::SingleResultWorker"),
              owner_ptr(owner_ptr),
              arguments_tuple(std::forward<ArgsT>(arguments)),
              result() {
    }

    SingleResultWorker(const SingleResultWorker&) = delete;
    SingleResultWorker(SingleResultWorker&&) = delete;

    void Execute() override
    try {
        result = CommandT::Execute(owner_ptr.get(), arguments_tuple);
    } catch (const std::exception& e) {
        SetErrorMessage(e.what());
    } catch (...) {
        // TODO(kko): provide something more sane than just tag
        SetErrorMessage("[Fatal] unhandled error inside worker thread");
    }

    void HandleOKCallback() override {
        Nan::HandleScope scope {};

        v8::Local<v8::Value> args[] {
            Nan::Null(),
            convert_cpp_type_to_js(result)
        };

        Nan::Call(
            callback->GetFunction(),
            Nan::GetCurrentContext()->Global(),
            number_of_arguments,
            args);
    }

    virtual ~SingleResultWorker() = default;
};


}  // namespace NC

#endif /* SINGLERESULTWORKER_HH */
