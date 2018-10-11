#ifndef METHODDISPATCH_HH
#define METHODDISPATCH_HH

#include <memory>

#include "nctypes.hh"

#include "arguments.hh"

#include "fetch.hh"

#include "sql.h"
#include "sqlext.h"

#include "ConnectionAwareStatement.hh"
#include "ConnectionAwareTransaction.hh"
#include "ConnectionCommands.hh"
#include "StatementCommands.hh"
#include "TransactionCommands.hh"

#include "odbcutil.hh"


namespace NC {

using NC::QueryArguments;

using NC::ConnectionCommands;
using NC::StatementCommands;

inline nc_string_t call_method(
        ConnectionMethodTag<ConnectionCommands::dbms_name>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<>&) {
    return owner->Synchronized([&](nanodbc::connection& connection) {
       return connection.dbms_name();
    });
}

inline bool call_method(
        ConnectionMethodTag<ConnectionCommands::is_connected>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<>&) {
    return owner->Synchronized([&](nanodbc::connection& connection) {
       return connection.connected();
    });
}

inline nc_string_t call_method(
        ConnectionMethodTag<ConnectionCommands::dbms_version>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<>&) {
    return owner->Synchronized([&](nanodbc::connection& connection) {
        return connection.dbms_version();
    });
}

inline nc_string_t call_method(
        ConnectionMethodTag<ConnectionCommands::catalog_name>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<>&) {
    return owner->Synchronized([&](nanodbc::connection& connection) {
       return connection.catalog_name();
    });
}

inline nc_string_t call_method(
        ConnectionMethodTag<ConnectionCommands::database_name>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<>&) {
    return owner->Synchronized([&](nanodbc::connection& connection) {
       return connection.database_name();
    });
}

inline nc_string_t call_method(
        ConnectionMethodTag<ConnectionCommands::driver_name>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<>&) {
    return owner->Synchronized([&](nanodbc::connection& connection) {
       return connection.driver_name();
    });
}

inline nc_null_t call_method(
        ConnectionMethodTag<ConnectionCommands::connect>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<nc_string_t, TimeoutArg>& args) {
    owner->Synchronized([&](nanodbc::connection& connection) {
        connection.connect(std::get<0>(args), std::get<1>(args));
    });

    return nc_null_t {};
}

inline nc_null_t call_method(
        ConnectionMethodTag<ConnectionCommands::connect>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<nc_string_t>& args) {
    owner->Synchronized([&](nanodbc::connection& connection) {
        connection.connect(std::get<0>(args));
    });
    return nc_null_t {};
}

inline nc_null_t call_method(
        ConnectionMethodTag<ConnectionCommands::connect>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        std::tuple<nc_string_t, nc_string_t, nc_string_t> args) {
    owner->Synchronized([&](nanodbc::connection& connection) {
        connection.connect(std::get<0>(args),
            std::get<1>(args), std::get<2>(args));
    });
    return nc_null_t {};
}

inline nc_null_t call_method(
        ConnectionMethodTag<ConnectionCommands::connect>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        std::tuple<nc_string_t, nc_string_t, nc_string_t, TimeoutArg> args) {
    owner->Synchronized([&](nanodbc::connection& connection) {
        connection.connect(std::get<0>(args),
            std::get<1>(args), std::get<2>(args), std::get<3>(args));
    });
    return nc_null_t {};
}

inline nc_null_t call_method(
        ConnectionMethodTag<ConnectionCommands::disconnect>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<>&) {
    owner->Synchronized([&](nanodbc::connection& connection) {
        connection.disconnect();
    });
    return nc_null_t {};
}

inline nc_null_t call_method(
        ConnectionMethodTag<ConnectionCommands::set_auto_commit>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<bool>& args) {
    using NC::success;

    auto enable = std::get<0>(args);
    owner->Synchronized([&](nanodbc::connection& connection) {
        auto* handle = connection.native_dbc_handle();
        auto retcode = SQLSetConnectAttr(
            handle,
            SQL_ATTR_AUTOCOMMIT,
            // they just check for null, i guess
            (SQLPOINTER) (enable ? SQL_AUTOCOMMIT_ON : SQL_AUTOCOMMIT_OFF),
            SQL_NTS);

        if (!success(retcode)) {
            throw Error("Unable to set auto commit mode");
        }
    });

    return nc_null_t {};
}

inline nc_result_t call_method(
        ConnectionMethodTag<ConnectionCommands::query>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<QueryArguments>& args) {
    return owner->Synchronized([&](nanodbc::connection& connection) {
        const QueryArguments& qargs = std::get<0>(args);

        nanodbc::result result = nanodbc::execute(connection,
            qargs.GetQuery(),
            qargs.GetBatchSize(),
            qargs.GetTimeout());
        return fetch_result_eagerly(&result);
    });
}

inline nc_null_t call_method(
        ConnectionMethodTag<ConnectionCommands::execute>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<QueryArguments>& args) {
    owner->Synchronized([&](nanodbc::connection& connection) {
        const QueryArguments& qargs = std::get<0>(args);
        nanodbc::just_execute(connection, qargs.GetQuery(),
            qargs.GetBatchSize(), qargs.GetTimeout());
    });
    return nc_null_t {};
}

inline nc_null_t call_method(
        StatementMethodTag<StatementCommands::prepare>,
        std::shared_ptr<ConnectionAwareStatement> owner,
        const std::tuple<nc_string_t>& args) {
    owner->Prepare(std::get<0>(args));
    return nc_null_t {};
}

inline nc_result_t call_method(
        StatementMethodTag<StatementCommands::query>,
        std::shared_ptr<ConnectionAwareStatement> owner,
        const std::tuple<PreparedStatementArguments>& args) {
    const PreparedStatementArguments& psargs = std::get<0>(args);
    return owner->Query(psargs.GetBindings(), psargs.GetBatchSize(),
            psargs.GetTimeout());
}

inline nc_null_t call_method(
        StatementMethodTag<StatementCommands::execute>,
        std::shared_ptr<ConnectionAwareStatement> owner,
        const std::tuple<PreparedStatementArguments>& args) {
    const PreparedStatementArguments& psargs = std::get<0>(args);
    owner->Execute(psargs.GetBindings(), psargs.GetBatchSize(),
        psargs.GetTimeout());
    return nc_null_t {};
}

inline nc_null_t call_method(
        StatementMethodTag<StatementCommands::close>,
        std::shared_ptr<ConnectionAwareStatement> owner,
        const std::tuple<>&) {
    owner->Close();
    return nc_null_t {};
}

inline bool call_method(
        StatementMethodTag<StatementCommands::is_open>,
        std::shared_ptr<ConnectionAwareStatement> owner,
        const std::tuple<>&) {
    return owner->IsOpen();
}

inline nc_null_t call_method(
        TransactionMethodTag<TransactionCommands::begin>,
        std::shared_ptr<ConnectionAwareTransaction> owner,
        const std::tuple<>&) {
    owner->Begin();
    return nc_null_t {};
}

inline nc_null_t call_method(
        TransactionMethodTag<TransactionCommands::commit>,
        std::shared_ptr<ConnectionAwareTransaction> owner,
        const std::tuple<>&) {
    owner->Commit();

    return nc_null_t {};
}

inline nc_null_t call_method(
        TransactionMethodTag<TransactionCommands::rollback>,
        std::shared_ptr<ConnectionAwareTransaction> owner,
        const std::tuple<>&) {
    owner->Rollback();

    return nc_null_t {};
}

}  // namespace NC

#endif  /* METHODDISPATCH_HH */
