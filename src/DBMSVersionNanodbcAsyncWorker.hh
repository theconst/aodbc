#ifndef DBMSVERSIONNANODBCASYNCWORKER_HH
#define DBMSVERSIONNANODBCASYNCWORKER_HH

#include <memory>

#include "NanobcAsyncWorker.hh"

#include "nan.h"

namespace AODBC {

using AODBC::UVMonitor;

class DBMSVersionNanodbcAsyncWorker final : public NanodbcAsyncWorker {
 public:
    explicit DBMSVersionNanodbcAsyncWorker(
        std::shared_ptr<UVMonitor<nanodbc::connection>> connection_monitor,
        Nan::Callback *callback);

    DBMSVersionNanodbcAsyncWorker(
        const DBMSVersionNanodbcAsyncWorker &orig) = delete;
    DBMSVersionNanodbcAsyncWorker(
        DBMSVersionNanodbcAsyncWorker &&orig) = delete;

    virtual ~DBMSVersionNanodbcAsyncWorker() = default;
 protected:
    v8::Local<v8::Value> DoGetResult() override;
    void DoExecute(nanodbc::connection *connection) override;

 private:
    std::string dbms_version;
};

}  // namespace AODBC

#endif /* DBMSVERSIONNANODBCASYNCWORKER_HH */
