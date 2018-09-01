#ifndef CATALOGNAMENANODBCASYNCWORKER_HH
#define CATALOGNAMENANODBCASYNCWORKER_HH

#include <memory>

#include "nan.h"

#include "NanobcAsyncWorker.hh"

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

