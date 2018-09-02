#include "DBMSVersionNanodbcAsyncWorker.hh"

namespace AODBC {

using AODBC::UVMonitor;

DBMSVersionNanodbcAsyncWorker::DBMSVersionNanodbcAsyncWorker(
        std::shared_ptr< UVMonitor<nanodbc::connection> > connection_monitor,
        Nan::Callback* callback) :
NanodbcAsyncWorker(connection_monitor, callback) {
}

void DBMSVersionNanodbcAsyncWorker::DoExecute(nanodbc::connection* connection) {
    dbms_version = connection->dbms_version();
}

v8::Local<v8::Value> DBMSVersionNanodbcAsyncWorker::DoGetResult() {
    Nan::EscapableHandleScope scope;

    return scope.Escape(
        Nan::New<v8::String>(dbms_version.c_str()).ToLocalChecked());
}

}  // namespace AODBC

