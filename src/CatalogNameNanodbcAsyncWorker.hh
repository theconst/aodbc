#ifndef CATALOGNAMENANODBCASYNCWORKER_HH
#define CATALOGNAMENANODBCASYNCWORKER_HH

#include <memory>

#include "nan.h"

#include "NanobcAsyncWorker.hh"

namespace AODBC {

using AODBC::UVMonitor;

class CatalogNameNanodbcAsyncWorker final : public NanodbcAsyncWorker {
 public:
    explicit CatalogNameNanodbcAsyncWorker(
        std::shared_ptr< UVMonitor<nanodbc::connection> > connection_monitor,
        Nan::Callback* callback);

    CatalogNameNanodbcAsyncWorker(
        const CatalogNameNanodbcAsyncWorker& orig) = delete;

    CatalogNameNanodbcAsyncWorker(
        CatalogNameNanodbcAsyncWorker&& orig) = delete;

    virtual ~CatalogNameNanodbcAsyncWorker() = default;
 protected:
    v8::Local<v8::Value> DoGetResult() override;
    void DoExecute(nanodbc::connection* connection) override;

 private:
    std::string catalog_name;
};

}

#endif /* CATALOGNAMENANODBCASYNCWORKER_HH */

