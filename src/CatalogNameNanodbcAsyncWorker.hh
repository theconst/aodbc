
/* 
 * File:   CatalogNameNanodbcAsyncWorker.hh
 * Author: kko
 *
 * Created on August 28, 2018, 8:16 PM
 */

#ifndef CATALOGNAMENANODBCASYNCWORKER_HH
#define CATALOGNAMENANODBCASYNCWORKER_HH

#include <memory>

#include "NanobcAsyncWorker.hh"

#include "nan.h"

namespace AODBC {

    using namespace AODBC;

    class CatalogNameNanodbcAsyncWorker final : public NanodbcAsyncWorker {
    public:
        explicit CatalogNameNanodbcAsyncWorker(
            std::shared_ptr< UVMonitor<nanodbc::connection> > connection_monitor,
            Nan::Callback* callback
        );

        CatalogNameNanodbcAsyncWorker(const CatalogNameNanodbcAsyncWorker& orig) = delete;

        CatalogNameNanodbcAsyncWorker(CatalogNameNanodbcAsyncWorker&& orig) = delete;

        virtual ~CatalogNameNanodbcAsyncWorker() = default;

    protected:

        virtual v8::Local<v8::Value> DoGetResult() override;

        virtual void DoExecute(nanodbc::connection* connection) override;
    private:
        std::string catalog_name;
    };

}

#endif /* CATALOGNAMENANODBCASYNCWORKER_HH */

