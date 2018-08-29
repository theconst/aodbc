#include <utility>

#include "uv.h"

#include "nanodbc.h"

#include "ODBCConnection.hh"
#include "UVMonitor.hh"
#include "DBMSNameNanodbcAsyncWorker.hh"
#include "ConnectedNanodbcAsyncWorker.hh"
#include "DisconnectNanodbcAsyncWorker.hh"
#include "ConnectNanodbcAsyncWorker.hh"
#include "DBMSVersionNanodbcAsyncWorker.hh"
#include "DriverNameNanodbcAsyncWorker.hh"
#include "CatalogNameNanodbcAsyncWorker.hh"
#include "DatabaseNameNanodbcAsyncWorker.hh"
#include "ExecuteNanodbcAsyncWorker.hh"

namespace AODBC {
    
using namespace AODBC;

const std::string ODBCConnection::JS_CLASS_NAME = "ODBCConnection";

Nan::Persistent<v8::FunctionTemplate> ODBCConnection::JS_CONSTRUCTOR;

ODBCConnection::ODBCConnection()
    : connection(std::make_shared<UVMonitor<nanodbc::connection> >()) {
    
}

ODBCConnection::ODBCConnection(std::string&& connection_string)
    : connection(std::make_shared<UVMonitor<nanodbc::connection> >(connection_string)) {
}

NAN_MODULE_INIT(ODBCConnection::Init) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(JsNew);
    JS_CONSTRUCTOR.Reset(tpl);
    
    tpl->SetClassName(Nan::New<v8::String>(JS_CLASS_NAME).ToLocalChecked());
    
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    
    Nan::SetPrototypeMethod(tpl, "isConnected", JsConnected);
    Nan::SetPrototypeMethod(tpl, "disconnect", JsDisconnect);
    Nan::SetPrototypeMethod(tpl, "connect", JsConnect);
    Nan::SetPrototypeMethod(tpl, "dbmsName", JsDBMSName);
    Nan::SetPrototypeMethod(tpl, "dbmsVersion", JsDBMSVersion);
    Nan::SetPrototypeMethod(tpl, "driverName", JsDriverName);
    Nan::SetPrototypeMethod(tpl, "catalogName", JsCatalogName);
    Nan::SetPrototypeMethod(tpl, "databaseName", JsDatabaseName);
    Nan::SetPrototypeMethod(tpl, "execute", JsExecute);
    
    Nan::Set(target,
            Nan::New(JS_CLASS_NAME.c_str()).ToLocalChecked(), 
            Nan::GetFunction(tpl).ToLocalChecked());
}


NAN_METHOD(ODBCConnection::JsNew) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowError(Nan::New("ODBC Connection should be called with new").ToLocalChecked());
    }
    
    v8::Local<v8::Value> connection_string_js = info[0];

    ODBCConnection *odbc_connection = nullptr; 
    if (connection_string_js->IsString()) { 
       odbc_connection = new ODBCConnection(*v8::String::Utf8Value(connection_string_js->ToString()));
    } else if (connection_string_js->IsUndefined()) {
       odbc_connection = new ODBCConnection();
    } else {
       return Nan::ThrowTypeError("Argument should be of type string");
    }
    
    odbc_connection->Wrap(info.Holder());
}

// Used to delegate 
template <>
struct Context<std::shared_ptr<UVMonitor <nanodbc::connection> > > {

    inline static std::shared_ptr <UVMonitor <nanodbc::connection> > Unwrap(const v8::Local<v8::Object>& obj) {
        ODBCConnection* conn = Nan::ObjectWrap::Unwrap<ODBCConnection>(obj);

        return conn->connection;
    }
};

NAN_METHOD(ODBCConnection::JsConnected) {
    return DelegateWork<
        std::shared_ptr<UVMonitor<nanodbc::connection> >,
        ConnectedNanodbcAsyncWorker
    >(info);
}

NAN_METHOD(ODBCConnection::JsConnect) {
    return DelegateWork<
        std::shared_ptr<UVMonitor<nanodbc::connection> >,
        ConnectNanodbcAsyncWorker,    
        std::string,
        long
    >(info);
}

NAN_METHOD(ODBCConnection::JsDisconnect) {
    DelegateWork<
        std::shared_ptr<UVMonitor<nanodbc::connection> >,
        DisconnectNanodbcAsyncWorker
    >(info);
}

NAN_METHOD(ODBCConnection::JsDBMSName) {
    return DelegateWork<
        std::shared_ptr<UVMonitor<nanodbc::connection> >, 
        DBMSNameNanodbcAsyncWorker
    >(info);
}

NAN_METHOD(ODBCConnection::JsDBMSVersion) {
    return DelegateWork<
        std::shared_ptr<UVMonitor<nanodbc::connection > >,
        DBMSVersionNanodbcAsyncWorker
    >(info);
};


NAN_METHOD(ODBCConnection::JsDriverName) {
    return DelegateWork<
        std::shared_ptr<UVMonitor<nanodbc::connection > >,
        DriverNameNanodbcAsyncWorker    
    >(info);
}

NAN_METHOD(ODBCConnection::JsCatalogName) {
    return DelegateWork<
        std::shared_ptr<UVMonitor<nanodbc::connection > >,
        CatalogNameNanodbcAsyncWorker  
    >(info);
}

NAN_METHOD(ODBCConnection::JsDatabaseName) {
    return DelegateWork<
        std::shared_ptr<UVMonitor<nanodbc::connection > >,
        DatabaseNameNanodbcAsyncWorker
    >(info);
}

NAN_METHOD(ODBCConnection::JsExecute) { 
    return DelegateWork<
        std::shared_ptr<UVMonitor<nanodbc::connection > >,
        ExecuteNanodbcAsyncWorker,
        std::string
    >(info);
}

}