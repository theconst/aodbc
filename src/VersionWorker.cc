
/* 
 * File:   VersionWorker.cc
 * Author: kko
 * 
 * This is a useless worker that displays version of the library on the screen
 * 
 * Created on August 25, 2018, 1:52 PM
 */

#include "VersionWorker.hh"

namespace AODBC {

using namespace AODBC;

const std::string VersionWorker::version = "0.0.1";

VersionWorker::VersionWorker(Nan::Callback *cb) : Nan::AsyncWorker(cb) {
}

void VersionWorker::Execute() {
    // this is just a demo
}

void VersionWorker::HandleOKCallback() {
    v8::Local<v8::Value> args[] = {
        Nan::Null(),
        Nan::New<v8::String>(version).ToLocalChecked(),
    };
    
    callback->Call(2, args, async_resource);
}

VersionWorker::~VersionWorker() {
    
}

}