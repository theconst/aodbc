
/* 
 * File:   DisconnectNanodbcAsyncWorker.hh
 * Author: kko
 *
 * Created on August 27, 2018, 11:15 PM
 */

#ifndef DISCONNECTNANODBCASYNCWORKER_HH
#define DISCONNECTNANODBCASYNCWORKER_HH

#include <memory>

#include "NanobcAsyncWorker.hh"

#include "nan.h"

namespace AODBC {

using namespace AODBC;

class DisconnectNanodbcAsyncWorker final : public NanodbcAsyncWorker {
public:
    explicit DisconnectNanodbcAsyncWorker(
            std::shared_ptr< UVMonitor<nanodbc::connection> > connection_monitor,
            Nan::Callback* callback
    );

    DisconnectNanodbcAsyncWorker(const DisconnectNanodbcAsyncWorker& orig) = delete;

    DisconnectNanodbcAsyncWorker(DisconnectNanodbcAsyncWorker&& orig) = delete;

    virtual ~DisconnectNanodbcAsyncWorker() = default;

protected:

    virtual v8::Local<v8::Value> DoGetResult() override;

    virtual void DoExecute(nanodbc::connection* connection) override;
};

}

#endif /* DISCONNECTNANODBCASYNCWORKER_HH */

