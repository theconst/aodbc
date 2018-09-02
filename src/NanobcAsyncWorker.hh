#ifndef NANOBCWORKER_HH
#define NANOBCWORKER_HH

#include "nan.h"

#include "nanodbc.h"

#include "UVMonitor.hh"

namespace AODBC {

/* abstract */ class NanodbcAsyncWorker : public Nan::AsyncWorker  {
 public:
    NanodbcAsyncWorker(const NanodbcAsyncWorker&) = delete;
    NanodbcAsyncWorker(NanodbcAsyncWorker&&) = delete;

    void Execute() final;
    void HandleOKCallback() final;

    virtual ~NanodbcAsyncWorker() = default;
 protected:
    static const int NUMBER_OF_ARGS = 2;

    explicit NanodbcAsyncWorker(
        std::shared_ptr< UVMonitor<nanodbc::connection> >, Nan::Callback*);

    virtual void DoExecute(nanodbc::connection* connection) = 0;

    //! Do not forget to escape the scope - we cannot force this
    virtual v8::Local<v8::Value> DoGetResult() = 0;

    std::shared_ptr<UVMonitor<nanodbc::connection> > connection_monitor;

 private:
};

}  // namespace AODBC

#endif /* NANOBCWORKER_HH */

