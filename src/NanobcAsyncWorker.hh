
/* 
 * File:   NanobcWorker.hh
 * Author: kko
 *
 * Created on August 26, 2018, 3:06 PM
 */

#ifndef NANOBCWORKER_HH
#define NANOBCWORKER_HH

#include "nan.h"
#include "nanodbc.h"
#include "UVMonitor.hh"

namespace AODBC {

/* abstract */ class NanodbcAsyncWorker : public Nan::AsyncWorker  {

public:
    
    NanodbcAsyncWorker(const NanodbcAsyncWorker&) =delete;
    
    NanodbcAsyncWorker(NanodbcAsyncWorker&&) =delete;

    virtual void Execute() override final;
    
    virtual ~NanodbcAsyncWorker();
    
protected:
    static const int NUMBER_OF_ARGS = 2;
    
    explicit NanodbcAsyncWorker(
        std::shared_ptr< UVMonitor<nanodbc::connection> >, Nan::Callback*);
    
    virtual void DoExecute(nanodbc::connection* connection) = 0;
    
    std::shared_ptr< UVMonitor<nanodbc::connection> > connection_monitor;
private:
};

}

#endif /* NANOBCWORKER_HH */

