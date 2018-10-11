#include "ODBCTransaction.hh"


#include "errors.hh"
#include "ConnectionAwareStatement.hh"
#include "delegation.hh"
#include "TransactionCommands.hh"

namespace NC {

using NC::TransactionCommands;
using NC::ODBCConnection;

const char* ODBCTransaction::js_class_name = "ODBCTransaction";

Nan::Persistent<v8::FunctionTemplate> ODBCTransaction::js_constructor;

ODBCTransaction::ODBCTransaction(
        std::shared_ptr<UVMonitor<nanodbc::connection>> connection) :
    transaction(std::make_shared<ConnectionAwareTransaction>(connection)) {
}

NAN_MODULE_INIT(ODBCTransaction::Init) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(JsNew);
    js_constructor.Reset(tpl);

    tpl->SetClassName(Nan::New<v8::String>(js_class_name).ToLocalChecked());

    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(tpl, "begin", JsBegin);
    Nan::SetPrototypeMethod(tpl, "commit", JsCommit);
    Nan::SetPrototypeMethod(tpl, "rollback", JsRollback);

    Nan::Set(target,
        Nan::New(js_class_name).ToLocalChecked(),
        Nan::GetFunction(tpl).ToLocalChecked());
}

NAN_METHOD(ODBCTransaction::JsBegin) {
    return delegate_work<
        ODBCTransaction,
        TransactionMethodTag<TransactionCommands::begin>,
        nc_null_t
    >(info);
}

NAN_METHOD(ODBCTransaction::JsCommit) {
    return delegate_work<
        ODBCTransaction,
        TransactionMethodTag<TransactionCommands::commit>,
        nc_null_t
    >(info);
}

NAN_METHOD(ODBCTransaction::JsRollback) {
    return delegate_work<
        ODBCTransaction,
        TransactionMethodTag<TransactionCommands::rollback>,
        nc_null_t
    >(info);
}

NAN_METHOD(ODBCTransaction::JsNew)
try {
    if (!info.IsConstructCall()) {
        throw Error("ODBC Transaction should be called with new");
    }

    v8::Local<v8::Value> arg0 { info[0] };

    if (!arg0->IsObject()) {
        throw Error("Error: connection should be object");
    }

    auto connection = ODBCConnection::Unwrap(arg0->ToObject());
    std::unique_ptr<ODBCTransaction> odbc_transaction {
        new ODBCTransaction(connection) };
    odbc_transaction->Wrap(info.Holder());
    info.GetReturnValue().Set(info.Holder());

    (void) odbc_transaction.release();
} catch (...) {
    handle_error();
}


}  // namespace NC
