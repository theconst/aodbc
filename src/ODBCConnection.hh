#ifndef ODBCCONNECTION_HH
#define ODBCCONNECTION_HH

#include <memory>

#include "nan.h"

#include "nanodbc.h"

#include "JsTypes.hh"
#include "UVMonitor.hh"


namespace AODBC {

using AODBC::UVMonitor;

class ODBCConnection final : public Nan::ObjectWrap {
 public:
    static NAN_MODULE_INIT(Init);

    ODBCConnection(const ODBCConnection&) = delete;

    ODBCConnection(ODBCConnection&&) = delete;

    virtual ~ODBCConnection() = default;

 private:
    friend class JsContext<std::shared_ptr<UVMonitor<nanodbc::connection>>>;

    static const char* JS_CLASS_NAME;

    static NAN_METHOD(JsNew);
    static NAN_METHOD(JsConnected);
    static NAN_METHOD(JsConnect);
    static NAN_METHOD(JsDisconnect);
    static NAN_METHOD(JsDBMSName);
    static NAN_METHOD(JsDBMSVersion);
    static NAN_METHOD(JsDriverName);
    static NAN_METHOD(JsCatalogName);
    static NAN_METHOD(JsDatabaseName);
    static NAN_METHOD(JsExecute);
    static NAN_METHOD(JsJustExecute);

    static Nan::Persistent<v8::FunctionTemplate> JS_CONSTRUCTOR;

    ODBCConnection();
    explicit ODBCConnection(std::string&& conn_string);

    std::shared_ptr<UVMonitor< nanodbc::connection>> connection;
};


}  // namespace AODBC
#endif /* ODBCCONNECTION_HH */

