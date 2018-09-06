#include <utility>

#include "ODBCConnection.hh"

#include "UVMonitor.hh"
#include "delegation.hh"

namespace AODBC {

using AODBC::MethodTag;
using AODBC::CommandNames;

const char* ODBCConnection::js_class_name = "ODBCConnection";

Nan::Persistent<v8::FunctionTemplate> ODBCConnection::js_constructor;

ODBCConnection::ODBCConnection()
    : connection(std::make_shared<UVMonitor<nanodbc::connection>>()) {
}

ODBCConnection::ODBCConnection(std::string&& connection_string)
    : connection(
        std::make_shared<UVMonitor<nanodbc::connection>>(connection_string)) {
}

NAN_MODULE_INIT(ODBCConnection::Init) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(JsNew);
    js_constructor.Reset(tpl);

    tpl->SetClassName(Nan::New<v8::String>(js_class_name).ToLocalChecked());

    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(tpl, "isConnected", JsConnected);
    Nan::SetPrototypeMethod(tpl, "disconnect", JsDisconnect);
    Nan::SetPrototypeMethod(tpl, "connect", JsConnect);
    Nan::SetPrototypeMethod(tpl, "dbmsName", JsDBMSName);
    Nan::SetPrototypeMethod(tpl, "dbmsVersion", JsDBMSVersion);
    Nan::SetPrototypeMethod(tpl, "driverName", JsDriverName);
    Nan::SetPrototypeMethod(tpl, "catalogName", JsCatalogName);
    Nan::SetPrototypeMethod(tpl, "databaseName", JsDatabaseName);
    Nan::SetPrototypeMethod(tpl, "query", JsQuery);
    Nan::SetPrototypeMethod(tpl, "execute", JsExecute);

    Nan::Set(target,
        Nan::New(js_class_name).ToLocalChecked(),
        Nan::GetFunction(tpl).ToLocalChecked());
}

template <>
std::shared_ptr<UVMonitor<nanodbc::connection>> unwrap_caller(
        v8::Local<v8::Object> obj) {
    ODBCConnection* conn = Nan::ObjectWrap::Unwrap<ODBCConnection>(obj);

    return conn->GetConnection();
}


NAN_METHOD(ODBCConnection::JsNew) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowError(
            Nan::New("ODBC Connection should be called with new")
            .ToLocalChecked());
    }

    v8::Local<v8::Value> connection_string_js = info[0];

    ODBCConnection *odbc_connection = nullptr;
    if (connection_string_js->IsString()) {
       odbc_connection = new ODBCConnection(
           *v8::String::Utf8Value(connection_string_js->ToString()));
    } else if (connection_string_js->IsUndefined()) {
       odbc_connection = new ODBCConnection();
    } else {
       return Nan::ThrowTypeError("Argument should be of type string");
    }

    odbc_connection->Wrap(info.Holder());
}

// TODO(kko): we can now make uber-define over all the methods

NAN_METHOD(ODBCConnection::JsConnected) {
    return DelegateWork<
        UVMonitor<nanodbc::connection>,
        MethodTag<CommandNames::is_connected>,
        bool
    >(info);
}

NAN_METHOD(ODBCConnection::JsConnect) {
    return DelegateWork<
        UVMonitor<nanodbc::connection>,
        MethodTag<CommandNames::connect>,
        sql_null_t,
        sql_string_t,
        sql_long_t
    >(info);
}

NAN_METHOD(ODBCConnection::JsDisconnect) {
    return DelegateWork<
        UVMonitor<nanodbc::connection>,
        MethodTag<CommandNames::disconnect>,
        sql_null_t
    >(info);
}

NAN_METHOD(ODBCConnection::JsDBMSName) {
    return DelegateWork<
        UVMonitor<nanodbc::connection>,
        MethodTag<CommandNames::dbms_name>,
        sql_string_t
    >(info);
}

NAN_METHOD(ODBCConnection::JsDBMSVersion) {
    return DelegateWork<
        UVMonitor<nanodbc::connection>,
        MethodTag<CommandNames::dbms_version>,
        sql_string_t
    >(info);
};


NAN_METHOD(ODBCConnection::JsDriverName) {
    return DelegateWork<
        UVMonitor<nanodbc::connection>,
        MethodTag<CommandNames::driver_name>,
        sql_string_t
    >(info);
}

NAN_METHOD(ODBCConnection::JsCatalogName) {
    return DelegateWork<
        UVMonitor<nanodbc::connection>,
        MethodTag<CommandNames::catalog_name>,
        sql_string_t
    >(info);
}

NAN_METHOD(ODBCConnection::JsDatabaseName) {
    return DelegateWork<
        UVMonitor<nanodbc::connection>,
        MethodTag<CommandNames::database_name>,
        sql_string_t  // TODO(kko): remove sql prefix
    >(info);
}

NAN_METHOD(ODBCConnection::JsQuery) {
    return DelegateWork<
        UVMonitor<nanodbc::connection>,
        MethodTag<CommandNames::query>,
        AODBC::sql_result_t,
        sql_string_t
    >(info);
}

NAN_METHOD(ODBCConnection::JsExecute) {
    return DelegateWork<
        UVMonitor<nanodbc::connection>,
        MethodTag<CommandNames::execute>,
        sql_null_t,
        sql_string_t
    >(info);
}

}  // namespace AODBC
