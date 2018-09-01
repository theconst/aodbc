#ifndef DRIVERNAMENANODBCASYNCWORKER_HH
#define DRIVERNAMENANODBCASYNCWORKER_HH

#include <memory>

#include "nan.h"

#include "NanobcAsyncWorker.hh"


namespace AODBC {

    using namespace AODBC;

    class DriverNameNanodbcAsyncWorker final : public NanodbcAsyncWorker {
    public:
        explicit DriverNameNanodbcAsyncWorker(
            std::shared_ptr< UVMonitor<nanodbc::connection> > connection_monitor,
            Nan::Callback* callback
        );

        DriverNameNanodbcAsyncWorker(const DriverNameNanodbcAsyncWorker& orig) = delete;

        DriverNameNanodbcAsyncWorker(DriverNameNanodbcAsyncWorker&& orig) = delete;

        virtual ~DriverNameNanodbcAsyncWorker() = default;

    protected:

        virtual v8::Local<v8::Value> DoGetResult() override;

        virtual void DoExecute(nanodbc::connection* connection) override;
    private:
        std::string driver_name;
    };

}

#endif /* DRIVERNAMENANODBCASYNCWORKER_HH */

