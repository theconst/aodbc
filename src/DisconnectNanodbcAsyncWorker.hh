#ifndef DISCONNECTNANODBCASYNCWORKER_HH
#define DISCONNECTNANODBCASYNCWORKER_HH

#include <memory>

#include "NanobcAsyncWorker.hh"

#include "nan.h"

namespace AODBC {

using AODBC::UVMonitor;

class DisconnectNanodbcAsyncWorker final : public NanodbcAsyncWorker {
 public:
    explicit DisconnectNanodbcAsyncWorker(
        std::shared_ptr<UVMonitor<nanodbc::connection>> connection_monitor,
        Nan::Callback *callback);

    DisconnectNanodbcAsyncWorker(
        const DisconnectNanodbcAsyncWorker &orig) = delete;
    DisconnectNanodbcAsyncWorker(DisconnectNanodbcAsyncWorker &&orig) = delete;

    virtual ~DisconnectNanodbcAsyncWorker() = default;

 protected:
    v8::Local<v8::Value> DoGetResult() override;
    void DoExecute(nanodbc::connection *connection) override;

 private:
};

}  // namespace AODBC

#endif /* DISCONNECTNANODBCASYNCWORKER_HH */
