#include "JustExecuteNanodbcAsyncWorker.hh"

#include "sqltypes.hh"
#include "cpp_to_js_converters.hh"
#include "fetch.hh"

namespace AODBC {

using AODBC::UVMonitor;

JustExecuteNanodbcAsyncWorker::JustExecuteNanodbcAsyncWorker(
        std::shared_ptr< UVMonitor<nanodbc::connection>> connection_monitor,
        std::string&& query,
        Nan::Callback* callback) :
    NanodbcAsyncWorker(connection_monitor, callback),
    query(std::move(query)) {
}

void JustExecuteNanodbcAsyncWorker::DoExecute(nanodbc::connection* connection) {
    nanodbc::just_execute(*connection, query);
}


}  // namespace AODBC
