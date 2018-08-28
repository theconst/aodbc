
/* 
 * File:   ConnectNanodbcAsyncWorker.hh
 * Author: kko
 *
 * Created on August 28, 2018, 1:25 AM
 */

#ifndef CONNECTNANODBCASYNCWORKER_HH
#define CONNECTNANODBCASYNCWORKER_HH

#include <memory>

#include "NanobcAsyncWorker.hh"

#include "nan.h"


namespace AODBC {

using namespace AODBC;

class ConnectNanodbcAsyncWorker final : public NanodbcAsyncWorker {
public:
    explicit ConnectNanodbcAsyncWorker(
        std::shared_ptr< UVMonitor<nanodbc::connection> > connection_monitor,
        std::string&& connection_string,
        long timeout,
        Nan::Callback* callback
    );

    ConnectNanodbcAsyncWorker(const ConnectNanodbcAsyncWorker& orig) = delete;

    ConnectNanodbcAsyncWorker(ConnectNanodbcAsyncWorker&& orig) = delete;

    virtual ~ConnectNanodbcAsyncWorker() = default;
protected:
    virtual v8::Local<v8::Value> DoGetResult() override;

    virtual void DoExecute(nanodbc::connection* connection) override;
private:
    std::string connection_string;
    
    long timeout;
};

}

#endif /* CONNECTNANODBCASYNCWORKER_HH */

