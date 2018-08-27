
/* 
 * File:   NanobcWorker.cc
 * Author: kko
 * 
 * Created on August 26, 2018, 3:06 PM
 */

#include "NanobcAsyncWorker.hh"

#include <memory>

#include <mutex>

#include <iostream>
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
        DoExecute();
    } catch (const nanodbc::database_error& db_err) {
        SetErrorMessage(db_err.what());
    } catch (const std::exception& e) {
        
        SetErrorMessage("Fatal: unhandled runtime exception");
    }
}

NanodbcAsyncWorker::~NanodbcAsyncWorker() {

}

}