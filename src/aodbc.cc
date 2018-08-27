#include <nan.h>
#include "ODBCConnection.hh"

using namespace v8;

using namespace AODBC;


NAN_MODULE_INIT(Init) {
    ODBCConnection::Init(target);
}

NODE_MODULE(aodbc, Init)