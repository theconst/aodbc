
/* 
 * File:   DBMSNameNanodbcAsyncWorker.cc
 * Author: kko
 * 
 * Created on August 27, 2018, 10:30 PM
 */

#include "DBMSNameNanodbcAsyncWorker.hh"

namespace AODBC {

    using namespace AODBC;

    DBMSNameNanodbcAsyncWorker::DBMSNameNanodbcAsyncWorker(
            std::shared_ptr< UVMonitor<nanodbc::connection> > connection_monitor,
            Nan::Callback* callback) :
    NanodbcAsyncWorker(connection_monitor, callback) {
    }

    void DBMSNameNanodbcAsyncWorker::DoExecute(nanodbc::connection* connection) {
        dbms_name = connection->dbms_name();
    }

    v8::Local<v8::Value> DBMSNameNanodbcAsyncWorker::DoGetResult() {
        Nan::EscapableHandleScope scope;

        return scope.Escape(Nan::New<v8::String>(dbms_name.c_str()).ToLocalChecked());
    }

}
