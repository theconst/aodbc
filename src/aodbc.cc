#include <nan.h>
#include "VersionWorker.hh"
#include "ODBCConnection.hh"

using namespace v8;

using namespace AODBC;

NAN_METHOD(Version) {
    Nan::Callback *cb = new Nan::Callback(Nan::To<Function>(info[0]).ToLocalChecked());
    
    Nan::AsyncQueueWorker(new AODBC::VersionWorker(cb));
}


NAN_MODULE_INIT(Init) {
    Nan::SetMethod(target, "version", Version);
    
    ODBCConnection::Init(target);
}

NODE_MODULE(aodbc, Init)