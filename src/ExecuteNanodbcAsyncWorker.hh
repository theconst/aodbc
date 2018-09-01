#ifndef EXECUTENANODBCASYNCWORKER_HH
#define EXECUTENANODBCASYNCWORKER_HH

#include <memory>

#include "nan.h"

#include "NanobcAsyncWorker.hh"
#include "sqltypes.hh"

namespace AODBC {

    using namespace AODBC;
    
    class ExecuteNanodbcAsyncWorker final : public NanodbcAsyncWorker {
    public:
        explicit ExecuteNanodbcAsyncWorker(
            std::shared_ptr< UVMonitor<nanodbc::connection> > connection_monitor,
            std::string&& query,
            Nan::Callback* callback
        );

        ExecuteNanodbcAsyncWorker(const ExecuteNanodbcAsyncWorker& orig) = delete;

        ExecuteNanodbcAsyncWorker(ExecuteNanodbcAsyncWorker&& orig) = delete;

        virtual ~ExecuteNanodbcAsyncWorker() = default;

    protected:

        virtual v8::Local<v8::Value> DoGetResult() override;

        virtual void DoExecute(nanodbc::connection* connection) override;
    private:
        std::string query;
          
        sql_result_t sql_result;
    };

}

#endif /* EXECUTENANODBCASYNCWORKER_HH */

