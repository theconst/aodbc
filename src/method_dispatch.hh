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


enum struct CommandNames {
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

template<CommandNames tag>
struct MethodTag {
    static constexpr CommandNames value = tag;
};

template<>
nc_string_t call_method(
        MethodTag<CommandNames::dbms_name>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<>&) {
    return (*owner)([&](nanodbc::connection& connection) {
       return connection.dbms_name();
    });
}

template<>
bool call_method(
        MethodTag<CommandNames::is_connected>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<>&) {
    return (*owner)([&](nanodbc::connection& connection) {
       return connection.connected();
    });
}

template<>
nc_string_t call_method(
        MethodTag<CommandNames::dbms_version>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<>&) {
    return (*owner)([&](nanodbc::connection& connection) {
       return connection.dbms_version();
    });
}

template<>
nc_string_t call_method(
        MethodTag<CommandNames::catalog_name>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<>&) {
    return (*owner)([&](nanodbc::connection& connection) {
       return connection.catalog_name();
    });
}

template<>
nc_string_t call_method(
        MethodTag<CommandNames::database_name>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<>&) {
    return (*owner)([&](nanodbc::connection& connection) {
       return connection.database_name();
    });
}

template<>
nc_string_t call_method(
        MethodTag<CommandNames::driver_name>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<>&) {
    return (*owner)([&](nanodbc::connection& connection) {
       return connection.driver_name();
    });
}

template<>
nc_null_t call_method(
        MethodTag<CommandNames::connect>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<nc_string_t, nc_long_t>& args) {
    (*owner)([&](nanodbc::connection& connection) {
        connection.connect(std::get<0>(args), std::get<1>(args));
    });
    return nc_null_t {};
}

template<>
nc_null_t call_method(
        MethodTag<CommandNames::connect>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<nc_string_t>& args) {
    (*owner)([&](nanodbc::connection& connection) {
        connection.connect(std::get<0>(args));
    });
    return nc_null_t {};
}

template<>
nc_null_t call_method(
        MethodTag<CommandNames::disconnect>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<>&) {
    (*owner)([&](nanodbc::connection& connection) {
        connection.disconnect();
    });
    return nc_null_t {};
}

template<>
nc_result_t call_method(
        MethodTag<CommandNames::query>,
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
        MethodTag<CommandNames::execute>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<nc_string_t>& args) {
    (*owner)([&](nanodbc::connection& connection) {
        nanodbc::just_execute(connection, std::get<0>(args));
    });
    return nc_null_t {};
}


}  // namespace NC

#endif  /* METHODDISPATCH_HH */
