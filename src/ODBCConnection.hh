
/* 
 * File:   ODBCConnection.hh
 * Author: kko
 *
 * Created on August 25, 2018, 8:22 PM
 */

#ifndef ODBCCONNECTION_HH
#define ODBCCONNECTION_HH

#include <memory>

#include "nan.h"
#include "uv.h"
#include "nanodbc.h"

#include "UVMonitor.hh"



namespace AODBC {

using namespace AODBC;
    
class ODBCConnection final : public Nan::ObjectWrap {
    public:
        static NAN_MODULE_INIT(Init);
    
    private:
        
        explicit ODBCConnection(std::string&& conn_string);
        
        virtual ~ODBCConnection();
        
        static NAN_METHOD(JsNew);
        
        static NAN_METHOD(JsConnected);
        
        static NAN_METHOD(JsDisconnect);
        
//        static NAN_METHOD(JsDBMSName);
        
//        TODO: other dbms methods
//        static NAN_METHOD(JsDBMSVersion);
//        
//        static NAN_METHOD(JsDriverName);
//        
//        static NAN_METHOD(JsCatalogName);
        
        static Nan::Persistent<v8::FunctionTemplate> JS_CONSTRUCTOR;
        
        static const std::string JS_CLASS_NAME;
        
        std::shared_ptr< UVMonitor< nanodbc::connection > > connection; 
};

}
#endif /* ODBCCONNECTION_HH */

