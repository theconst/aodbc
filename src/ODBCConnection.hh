#ifndef ODBCCONNECTION_HH
#define ODBCCONNECTION_HH

#include <memory>
#include <cstring>
#include <exception>

#include "nan.h"
#include "nanodbc.h"

#include "nctypes.hh"
#include "UVMonitor.hh"
#include "SafeUnwrap.hh"

namespace NC {

using NC::UVMonitor;
using NC::SafeUnwrap;
using NC::Error;

class ODBCConnection final : public Nan::ObjectWrap {
 public:
    using value_type = UVMonitor<nanodbc::connection>;

    static NAN_MODULE_INIT(Init);

    static std::shared_ptr<value_type> Unwrap(v8::Local<v8::Object> self) {
        return SafeUnwrap<ODBCConnection>::Unwrap(self)->connection;
    }

    ODBCConnection();

    ODBCConnection(const ODBCConnection&) = delete;

    ODBCConnection(ODBCConnection&&) = delete;

    virtual ~ODBCConnection() = default;

 private:
    friend class SafeUnwrap<ODBCConnection>;
    static const char* js_class_name;
    static Nan::Persistent<v8::FunctionTemplate> js_constructor;

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
    static NAN_METHOD(JsSetAutoCommit);

    std::shared_ptr<UVMonitor<nanodbc::connection>> connection;
};

}  // namespace NC
#endif /* ODBCCONNECTION_HH */

