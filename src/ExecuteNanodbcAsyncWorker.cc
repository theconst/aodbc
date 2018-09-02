#include "ExecuteNanodbcAsyncWorker.hh"

#include "sqltypes.hh"
#include "converters.hh"
#include "fetch.hh"

namespace AODBC {

using AODBC::UVMonitor;

ExecuteNanodbcAsyncWorker::ExecuteNanodbcAsyncWorker(
        std::shared_ptr< UVMonitor<nanodbc::connection>> connection_monitor,
        std::string&& query,
        Nan::Callback* callback) :
    NanodbcAsyncWorker(connection_monitor, callback),
    query(std::move(query)) {
}

void ExecuteNanodbcAsyncWorker::DoExecute(nanodbc::connection* connection) {
    nanodbc::result result = nanodbc::execute(*connection, query);

    sql_result = fetch_result_eagerly(&result);
}

v8::Local<v8::Value> ExecuteNanodbcAsyncWorker::DoGetResult() {
    Nan::EscapableHandleScope scope;
    return scope.Escape(ConvertNanodbcTypeToJsObject(sql_result));
}

}  // namespace AODBC
