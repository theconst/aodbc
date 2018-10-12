#ifndef ODBCTRANSACTION_HH
#define ODBCTRANSACTION_HH

#include <memory>
#include <cstring>

#include <nan.h>

#include "ODBCConnection.hh"
#include "ConnectionAwareTransaction.hh"
#include "SafeUnwrap.hh"

namespace NC {

using NC::ConnectionAwareTransaction;
using NC::SafeUnwrap;


class ODBCTransaction final : public Nan::ObjectWrap {
 public:
    using value_type = ConnectionAwareTransaction;

    static NAN_MODULE_INIT(Init);

    explicit ODBCTransaction(
        std::shared_ptr<UVMonitor<nanodbc::connection>> connection);

    ODBCTransaction(const ODBCTransaction&) = delete;

    ODBCTransaction(ODBCTransaction&&) = delete;

    virtual ~ODBCTransaction() = default;

    static std::shared_ptr<value_type> Unwrap(v8::Local<v8::Object> self) {
        return SafeUnwrap<ODBCTransaction>::Unwrap(self)->transaction;
    }

 private:
    friend class SafeUnwrap<ODBCTransaction>;
    static const char* js_class_name;
    static Nan::Persistent<v8::FunctionTemplate> js_constructor;

    static NAN_METHOD(JsNew);
    static NAN_METHOD(JsBegin);
    static NAN_METHOD(JsCommit);
    static NAN_METHOD(JsRollback);

    std::shared_ptr<ConnectionAwareTransaction> transaction;
};

}  // namespace NC

#endif  /* ODBCTRANSACTION_HH */
