#ifndef JSTYPES_HH
#define JSTYPES_HH

#include "nan.h"

// TODO: remove unneeded objects 

namespace AODBC {

template<typename T>
struct JsContext {
    inline static T Unwrap(v8::Local<v8::Object>);
};

// convert and validate common js types
template<typename T>
struct JsType {
    inline static T Convert(v8::Local<v8::Value> local);

    inline static bool IsValidCppType(v8::Local<v8::Value> local);
};

template<>
struct JsType<std::string> {
    inline static std::string Convert(v8::Local<v8::Value> local) {
        return std::string(*v8::String::Utf8Value(local->ToString()));
    }

    inline static bool IsValidCppType(v8::Local<v8::Value> local) {
        return local->IsString();
    }
};

template<>
struct JsType<long> {  // NOLINT(runtime/int)
                                               //- nanodbc defined API
    inline static long Convert(  // NOLINT(runtime/int) - nanodbc defined API
            v8::Local<v8::Value> local) {
        return local->IntegerValue();
    }

    inline static bool IsValidCppType(v8::Local<v8::Value> local) {
        return local->IsNumber();
    }
};

}  // namespace AODBC



#endif  // JSTYPES_HH
