#ifndef DRIVERNAMENANODBCASYNCWORKER_HH
#define DRIVERNAMENANODBCASYNCWORKER_HH

#include <memory>

#include "nan.h"

#include "NanobcAsyncWorker.hh"


namespace AODBC {

using AODBC::UVMonitor;

class DriverNameNanodbcAsyncWorker final : public NanodbcAsyncWorker {
 public:
    explicit DriverNameNanodbcAsyncWorker(
        std::shared_ptr< UVMonitor<nanodbc::connection> > connection_monitor,
        Nan::Callback* callback);

    DriverNameNanodbcAsyncWorker(
        const DriverNameNanodbcAsyncWorker& orig) = delete;
    DriverNameNanodbcAsyncWorker(DriverNameNanodbcAsyncWorker&& orig) = delete;

    virtual ~DriverNameNanodbcAsyncWorker() = default;
 protected:
    v8::Local<v8::Value> DoGetResult() override;
    void DoExecute(nanodbc::connection* connection) override;

 private:
    std::string driver_name;
};

}  // namespace AODBC

#endif /* DRIVERNAMENANODBCASYNCWORKER_HH */

