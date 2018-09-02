#ifndef DATABASENAMENANODBCASYNCWORKER_HH
#define DATABASENAMENANODBCASYNCWORKER_HH

#include <memory>

#include "NanobcAsyncWorker.hh"

#include "nan.h"

namespace AODBC {

using AODBC::UVMonitor;

class DatabaseNameNanodbcAsyncWorker final : public NanodbcAsyncWorker {
 public:
    explicit DatabaseNameNanodbcAsyncWorker(
        std::shared_ptr<UVMonitor<nanodbc::connection>> connection_monitor,
        Nan::Callback *callback);

    DatabaseNameNanodbcAsyncWorker(
        const DatabaseNameNanodbcAsyncWorker &orig) = delete;
    DatabaseNameNanodbcAsyncWorker(
        DatabaseNameNanodbcAsyncWorker &&orig) = delete;

    virtual ~DatabaseNameNanodbcAsyncWorker() = default;

 protected:
    v8::Local<v8::Value> DoGetResult() override;
    void DoExecute(nanodbc::connection *connection) override;

 private:
    std::string database_name;
};

}  // namespace AODBC

#endif /* DATABASENAMENANODBCASYNCWORKER_HH */
