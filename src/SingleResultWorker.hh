#ifndef SINGLERESULTWORKER_HH
#define SINGLERESULTWORKER_HH

#include "nan.h"

#include "nanodbc.h"

#include "UVMonitor.hh"

#include "cpp_to_js_converters.hh"

namespace AODBC {

enum struct CommandNames {
    dbms_name
};

template<CommandNames tag>
struct MethodTag {
};


template <
    typename OwnerT,
    typename ResultT,
    typename MethodT,
    typename ArgsTuple
>
ResultT call_method(MethodT tag, OwnerT* owner, ArgsTuple args);

template <
    typename OwnerT,
    typename MethodT,
    typename ResultT,
    typename... Args
>
class SingleResultWorker : public Nan::AsyncWorker {
 private:
    static const int number_of_arguments = 2;

    std::shared_ptr<OwnerT> owner_ptr;
    std::tuple<Args...> arguments_tuple;
    ResultT result;

 public:
    explicit SingleResultWorker(
        Nan::Callback* callback,
        std::shared_ptr<OwnerT> owner_ptr,
        Args... arguments)
            : AsyncWorker(callback, "AODBC::SingleResultWorker"),
              owner_ptr(owner_ptr),
              arguments_tuple(std::forward(arguments)...) {
    }

    SingleResultWorker(const SingleResultWorker&) = delete;
    SingleResultWorker(SingleResultWorker&&) = delete;

    void Execute() override {
        try {
            result = call_method<
                OwnerT,
                ResultT,
                MethodT,
                std::tuple<Args...>
            >(MethodT {}, owner_ptr.get(), arguments_tuple);
        } catch (const nanodbc::database_error& db_err) {
            SetErrorMessage(db_err.what());
        } catch (const std::exception& e) {
            SetErrorMessage(e.what());
        }
    }

    void HandleOKCallback() override {
        Nan::HandleScope scope;

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

template<>
nanodbc::string call_method(
        MethodTag<CommandNames::dbms_name>,
        UVMonitor<nanodbc::connection>* owner,
        std::tuple<> tuple) {
    Synchronized lock {owner};
    return owner->get()->dbms_name();
}


}  // namespace AODBC

#endif /* SINGLERESULTWORKER_HH */