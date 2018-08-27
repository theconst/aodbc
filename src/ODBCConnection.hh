
/* 
 * File:   ODBCConnection.hh
 * Author: kko
 *
 * Created on August 25, 2018, 8:22 PM
 */

#ifndef ODBCCONNECTION_HH
#define ODBCCONNECTION_HH

#include <memory>

#include "nan.h"
#include "uv.h"
#include "nanodbc.h"

#include "UVMonitor.hh"



namespace AODBC {

using namespace AODBC;
    
class ODBCConnection final : public Nan::ObjectWrap {
    public:
        static NAN_MODULE_INIT(Init);
        
        virtual ~ODBCConnection() = default;
    
    private:
        
        explicit ODBCConnection(std::string&& conn_string);
        
        static NAN_METHOD(JsNew);
        
        static NAN_METHOD(JsConnected);
        
        static NAN_METHOD(JsDisconnect);
        
        static NAN_METHOD(JsDBMSName);
        
//        TODO: other dbms methods
//        static NAN_METHOD(JsDBMSVersion);
//        
//        static NAN_METHOD(JsDriverName);
//        
//        static NAN_METHOD(JsCatalogName);
        
        template<typename WorkerT>
        inline static NAN_METHOD(DelegateWork);
        
        static Nan::Persistent<v8::FunctionTemplate> JS_CONSTRUCTOR;
        
        static const std::string JS_CLASS_NAME;
        
        std::shared_ptr< UVMonitor< nanodbc::connection > > connection; 
};

template<typename WorkerT>
NAN_METHOD(ODBCConnection::DelegateWork) {
    ODBCConnection *odbc_conn = Nan::ObjectWrap::Unwrap<ODBCConnection>(info.This());

    v8::Local<v8::Value> arg0 = info[0];
    if (!arg0->IsFunction()) {
        return Nan::ThrowTypeError("ODBC connection accepts callback");
    }

    v8::Local<v8::Function> js_callback = Nan::To<v8::Function>(arg0)
            .ToLocalChecked();

    Nan::AsyncQueueWorker(new WorkerT(
            odbc_conn->connection,
            new Nan::Callback(js_callback)
        )
    );
}

}
#endif /* ODBCCONNECTION_HH */

