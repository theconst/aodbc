#ifndef DBMSNAMENANODBCASYNCWORKER_HH
#define DBMSNAMENANODBCASYNCWORKER_HH

#include <memory>

#include "nan.h"

#include "NanobcAsyncWorker.hh"

namespace AODBC {

    using namespace AODBC;

    class DBMSNameNanodbcAsyncWorker final : public NanodbcAsyncWorker {
    public:
        explicit DBMSNameNanodbcAsyncWorker(
            std::shared_ptr< UVMonitor<nanodbc::connection> > connection_monitor,
            Nan::Callback* callback
        );

        DBMSNameNanodbcAsyncWorker(const DBMSNameNanodbcAsyncWorker& orig) = delete;

        DBMSNameNanodbcAsyncWorker(DBMSNameNanodbcAsyncWorker&& orig) = delete;

        virtual ~DBMSNameNanodbcAsyncWorker() = default;

    protected:

        virtual v8::Local<v8::Value> DoGetResult() override;

        virtual void DoExecute(nanodbc::connection* connection) override;
    private:
        std::string dbms_name;
    };

}

#endif /* DBMSNAMENANODBCASYNCWORKER_HH */

