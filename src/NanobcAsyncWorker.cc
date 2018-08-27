
/* 
 * File:   NanobcWorker.cc
 * Author: kko
 * 
 * Created on August 26, 2018, 3:06 PM
 * 
 * //TODO: can we generalize even more to remove worker boilerplate
 */

#include "NanobcAsyncWorker.hh"

#include <memory>

#include "UVMonitor.hh"

#include <exception>

namespace AODBC {
    
using namespace AODBC;

NanodbcAsyncWorker::NanodbcAsyncWorker(
        std::shared_ptr< UVMonitor<nanodbc::connection> > monitor, 
        Nan::Callback* callback) :
    AsyncWorker(std::move(callback), "AODBC::NanodbcAsyncWorker"), 
    connection_monitor(monitor) {
}

void NanodbcAsyncWorker::Execute() {
    try {
        UVMonitor<nanodbc::connection>* connection = connection_monitor.get();
        
        Synchronized lock(connection);
        
        DoExecute(connection->get());
    } catch (const nanodbc::database_error& db_err) {
        SetErrorMessage(db_err.what());
    } catch (const std::exception& e) {
        
        SetErrorMessage("Fatal: unhandled runtime exception");
    }
}

void NanodbcAsyncWorker::HandleOKCallback() {
    Nan::HandleScope scope;

    v8::Local<v8::Value> args[] = {
        Nan::Null(),
        DoGetResult()
    };

    Nan::Call(
        callback->GetFunction(),
        Nan::GetCurrentContext()->Global(),
        NUMBER_OF_ARGS,
        args
    );
}

}