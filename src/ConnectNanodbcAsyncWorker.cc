#include "ConnectNanodbcAsyncWorker.hh"

#include "UVMonitor.hh"
#include "nanodbc.h"

namespace AODBC {

using namespace AODBC;

ConnectNanodbcAsyncWorker::ConnectNanodbcAsyncWorker(
    std::shared_ptr< UVMonitor<nanodbc::connection> > connection_monitor,
    std::string&& connection_string,
    long timeout,
    Nan::Callback* callback) :
NanodbcAsyncWorker(connection_monitor, callback),
connection_string(std::move(connection_string)),
timeout(timeout) {

}

void ConnectNanodbcAsyncWorker::DoExecute(nanodbc::connection* connection) {
    connection->connect(connection_string, timeout);
}

v8::Local<v8::Value> ConnectNanodbcAsyncWorker::DoGetResult() {
    Nan::EscapableHandleScope scope;

    return scope.Escape(Nan::Undefined());
}

}

