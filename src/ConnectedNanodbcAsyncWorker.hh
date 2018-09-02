#ifndef CONNECTEDNANODBCASYNCWORKER_HH
#define CONNECTEDNANODBCASYNCWORKER_HH

#include <memory>

#include "nan.h"

#include "NanobcAsyncWorker.hh"

namespace AODBC {

using AODBC::UVMonitor;

class ConnectedNanodbcAsyncWorker final : public NanodbcAsyncWorker {
 public:
  explicit ConnectedNanodbcAsyncWorker(
      std::shared_ptr<UVMonitor<nanodbc::connection>> connection_monitor,
      Nan::Callback *callback);

  ConnectedNanodbcAsyncWorker(const ConnectedNanodbcAsyncWorker &orig) = delete;
  ConnectedNanodbcAsyncWorker(ConnectedNanodbcAsyncWorker &&orig) = delete;

  virtual ~ConnectedNanodbcAsyncWorker() = default;
 protected:
  v8::Local<v8::Value> DoGetResult() override;
  void DoExecute(nanodbc::connection *connection) override;

 private:
  bool connected;
};

}  // namespace AODBC

#endif /* CONNECTEDNANODBCASYNCWORKER_HH */
