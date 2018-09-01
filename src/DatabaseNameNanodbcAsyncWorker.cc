#include "DatabaseNameNanodbcAsyncWorker.hh"

namespace AODBC {

    using namespace AODBC;

    DatabaseNameNanodbcAsyncWorker::DatabaseNameNanodbcAsyncWorker(
            std::shared_ptr< UVMonitor<nanodbc::connection> > connection_monitor,
            Nan::Callback* callback) :
        NanodbcAsyncWorker(connection_monitor, callback) {
    }

    void DatabaseNameNanodbcAsyncWorker::DoExecute(nanodbc::connection* connection) {
        database_name = connection->database_name();
    }

    v8::Local<v8::Value> DatabaseNameNanodbcAsyncWorker::DoGetResult() {
        Nan::EscapableHandleScope scope;

        return scope.Escape(Nan::New<v8::String>(database_name.c_str()).ToLocalChecked());
    }

}

