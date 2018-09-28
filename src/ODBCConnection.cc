#include "ODBCConnection.hh"

#include <utility>

#include "delegation.hh"

namespace NC {

using NC::ConnectionMethodTag;
using NC::ConnectionCommands;

using NC::QueryArguments;

const char* ODBCConnection::js_class_name = "ODBCConnection";

Nan::Persistent<v8::FunctionTemplate> ODBCConnection::js_constructor;

ODBCConnection::ODBCConnection()
    : connection(std::make_shared<UVMonitor<nanodbc::connection>>()) {
}

ODBCConnection::ODBCConnection(nc_string_t connection_string)
    : connection(std::make_shared<UVMonitor<nanodbc::connection>>(
        std::move(connection_string))) {
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


NAN_METHOD(ODBCConnection::JsNew)
try {
    if (!info.IsConstructCall()) {
        throw Error("ODBC Connection should be called with new");
    }

    v8::Local<v8::Value> connection_string_js = info[0];

    ODBCConnection* odbc_connection {};
    if (connection_string_js->IsString()) {
       odbc_connection = new ODBCConnection(
           *Nan::Utf8String(connection_string_js));
    } else if (connection_string_js->IsUndefined()) {
       odbc_connection = new ODBCConnection();
    } else {
       throw TypeError("Argument should be of type string");
    }

    odbc_connection->Wrap(info.Holder());

    info.GetReturnValue().Set(info.Holder());
} catch (...) {
    handle_error();
}

NAN_METHOD(ODBCConnection::JsConnected) {
    return delegate_work<
        ODBCConnection,
        ConnectionMethodTag<ConnectionCommands::is_connected>,
        bool
    >(info);
}

NAN_METHOD(ODBCConnection::JsConnect) {
    // TODO(kko): make Either<T1, T2>
    static constexpr int number_of_args_with_timeout = 2 + 1;
    int len = info.Length();
    if (len >= number_of_args_with_timeout) {
        return delegate_work<
            ODBCConnection,
            ConnectionMethodTag<ConnectionCommands::connect>,
            nc_null_t,
            nc_string_t,
            TimeoutArg
        >(info);
    } else {
        return delegate_work<
            ODBCConnection,
            ConnectionMethodTag<ConnectionCommands::connect>,
            nc_null_t,
            nc_string_t
        >(info);
    }
}

NAN_METHOD(ODBCConnection::JsDisconnect) {
    return delegate_work<
        ODBCConnection,
        ConnectionMethodTag<ConnectionCommands::disconnect>,
        nc_null_t
    >(info);
}

NAN_METHOD(ODBCConnection::JsDBMSName) {
    return delegate_work<
        ODBCConnection,
        ConnectionMethodTag<ConnectionCommands::dbms_name>,
        nc_string_t
    >(info);
}

NAN_METHOD(ODBCConnection::JsDBMSVersion) {
    return delegate_work<
        ODBCConnection,
        ConnectionMethodTag<ConnectionCommands::dbms_version>,
        nc_string_t
    >(info);
};


NAN_METHOD(ODBCConnection::JsDriverName) {
    return delegate_work<
        ODBCConnection,
        ConnectionMethodTag<ConnectionCommands::driver_name>,
        nc_string_t
    >(info);
}

NAN_METHOD(ODBCConnection::JsCatalogName) {
    return delegate_work<
        ODBCConnection,
        ConnectionMethodTag<ConnectionCommands::catalog_name>,
        nc_string_t
    >(info);
}

NAN_METHOD(ODBCConnection::JsDatabaseName) {
    return delegate_work<
        ODBCConnection,
        ConnectionMethodTag<ConnectionCommands::database_name>,
        nc_string_t
    >(info);
}

NAN_METHOD(ODBCConnection::JsQuery) {
    return delegate_work<
        ODBCConnection,
        ConnectionMethodTag<ConnectionCommands::query>,
        nc_result_t,
        QueryArguments
    >(info);
}

NAN_METHOD(ODBCConnection::JsExecute) {
    return delegate_work<
        ODBCConnection,
        ConnectionMethodTag<ConnectionCommands::execute>,
        nc_null_t,
        QueryArguments
    >(info);
}

}  // namespace NC
