#include <nan.h>

#include "ODBCConnection.hh"
#include "ODBCStatement.hh"

NAN_MODULE_INIT(Init) {
    NC::ODBCConnection::Init(target);
    NC::ODBCStatement::Init(target);
}

NODE_MODULE(NC, Init)
