
/* 
 * File:   DBMSVersionNanodbcAsyncWorker.hh
 * Author: kko
 *
 * Created on August 28, 2018, 8:16 PM
 */

#ifndef DBMSVERSIONNANODBCASYNCWORKER_HH
#define DBMSVERSIONNANODBCASYNCWORKER_HH

#include <memory>

#include "NanobcAsyncWorker.hh"

#include "nan.h"

namespace AODBC {

    using namespace AODBC;

    class DBMSVersionNanodbcAsyncWorker final : public NanodbcAsyncWorker {
    public:
        explicit DBMSVersionNanodbcAsyncWorker(
            std::shared_ptr< UVMonitor<nanodbc::connection> > connection_monitor,
            Nan::Callback* callback
        );

        DBMSVersionNanodbcAsyncWorker(const DBMSVersionNanodbcAsyncWorker& orig) = delete;

        DBMSVersionNanodbcAsyncWorker(DBMSVersionNanodbcAsyncWorker&& orig) = delete;

        virtual ~DBMSVersionNanodbcAsyncWorker() = default;

    protected:

        virtual v8::Local<v8::Value> DoGetResult() override;

        virtual void DoExecute(nanodbc::connection* connection) override;
    private:
        std::string dbms_version;
    };

}

#endif /* DBMSVERSIONNANODBCASYNCWORKER_HH */

