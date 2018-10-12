#include "ODBCStatement.hh"

#include "errors.hh"
#include "ConnectionAwareStatement.hh"
#include "delegation.hh"
#include "StatementCommands.hh"

namespace NC {

using NC::StatementCommands;
using NC::ODBCConnection;

const char* ODBCStatement::js_class_name = "ODBCStatement";

Nan::Persistent<v8::FunctionTemplate> ODBCStatement::js_constructor;

ODBCStatement::ODBCStatement(
        std::shared_ptr<UVMonitor<nanodbc::connection>> connection) :
    statement(std::make_shared<ConnectionAwareStatement>(connection)) {
}

NAN_MODULE_INIT(ODBCStatement::Init) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(JsNew);
    js_constructor.Reset(tpl);

    tpl->SetClassName(Nan::New<v8::String>(js_class_name).ToLocalChecked());

    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(tpl, "query", JsQuery);
    Nan::SetPrototypeMethod(tpl, "execute", JsExecute);
    Nan::SetPrototypeMethod(tpl, "prepare", JsPrepare);
    Nan::SetPrototypeMethod(tpl, "close", JsClose);
    Nan::SetPrototypeMethod(tpl, "isOpen", JsOpen);

    Nan::Set(target,
        Nan::New(js_class_name).ToLocalChecked(),
        Nan::GetFunction(tpl).ToLocalChecked());
}

NAN_METHOD(ODBCStatement::JsQuery) {
    return delegate_work<
        ODBCStatement,
        StatementCommand<StatementCommands::query>,
        nc_result_t,
        PreparedStatementArguments
    >(info);
}

NAN_METHOD(ODBCStatement::JsExecute) {
    return delegate_work<
        ODBCStatement,
        StatementCommand<StatementCommands::execute>,
        nc_null_t,
        PreparedStatementArguments
    >(info);
}

NAN_METHOD(ODBCStatement::JsPrepare) {
    return delegate_work<
        ODBCStatement,
        StatementCommand<StatementCommands::prepare>,
        nc_null_t,
        nc_string_t
    >(info);
}

NAN_METHOD(ODBCStatement::JsNew)
try {
    if (!info.IsConstructCall()) {
        throw Error("ODBC Statement should be called with new");
    }

    v8::Local<v8::Value> arg0 { info[0] };

    if (!arg0->IsObject()) {
        throw Error("Error: connection should be object");
    }

    auto connection = ODBCConnection::Unwrap(arg0->ToObject());
    std::unique_ptr<ODBCStatement> odbc_statement {
        new ODBCStatement(connection) };
    odbc_statement->Wrap(info.Holder());
    info.GetReturnValue().Set(info.Holder());

    (void) odbc_statement.release();
} catch (...) {
    handle_error();
}

NAN_METHOD(ODBCStatement::JsClose) {
    return delegate_work<
        ODBCStatement,
        StatementCommand<StatementCommands::close>,
        nc_null_t
    >(info);
}

NAN_METHOD(ODBCStatement::JsOpen) {
    return delegate_work<
        ODBCStatement,
        StatementCommand<StatementCommands::is_open>,
        bool
    >(info);
}


}  // namespace NC
