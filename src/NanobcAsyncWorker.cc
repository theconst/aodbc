#include "NanobcAsyncWorker.hh"

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
       
        SetErrorMessage(e.what());
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