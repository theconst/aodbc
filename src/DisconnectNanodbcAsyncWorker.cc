
/* 
 * File:   DisconnectNanodbcAsyncWorker.cc
 * Author: kko
 * 
 * Created on August 27, 2018, 11:15 PM
 */

#include "DisconnectNanodbcAsyncWorker.hh"

namespace AODBC {

using namespace AODBC;

DisconnectNanodbcAsyncWorker::DisconnectNanodbcAsyncWorker(
        std::shared_ptr< UVMonitor<nanodbc::connection> > connection_monitor,
        Nan::Callback* callback) :
NanodbcAsyncWorker(connection_monitor, callback) {
    
}

void DisconnectNanodbcAsyncWorker::DoExecute(nanodbc::connection* connection) {
    connection->disconnect();
}

v8::Local<v8::Value> DisconnectNanodbcAsyncWorker::DoGetResult() {
    Nan::EscapableHandleScope scope;

    return scope.Escape(Nan::Undefined());
}

}
