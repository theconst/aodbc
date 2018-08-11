#include <nan.h>

#define VERSION "0.0.1"

using namespace v8;


NAN_METHOD(Version) {
    info.GetReturnValue().Set(Nan::New(VERSION).ToLocalChecked());
}

NAN_MODULE_INIT(init) {
    Nan::SetMethod(target, "version", Version);
}

NODE_MODULE(factorial, init)