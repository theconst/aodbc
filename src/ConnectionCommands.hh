#ifndef CONNECTIONCOMMANDS_HH
#define CONNECTIONCOMMANDS_HH

#include "nanodbc/nanodbc.h"

#include <sql.h>
#include <sqlext.h>

#include "errors.hh"
#include "fetch.hh"
#include "UVMonitor.hh"
#include "odbcutil.hh"
#include "arguments.hh"

namespace NC {

enum struct ConnectionCommands {
    dbms_name,
    is_connected,
    dbms_version,
    catalog_name,
    database_name,
    driver_name,
    connect,
    disconnect,
    query,
    execute,
    set_auto_commit
};

template<ConnectionCommands c>
struct ConnectionCommand {
};

template<>
struct ConnectionCommand<ConnectionCommands::dbms_name> {
    static nc_string_t Execute(
            UVMonitor<nanodbc::connection>* owner, const std::tuple<>&) {
        return owner->Synchronized([&](nanodbc::connection& connection) {
            return connection.dbms_name();
        });
    }
};

template<>
struct ConnectionCommand<ConnectionCommands::is_connected> {
    static bool Execute(
            UVMonitor<nanodbc::connection>* owner,
            const std::tuple<>&) {
        return owner->Synchronized([&](nanodbc::connection& connection) {
           return connection.connected();
        });
    }
};

template<>
struct ConnectionCommand<ConnectionCommands::dbms_version> {
    static nc_string_t Execute(
            UVMonitor<nanodbc::connection>* owner,
            const std::tuple<>&) {
        return owner->Synchronized([&](nanodbc::connection& connection) {
            return connection.dbms_version();
        });
    }
};

template<>
struct ConnectionCommand<ConnectionCommands::catalog_name> {
    static nc_string_t Execute(UVMonitor<nanodbc::connection>* owner,
            const std::tuple<>&) {
        return owner->Synchronized([&](nanodbc::connection& connection) {
            return connection.catalog_name();
        });
    }
};

template<>
struct ConnectionCommand<ConnectionCommands::database_name> {
    static nc_string_t Execute(UVMonitor<nanodbc::connection>* owner,
            const std::tuple<>&) {
        return owner->Synchronized([&](nanodbc::connection& connection) {
            return connection.database_name();
        });
    }
};

template<>
struct ConnectionCommand<ConnectionCommands::driver_name> {
    static nc_string_t Execute(UVMonitor<nanodbc::connection>* owner,
            const std::tuple<>&) {
        return owner->Synchronized([&](nanodbc::connection& connection) {
            return connection.driver_name();
        });
    }
};

template<>
struct ConnectionCommand<ConnectionCommands::connect> {
    static nc_null_t Execute(UVMonitor<nanodbc::connection>* owner,
            const std::tuple<nc_string_t, TimeoutArg>& args) {
        owner->Synchronized([&](nanodbc::connection& connection) {
            connection.connect(std::get<0>(args), std::get<1>(args));
        });
        return nc_null_t {};
    }

    static nc_null_t Execute(UVMonitor<nanodbc::connection>* owner,
            const std::tuple<nc_string_t>& args) {
            owner->Synchronized([&](nanodbc::connection& connection) {
            connection.connect(std::get<0>(args));
        });
        return nc_null_t {};
    }

    static nc_null_t Execute(UVMonitor<nanodbc::connection>* owner,
            std::tuple<nc_string_t, nc_string_t, nc_string_t> args) {
        owner->Synchronized([&](nanodbc::connection& connection) {
            connection.connect(std::get<0>(args),
                std::get<1>(args), std::get<2>(args));
        });
        return nc_null_t {};
    }

    static nc_null_t Execute(UVMonitor<nanodbc::connection>* owner,
            std::tuple<nc_string_t, nc_string_t, nc_string_t,
                TimeoutArg> args) {
        owner->Synchronized([&](nanodbc::connection& connection) {
            connection.connect(std::get<0>(args),
                std::get<1>(args), std::get<2>(args), std::get<3>(args));
        });
        return nc_null_t {};
    }
};

template<>
struct ConnectionCommand<ConnectionCommands::disconnect> {
    static nc_null_t Execute(UVMonitor<nanodbc::connection>* owner,
            const std::tuple<>&) {
        owner->Synchronized([&](nanodbc::connection& connection) {
            connection.disconnect();
        });
        return nc_null_t {};
    }
};

template<>
struct ConnectionCommand<ConnectionCommands::set_auto_commit> {
    static nc_null_t Execute(
            UVMonitor<nanodbc::connection>* owner,
            const std::tuple<bool>& args) {
        using NC::success;

        auto enable = std::get<0>(args);
        owner->Synchronized([&](nanodbc::connection& connection) {
            auto* handle = connection.native_dbc_handle();
            auto retcode = SQLSetConnectAttr(
                handle,
                SQL_ATTR_AUTOCOMMIT,
                (SQLPOINTER) (enable ? SQL_AUTOCOMMIT_ON : SQL_AUTOCOMMIT_OFF),
                SQL_NTS);

            if (!success(retcode)) {
                 throw nanodbc::database_error(handle, SQL_HANDLE_DBC,
                    std::string("Unable to set auto commit mode"));
            }
        });
        return nc_null_t {};
    }
};

template<>
struct ConnectionCommand<ConnectionCommands::query> {
    static nc_result_t Execute(UVMonitor<nanodbc::connection>* owner,
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
};

template<>
struct ConnectionCommand<ConnectionCommands::execute> {
    static nc_null_t Execute(
            UVMonitor<nanodbc::connection>* owner,
            const std::tuple<QueryArguments>& args) {
        owner->Synchronized([&](nanodbc::connection& connection) {
            const QueryArguments& qargs = std::get<0>(args);
            nanodbc::just_execute(connection, qargs.GetQuery(),
                qargs.GetBatchSize(), qargs.GetTimeout());
        });
        return nc_null_t {};
    }
};

}  // namespace NC


#endif /* CONNECTIONCOMMANDS_HH */
