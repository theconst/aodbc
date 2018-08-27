#include "uv.h"

#include "nanodbc.h"

#include "ODBCConnection.hh"

#include "ConnectedNanodbcAsyncWorker.hh"

#include "UVMonitor.hh"

#include <utility>

namespace AODBC {
    
using namespace AODBC;

const std::string ODBCConnection::JS_CLASS_NAME = "ODBCConnection";

Nan::Persistent<v8::FunctionTemplate> ODBCConnection::JS_CONSTRUCTOR;

ODBCConnection::ODBCConnection(std::string&& connection_string)
    : connection(std::make_shared<UVMonitor<nanodbc::connection> >(connection_string)) {
}

ODBCConnection::~ODBCConnection() {

}

NAN_MODULE_INIT(ODBCConnection::Init) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(JsNew);
    JS_CONSTRUCTOR.Reset(tpl);
    
    tpl->SetClassName(Nan::New<v8::String>(JS_CLASS_NAME).ToLocalChecked());
    
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    
    Nan::SetPrototypeMethod(tpl, "isConnected", JsConnected);
    Nan::SetPrototypeMethod(tpl, "disconnect", JsDisconnect);
    
    Nan::Set(target,
            Nan::New(JS_CLASS_NAME.c_str()).ToLocalChecked(), 
            Nan::GetFunction(tpl).ToLocalChecked());
//    Nan::SetPrototypeMethod(tpl, "dbmsName", JsDBMSName);
//    Nan::SetPrototypeMethod(tpl, "driverName", JsDriverName);
//    Nan::SetPrototypeMethod(tpl, "catalogName", JsCatalogName);
   
}


NAN_METHOD(ODBCConnection::JsNew) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowError(Nan::New("ODBC Connection should be called with new").ToLocalChecked());
    }
    
    v8::Local<v8::Value> connection_string_js = info[0];

    ODBCConnection *odbc_connection = nullptr; 
    if (connection_string_js->IsString()) { 
       odbc_connection = new ODBCConnection(*v8::String::Utf8Value(connection_string_js->ToString()));
    } else {
        return Nan::ThrowTypeError("Argument should be of type string");
    } 
    
    odbc_connection->Wrap(info.Holder());
}

NAN_METHOD(ODBCConnection::JsConnected) {
   ODBCConnection *odbc_conn = Nan::ObjectWrap::Unwrap<ODBCConnection>(info.This());

   v8::Local<v8::Value> arg0 = info[0];
   if (!arg0->IsFunction()) {
       return Nan::ThrowTypeError("ODBC connection accepts callback");
   }
   
   v8::Local<v8::Function> js_callback = Nan::To<v8::Function>(arg0)
           .ToLocalChecked();
   
   Nan::AsyncQueueWorker(new ConnectedNanodbcAsyncWorker(
       odbc_conn->connection,
       new Nan::Callback(js_callback)
   ));
}

NAN_METHOD(ODBCConnection::JsDisconnect) {
    
}

//NAN_METHOD(ODBCConnection::JsDBMSName) {
//    
//}

//static NAN_METHOD(JsDBMSVersion);
//
//static NAN_METHOD(JsDriverName);
//
//static NAN_METHOD(JsCatalogName);

}