#ifndef METHODDISPATCH_HH
#define METHODDISPATCH_HH

#include "nctypes.hh"
#include "fetch.hh"

namespace NC {

using NC::QueryArguments;

template <
    typename OwnerT,
    typename ResultT,
    typename MethodT,
    typename ArgsTuple
>
ResultT call_method(MethodT tag, std::shared_ptr<OwnerT> owner,
    const ArgsTuple& args);


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
    execute
};

template<ConnectionCommands tag>
struct MethodTag {
    static constexpr ConnectionCommands value = tag;
};

template<>
nc_string_t call_method(
        MethodTag<ConnectionCommands::dbms_name>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<>&) {
    return (*owner)([&](nanodbc::connection& connection) {
       return connection.dbms_name();
    });
}

template<>
bool call_method(
        MethodTag<ConnectionCommands::is_connected>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<>&) {
    return (*owner)([&](nanodbc::connection& connection) {
       return connection.connected();
    });
}

template<>
nc_string_t call_method(
        MethodTag<ConnectionCommands::dbms_version>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<>&) {
    return (*owner)([&](nanodbc::connection& connection) {
       return connection.dbms_version();
    });
}

template<>
nc_string_t call_method(
        MethodTag<ConnectionCommands::catalog_name>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<>&) {
    return (*owner)([&](nanodbc::connection& connection) {
       return connection.catalog_name();
    });
}

template<>
nc_string_t call_method(
        MethodTag<ConnectionCommands::database_name>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<>&) {
    return (*owner)([&](nanodbc::connection& connection) {
       return connection.database_name();
    });
}

template<>
nc_string_t call_method(
        MethodTag<ConnectionCommands::driver_name>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<>&) {
    return (*owner)([&](nanodbc::connection& connection) {
       return connection.driver_name();
    });
}

template<>
nc_null_t call_method(
        MethodTag<ConnectionCommands::connect>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<nc_string_t, nc_long_t>& args) {
    (*owner)([&](nanodbc::connection& connection) {
        connection.connect(std::get<0>(args), std::get<1>(args));
    });
    return nc_null_t {};
}

template<>
nc_null_t call_method(
        MethodTag<ConnectionCommands::connect>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<nc_string_t>& args) {
    (*owner)([&](nanodbc::connection& connection) {
        connection.connect(std::get<0>(args));
    });
    return nc_null_t {};
}

template<>
nc_null_t call_method(
        MethodTag<ConnectionCommands::disconnect>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<>&) {
    (*owner)([&](nanodbc::connection& connection) {
        connection.disconnect();
    });
    return nc_null_t {};
}

template<>
nc_result_t call_method(
        MethodTag<ConnectionCommands::query>,
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
        MethodTag<ConnectionCommands::execute>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<nc_string_t>& args) {
    (*owner)([&](nanodbc::connection& connection) {
        nanodbc::just_execute(connection, std::get<0>(args));
    });
    return nc_null_t {};
}


}  // namespace NC

#endif  /* METHODDISPATCH_HH */
