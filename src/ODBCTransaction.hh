#ifndef ODBCTRANSACTION_HH
#define ODBCTRANSACTION_HH

#include <memory>
#include <cstring>

#include "nan.h"

#include "ODBCConnection.hh"

#include "ConnectionAwareTransaction.hh"


namespace NC {

using NC::ConnectionAwareTransaction;


class ODBCTransaction final : public Nan::ObjectWrap {
 public:
    static const char* js_class_name;

    using value_type = ConnectionAwareTransaction;

    static NAN_MODULE_INIT(Init);

    explicit ODBCTransaction(
        std::shared_ptr<UVMonitor<nanodbc::connection>> connection);

    ODBCTransaction(const ODBCTransaction&) = delete;

    ODBCTransaction(ODBCTransaction&&) = delete;

    virtual ~ODBCTransaction() = default;

    static std::shared_ptr<value_type> Unwrap(v8::Local<v8::Object> self) {
        if (!Nan::New(js_constructor)->HasInstance(self)) {
            throw Error("Argument should be derived from ODBCTransaction");
        }

        auto* odbc_transaction = Nan::ObjectWrap::Unwrap<ODBCTransaction>(self);
        return odbc_transaction->transaction;
    }

 private:
    static Nan::Persistent<v8::FunctionTemplate> js_constructor;

    static NAN_METHOD(JsNew);
    static NAN_METHOD(JsBegin);
    static NAN_METHOD(JsCommit);
    static NAN_METHOD(JsRollback);

    std::shared_ptr<ConnectionAwareTransaction> transaction;
};

}  // namespace NC

#endif  /* ODBCTRANSACTION_HH */
