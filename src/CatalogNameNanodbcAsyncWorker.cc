
/* 
 * File:   CatalogNameNanodbcAsyncWorker.cc
 * Author: kko
 * 
 * Created on August 28, 2018, 8:16 PM
 */

#include "CatalogNameNanodbcAsyncWorker.hh"

namespace AODBC {

    using namespace AODBC;

    CatalogNameNanodbcAsyncWorker::CatalogNameNanodbcAsyncWorker(
            std::shared_ptr< UVMonitor<nanodbc::connection> > connection_monitor,
            Nan::Callback* callback) :
    NanodbcAsyncWorker(connection_monitor, callback) {
    }

    void CatalogNameNanodbcAsyncWorker::DoExecute(nanodbc::connection* connection) {
        catalog_name = connection->catalog_name();
    }

    v8::Local<v8::Value> CatalogNameNanodbcAsyncWorker::DoGetResult() {
        Nan::EscapableHandleScope scope;

        return scope.Escape(Nan::New<v8::String>(catalog_name.c_str()).ToLocalChecked());
    }

}
