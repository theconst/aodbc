#include <nan.h>

#define VERSION "0.0.1"

using namespace v8;


NAN_METHOD(VersionSync) {
    info.GetReturnValue().Set(Nan::New(VERSION).ToLocalChecked());
}

NAN_MODULE_INIT(init) {
    Nan::SetMethod(target, "versionSync", VersionSync);
}

NODE_MODULE(anodem, init)