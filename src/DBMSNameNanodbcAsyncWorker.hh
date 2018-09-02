#ifndef DBMSNAMENANODBCASYNCWORKER_HH
#define DBMSNAMENANODBCASYNCWORKER_HH

#include <memory>

#include "nan.h"

#include "NanobcAsyncWorker.hh"

namespace AODBC {

using AODBC::UVMonitor;

class DBMSNameNanodbcAsyncWorker final : public NanodbcAsyncWorker {
 public:
    explicit DBMSNameNanodbcAsyncWorker(
        std::shared_ptr<UVMonitor<nanodbc::connection>> connection_monitor,
        Nan::Callback *callback);

    DBMSNameNanodbcAsyncWorker(const DBMSNameNanodbcAsyncWorker &orig) = delete;
    DBMSNameNanodbcAsyncWorker(DBMSNameNanodbcAsyncWorker &&orig) = delete;

    virtual ~DBMSNameNanodbcAsyncWorker() = default;
 protected:
    v8::Local<v8::Value> DoGetResult() override;
    void DoExecute(nanodbc::connection *connection) override;

 private:
    std::string dbms_name;
};

}  // namespace AODBC

#endif /* DBMSNAMENANODBCASYNCWORKER_HH */
