#ifndef CONNECTNANODBCASYNCWORKER_HH
#define CONNECTNANODBCASYNCWORKER_HH

#include <memory>

#include "nan.h"

#include "NanobcAsyncWorker.hh"

namespace AODBC {

using AODBC::UVMonitor;

class ConnectNanodbcAsyncWorker final : public NanodbcAsyncWorker {
 public:
    explicit ConnectNanodbcAsyncWorker(
        std::shared_ptr<UVMonitor<nanodbc::connection> > connection_monitor,
        std::string&& connection_string,
        long timeout,  // NOLINT(runtime/int) - nanodbc defined API
        Nan::Callback* callback);

    ConnectNanodbcAsyncWorker(const ConnectNanodbcAsyncWorker& orig) = delete;
    ConnectNanodbcAsyncWorker(ConnectNanodbcAsyncWorker&& orig) = delete;

    virtual ~ConnectNanodbcAsyncWorker() = default;
 protected:
    v8::Local<v8::Value> DoGetResult() override;

    void DoExecute(nanodbc::connection* connection) override;

 private:
    std::string connection_string;
    long timeout;  // NOLINT(runtime/int) - nanodbc defined API
};

}  // namespace AODBC

#endif /* CONNECTNANODBCASYNCWORKER_HH */

