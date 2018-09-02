#include "CatalogNameNanodbcAsyncWorker.hh"

namespace AODBC {

    using AODBC::UVMonitor;

    CatalogNameNanodbcAsyncWorker::CatalogNameNanodbcAsyncWorker(
            std::shared_ptr<
                UVMonitor<nanodbc::connection> > connection_monitor,
            Nan::Callback* callback) :
        NanodbcAsyncWorker(connection_monitor, callback) {
    }

    void CatalogNameNanodbcAsyncWorker::DoExecute(
            nanodbc::connection* connection) {
        catalog_name = connection->catalog_name();
    }

    v8::Local<v8::Value> CatalogNameNanodbcAsyncWorker::DoGetResult() {
        Nan::EscapableHandleScope scope;

        return scope.Escape(
            Nan::New<v8::String>(catalog_name.c_str()).ToLocalChecked());
    }

}  // namespace AODBC
