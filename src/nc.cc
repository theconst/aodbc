#include <nan.h>
#include "ODBCConnection.hh"

NAN_MODULE_INIT(Init) {
    NC::ODBCConnection::Init(target);
}

NODE_MODULE(NC, Init)
