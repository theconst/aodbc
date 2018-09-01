#ifndef DATABASENAMENANODBCASYNCWORKER_HH
#define DATABASENAMENANODBCASYNCWORKER_HH

#include <memory>

#include "NanobcAsyncWorker.hh"

#include "nan.h"

namespace AODBC {

    using namespace AODBC;

    class DatabaseNameNanodbcAsyncWorker final : public NanodbcAsyncWorker {
    public:
        explicit DatabaseNameNanodbcAsyncWorker(
            std::shared_ptr< UVMonitor<nanodbc::connection> > connection_monitor,
            Nan::Callback* callback
        );

        DatabaseNameNanodbcAsyncWorker(const DatabaseNameNanodbcAsyncWorker& orig) = delete;

        DatabaseNameNanodbcAsyncWorker(DatabaseNameNanodbcAsyncWorker&& orig) = delete;

        virtual ~DatabaseNameNanodbcAsyncWorker() = default;

    protected:

        virtual v8::Local<v8::Value> DoGetResult() override;

        virtual void DoExecute(nanodbc::connection* connection) override;
    private:
        std::string database_name;
    };

}

#endif /* DATABASENAMENANODBCASYNCWORKER_HH */

