#include <utility>

#include "nanodbc.h"

#include "ConnectedNanodbcAsyncWorker.hh"

namespace AODBC {

using AODBC::UVMonitor;

ConnectedNanodbcAsyncWorker::ConnectedNanodbcAsyncWorker(
        std::shared_ptr< UVMonitor<nanodbc::connection> > connection_monitor,
        Nan::Callback* callback) :
    NanodbcAsyncWorker(connection_monitor, callback),
    connected(false) {
}

void ConnectedNanodbcAsyncWorker::DoExecute(nanodbc::connection* connection) {
    connected = connection->connected();
}

v8::Local<v8::Value> ConnectedNanodbcAsyncWorker::DoGetResult() {
    Nan::EscapableHandleScope scope;

    return scope.Escape(Nan::New<v8::Boolean>(connected));
}

}
