#include <nan.h>
#include "ODBCConnection.hh"

NAN_MODULE_INIT(Init) {
    AODBC::ODBCConnection::Init(target);
}

NODE_MODULE(aodbc, Init)
