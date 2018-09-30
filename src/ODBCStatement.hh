#ifndef ODBCSTATEMENT_HH
#define ODBCSTATEMENT_HH

#include <memory>
#include <cstring>

#include "nan.h"

#include "ODBCConnection.hh"

#include "ConnectionAwareStatement.hh"


namespace NC {

using NC::ConnectionAwareStatement;


class ODBCStatement final : public Nan::ObjectWrap {
 public:
    static const char* js_class_name;

    using value_type = ConnectionAwareStatement;

    static NAN_MODULE_INIT(Init);

    explicit ODBCStatement(
        std::shared_ptr<UVMonitor<nanodbc::connection>> connection);

    ODBCStatement(const ODBCStatement&) = delete;

    ODBCStatement(ODBCStatement&&) = delete;

    virtual ~ODBCStatement() = default;

    static std::shared_ptr<value_type> Unwrap(v8::Local<v8::Object> self) {
        if (!Nan::New(js_constructor)->HasInstance(self)) {
            throw Error("Argument should be derived from ODBCStatement");
        }

        auto* odbc_statement = Nan::ObjectWrap::Unwrap<ODBCStatement>(self);
        return odbc_statement->statement;
    }

 private:
    static Nan::Persistent<v8::FunctionTemplate> js_constructor;

    static NAN_METHOD(JsNew);
    static NAN_METHOD(JsQuery);
    static NAN_METHOD(JsExecute);
    static NAN_METHOD(JsPrepare);
    static NAN_METHOD(JsClose);
    static NAN_METHOD(JsOpen);

    std::shared_ptr<ConnectionAwareStatement> statement;
};

}  // namespace NC

#endif  /* ODBCSTATEMENT_HH */
