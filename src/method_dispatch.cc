#include "method_dispatch.hh"

#include "fetch.hh"

namespace NC {


template<>
nc_string_t call_method(
        ConnectionMethodTag<ConnectionCommands::dbms_name>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<>&) {
    return (*owner)([&](nanodbc::connection& connection) {
       return connection.dbms_name();
    });
}

template<>
bool call_method(
        ConnectionMethodTag<ConnectionCommands::is_connected>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<>&) {
    return (*owner)([&](nanodbc::connection& connection) {
       return connection.connected();
    });
}

template<>
nc_string_t call_method(
        ConnectionMethodTag<ConnectionCommands::dbms_version>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<>&) {
    return (*owner)([&](nanodbc::connection& connection) {
       return connection.dbms_version();
    });
}

template<>
nc_string_t call_method(
        ConnectionMethodTag<ConnectionCommands::catalog_name>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<>&) {
    return (*owner)([&](nanodbc::connection& connection) {
       return connection.catalog_name();
    });
}

template<>
nc_string_t call_method(
        ConnectionMethodTag<ConnectionCommands::database_name>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<>&) {
    return (*owner)([&](nanodbc::connection& connection) {
       return connection.database_name();
    });
}

template<>
nc_string_t call_method(
        ConnectionMethodTag<ConnectionCommands::driver_name>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<>&) {
    return (*owner)([&](nanodbc::connection& connection) {
       return connection.driver_name();
    });
}

template<>
nc_null_t call_method(
        ConnectionMethodTag<ConnectionCommands::connect>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<nc_string_t, nc_long_t>& args) {
    (*owner)([&](nanodbc::connection& connection) {
        connection.connect(std::get<0>(args), std::get<1>(args));
    });
    return nc_null_t {};
}

template<>
nc_null_t call_method(
        ConnectionMethodTag<ConnectionCommands::connect>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<nc_string_t>& args) {
    (*owner)([&](nanodbc::connection& connection) {
        connection.connect(std::get<0>(args));
    });
    return nc_null_t {};
}

template<>
nc_null_t call_method(
        ConnectionMethodTag<ConnectionCommands::disconnect>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<>&) {
    (*owner)([&](nanodbc::connection& connection) {
        connection.disconnect();
    });
    return nc_null_t {};
}

template<>
nc_result_t call_method(
        ConnectionMethodTag<ConnectionCommands::query>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<QueryArguments>& args) {
    return (*owner)([&](nanodbc::connection& connection) {
        const QueryArguments& qargs = std::get<0>(args);
        nanodbc::result result = nanodbc::execute(
            connection, qargs.query, qargs.batch_size, qargs.timeout);
        return fetch_result_eagerly(&result);
    });
}

template<>
nc_null_t call_method(
        ConnectionMethodTag<ConnectionCommands::execute>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<nc_string_t>& args) {
    (*owner)([&](nanodbc::connection& connection) {
        nanodbc::just_execute(connection, std::get<0>(args));
    });
    return nc_null_t {};
}

template<>
nc_null_t call_method(
        StatementMethodTag<StatementCommands::prepare>,
        std::shared_ptr<ConnectionAwareStatement> owner,
        const std::tuple<nc_string_t>& args) {
    owner->Prepare(std::get<0>(args));
    return nc_null_t {};
}

template<>
nc_result_t call_method(
        StatementMethodTag<StatementCommands::query>,
        std::shared_ptr<ConnectionAwareStatement> owner,
        const std::tuple<std::vector<nc_variant_t>>& args) {
    return owner->Query(std::get<0>(args));
}

template<>
nc_null_t call_method(
        StatementMethodTag<StatementCommands::execute>,
        std::shared_ptr<ConnectionAwareStatement> owner,
        const std::tuple<std::vector<nc_variant_t>>& args) {
    owner->Execute(std::get<0>(args));
    return nc_null_t {};
}

}  // namespace NC
