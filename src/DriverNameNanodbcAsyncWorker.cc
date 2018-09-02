#include "DriverNameNanodbcAsyncWorker.hh"

namespace AODBC {

using AODBC::UVMonitor;

DriverNameNanodbcAsyncWorker::DriverNameNanodbcAsyncWorker(
        std::shared_ptr< UVMonitor<nanodbc::connection>> connection_monitor,
        Nan::Callback* callback) :
    NanodbcAsyncWorker(connection_monitor, callback) {
}

void DriverNameNanodbcAsyncWorker::DoExecute(nanodbc::connection* connection) {
    driver_name = connection->driver_name();
}

v8::Local<v8::Value> DriverNameNanodbcAsyncWorker::DoGetResult() {
    Nan::EscapableHandleScope scope;

    return scope.Escape(
        Nan::New<v8::String>(driver_name.c_str()).ToLocalChecked());
}

}  // namespace AODBC

