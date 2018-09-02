#ifndef JUSTEXECUTENANODBCASYNCWORKER_HH
#define JUSTEXECUTENANODBCASYNCWORKER_HH

#include <memory>

#include "nan.h"

#include "NanobcAsyncWorker.hh"
#include "sqltypes.hh"

namespace AODBC {

using AODBC::UVMonitor;

class JustExecuteNanodbcAsyncWorker final : public NanodbcAsyncWorker {
 public:
    explicit JustExecuteNanodbcAsyncWorker(
        std::shared_ptr< UVMonitor<nanodbc::connection>> connection_monitor,
        std::string&& query,
        Nan::Callback* callback);

    JustExecuteNanodbcAsyncWorker(
        const JustExecuteNanodbcAsyncWorker& orig) = delete;
    JustExecuteNanodbcAsyncWorker(
        JustExecuteNanodbcAsyncWorker&& orig) = delete;

    virtual ~JustExecuteNanodbcAsyncWorker() = default;
 protected:
    void DoExecute(nanodbc::connection* connection) override;

 private:
    std::string query;
    sql_result_t sql_result;
};

}  // namespace AODBC

#endif /* JUSTEXECUTENANODBCASYNCWORKER_HH */

