
/* 
 * File:   ConnectionNanodbcAsyncWorker.hh
 * Author: kko
 *
 * Created on August 26, 2018, 9:47 PM
 */

#ifndef CONNECTIONNANODBCASYNCWORKER_HH
#define CONNECTIONNANODBCASYNCWORKER_HH

#include <memory>

#include "NanobcAsyncWorker.hh"

#include "nan.h"

namespace AODBC {
    
using namespace AODBC;

class ConnectedNanodbcAsyncWorker final : public NanodbcAsyncWorker {
public:
    explicit ConnectedNanodbcAsyncWorker(
        std::shared_ptr< UVMonitor<nanodbc::connection> > connection_monitor,
        Nan::Callback* callback
    );
    
    ConnectedNanodbcAsyncWorker(const ConnectedNanodbcAsyncWorker& orig) = delete;
    
    ConnectedNanodbcAsyncWorker(ConnectedNanodbcAsyncWorker&& orig) = delete;
    
    virtual ~ConnectedNanodbcAsyncWorker() = default;
    
protected:
    
    virtual v8::Local<v8::Value> DoGetResult() override;
    
    virtual void DoExecute(nanodbc::connection* connection) override;
private:    
    bool connected;
};

}

#endif /* CONNECTIONNANODBCASYNCWORKER_HH */

