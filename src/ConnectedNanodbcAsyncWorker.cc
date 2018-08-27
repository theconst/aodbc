
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
    
    void ConnectedNanodbcAsyncWorker::DoExecute() {
        UVMonitor<nanodbc::connection>* connection = connection_monitor.get();
        
        UVMonitor<nanodbc::connection>::Synchronized lock(connection);
        
        
        //TODO: do something with copy construction??
        connected = connection->get()->connected();
    }
    
    void ConnectedNanodbcAsyncWorker::HandleOKCallback() {
        Nan::HandleScope scope;
        
        v8::Local<v8::Value> args[] = {
            Nan::Null(),
            Nan::New<v8::Boolean>(connected)
        };
        
        Nan::Call(
            callback->GetFunction(), 
            Nan::GetCurrentContext()->Global(), 
            NUMBER_OF_ARGS, 
            args);
    }

    ConnectedNanodbcAsyncWorker::~ConnectedNanodbcAsyncWorker() {
        
    }

}
