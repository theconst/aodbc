#ifndef ODBCCONNECTION_HH
#define ODBCCONNECTION_HH

#include <memory>
#include <cstring>
#include <exception>

#include "nan.h"
#include "nanodbc.h"

#include "errors.hh"
#include "nctypes.hh"
#include "UVMonitor.hh"

namespace NC {

using NC::UVMonitor;

using NC::Error;

class ODBCConnection final : public Nan::ObjectWrap {
 public:
    static const char* js_class_name;

    using value_type = UVMonitor<nanodbc::connection>;

    static NAN_MODULE_INIT(Init);

    ODBCConnection(const ODBCConnection&) = delete;

    ODBCConnection(ODBCConnection&&) = delete;

    virtual ~ODBCConnection() = default;

    static std::shared_ptr<value_type> Unwrap(v8::Local<v8::Object> self) {
        char* constructor_name = *Nan::Utf8String(self->GetConstructorName());
        if (0 != std::strcmp(constructor_name, js_class_name)) {
            throw Error("Argument should be of type ODBCConnection");
        }

        ODBCConnection* odbc_connection =
            Nan::ObjectWrap::Unwrap<ODBCConnection>(self);
        return odbc_connection->connection;
    }

 private:
    static NAN_METHOD(JsNew);
    static NAN_METHOD(JsConnected);
    static NAN_METHOD(JsConnect);
    static NAN_METHOD(JsDisconnect);
    static NAN_METHOD(JsDBMSName);
    static NAN_METHOD(JsDBMSVersion);
    static NAN_METHOD(JsDriverName);
    static NAN_METHOD(JsCatalogName);
    static NAN_METHOD(JsDatabaseName);
    static NAN_METHOD(JsQuery);
    static NAN_METHOD(JsExecute);

    static Nan::Persistent<v8::FunctionTemplate> js_constructor;

    ODBCConnection();
    explicit ODBCConnection(nc_string_t conn_string);

    std::shared_ptr<UVMonitor<nanodbc::connection>> connection;
};

}  // namespace NC
#endif /* ODBCCONNECTION_HH */

