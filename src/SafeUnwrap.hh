#ifndef SAFEUNWRAP_HH
#define SAFEUNWRAP_HH

#include <nan.h>

#include "errors.hh"

namespace NC {

template<typename JsT>
struct SafeUnwrap {
    static JsT* Unwrap(v8::Local<v8::Object> self) {
        if (!Nan::New(JsT::js_constructor)->HasInstance(self)) {
            std::string msg {"Argument should be derived from "};
            msg += JsT::js_class_name;
            throw TypeError(msg);
        }
        return Nan::ObjectWrap::Unwrap<JsT>(self);
    }
};


}  // namespace NC


#endif  /* SAFEUNWRAP_HH */
