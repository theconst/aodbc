#include "ODBCStatement.hh"

#include "ConnectionAwareStatement.hh"

#include "delegation.hh"

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

    Nan::Set(target,
        Nan::New(js_class_name).ToLocalChecked(),
        Nan::GetFunction(tpl).ToLocalChecked());
}

NAN_METHOD(ODBCStatement::JsQuery) {
    return delegate_work<
        ODBCStatement,
        StatementMethodTag<StatementCommands::query>,
        nc_result_t,
        std::vector<nc_variant_t>
    >(info);
}

NAN_METHOD(ODBCStatement::JsExecute) {
    return delegate_work<
        ODBCStatement,
        StatementMethodTag<StatementCommands::execute>,
        nc_null_t,
        std::vector<nc_variant_t>
    >(info);
}

NAN_METHOD(ODBCStatement::JsPrepare) {
    return delegate_work<
        ODBCStatement,
        StatementMethodTag<StatementCommands::prepare>,
        nc_null_t,
        nc_string_t
    >(info);
}

NAN_METHOD(ODBCStatement::JsNew) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowError(
            Nan::New("ODBC Statement should be called with new")
            .ToLocalChecked());
    }

    v8::Local<v8::Value> arg0 { info[0] };

    if (!arg0->IsObject()) {
        return Nan::ThrowError("Error: connection should be object");
    }

    v8::Local<v8::Object> obj0 { arg0->ToObject() };

    ODBCStatement* odbc_statement {};
    odbc_statement = new ODBCStatement(ODBCConnection::Unwrap(obj0));
    odbc_statement->Wrap(info.Holder());
    info.GetReturnValue().Set(info.Holder());
}

NAN_METHOD(ODBCStatement::JsClose) {
    return delegate_work<
        ODBCStatement,
        StatementMethodTag<StatementCommands::close>,
        nc_null_t
    >(info);
}

}  // namespace NC
