
/* 
 * File:   ConnectionNanodbcAsyncWorker.cc
 * Author: kko
 * 
 * Created on August 26, 2018, 9:47 PM
 */

#include <utility>

#include "ConnectedNanodbcAsyncWorker.hh"
#include "UVMonitor.hh"
#include "nanodbc.h"
#include <iostream>

namespace AODBC {
    
    using namespace AODBC;
    
    ConnectedNanodbcAsyncWorker::ConnectedNanodbcAsyncWorker(
            std::shared_ptr< UVMonitor<nanodbc::connection> > connection_monitor,
            Nan::Callback* callback) :
        NanodbcAsyncWorker(connection_monitor, callback),
            
        connected(false)
    { }
    
    void ConnectedNanodbcAsyncWorker::DoExecute(nanodbc::connection* connection) {
        connected = connection->connected();
    }
    
    v8::Local<v8::Value> ConnectedNanodbcAsyncWorker::DoGetResult() {
        Nan::EscapableHandleScope scope;
        
        return scope.Escape(Nan::New<v8::Boolean>(connected));
    }

    ConnectedNanodbcAsyncWorker::~ConnectedNanodbcAsyncWorker() {
        
    }

}
