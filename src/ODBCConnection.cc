#include "ODBCConnection.hh"

#include <memory>
#include <utility>

#include "delegation.hh"
#include "ConnectionCommands.hh"

namespace NC {

using NC::ConnectionCommand;
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
    Nan::SetPrototypeMethod(tpl, "setAutoCommit", JsSetAutoCommit);

    Nan::Set(target,
        Nan::New(js_class_name).ToLocalChecked(),
        Nan::GetFunction(tpl).ToLocalChecked());
}


NAN_METHOD(ODBCConnection::JsNew)
try {
    if (!info.IsConstructCall()) {
        throw Error("ODBC Connection should be called with new");
    }

    std::unique_ptr<ODBCConnection> odbc_connection { new ODBCConnection() };
    odbc_connection->Wrap(info.Holder());

    info.GetReturnValue().Set(info.Holder());

    (void) odbc_connection.release();
} catch (...) {
    handle_error();
}

NAN_METHOD(ODBCConnection::JsConnected) {
    return delegate_work<
        ODBCConnection,
        ConnectionCommand<ConnectionCommands::is_connected>,
        bool
    >(info);
}

NAN_METHOD(ODBCConnection::JsConnect) {
    // TODO(kko): make Either<T1, T2>
    static constexpr int dsn_timeout_size = 2 + 1;
    static constexpr int dsn_username_password_args_size = 3 + 1;
    static constexpr int dsn_username_passord_timeout_args_size = 4 + 1;
    switch (info.Length()) {
    case dsn_username_passord_timeout_args_size:
        return delegate_work<
            ODBCConnection,
            ConnectionCommand<ConnectionCommands::connect>,
            nc_null_t,
            nc_string_t,
            nc_string_t,
            nc_string_t,
            TimeoutArg
        >(info);
    case dsn_username_password_args_size:
        return delegate_work<
            ODBCConnection,
            ConnectionCommand<ConnectionCommands::connect>,
            nc_null_t,
            nc_string_t,
            nc_string_t,
            nc_string_t
        >(info);
    case dsn_timeout_size:
        return delegate_work<
            ODBCConnection,
            ConnectionCommand<ConnectionCommands::connect>,
            nc_null_t,
            nc_string_t,
            TimeoutArg
        >(info);
    default:
        return delegate_work<
            ODBCConnection,
            ConnectionCommand<ConnectionCommands::connect>,
            nc_null_t,
            nc_string_t
        >(info);
    }
}

NAN_METHOD(ODBCConnection::JsDisconnect) {
    return delegate_work<
        ODBCConnection,
        ConnectionCommand<ConnectionCommands::disconnect>,
        nc_null_t
    >(info);
}

NAN_METHOD(ODBCConnection::JsDBMSName) {
    return delegate_work<
        ODBCConnection,
        ConnectionCommand<ConnectionCommands::dbms_name>,
        nc_string_t
    >(info);
}

NAN_METHOD(ODBCConnection::JsDBMSVersion) {
    return delegate_work<
        ODBCConnection,
        ConnectionCommand<ConnectionCommands::dbms_version>,
        nc_string_t
    >(info);
};


NAN_METHOD(ODBCConnection::JsDriverName) {
    return delegate_work<
        ODBCConnection,
        ConnectionCommand<ConnectionCommands::driver_name>,
        nc_string_t
    >(info);
}

NAN_METHOD(ODBCConnection::JsCatalogName) {
    return delegate_work<
        ODBCConnection,
        ConnectionCommand<ConnectionCommands::catalog_name>,
        nc_string_t
    >(info);
}

NAN_METHOD(ODBCConnection::JsDatabaseName) {
    return delegate_work<
        ODBCConnection,
        ConnectionCommand<ConnectionCommands::database_name>,
        nc_string_t
    >(info);
}

NAN_METHOD(ODBCConnection::JsQuery) {
    return delegate_work<
        ODBCConnection,
        ConnectionCommand<ConnectionCommands::query>,
        nc_result_t,
        QueryArguments
    >(info);
}

NAN_METHOD(ODBCConnection::JsExecute) {
    return delegate_work<
        ODBCConnection,
        ConnectionCommand<ConnectionCommands::execute>,
        nc_null_t,
        QueryArguments
    >(info);
}

NAN_METHOD(ODBCConnection::JsSetAutoCommit) {
    return delegate_work<
        ODBCConnection,
        ConnectionCommand<ConnectionCommands::set_auto_commit>,
        nc_null_t,
        bool
    >(info);
}

}  // namespace NC
