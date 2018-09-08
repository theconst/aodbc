#ifndef METHODDISPATCH_HH
#define METHODDISPATCH_HH

#include "sqltypes.hh"
#include "fetch.hh"

namespace AODBC {

using AODBC::QueryArguments;

template <
    typename OwnerT,
    typename ResultT,
    typename MethodT,
    typename ArgsTuple
>
ResultT call_method(MethodT tag, OwnerT* owner, ArgsTuple args);


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
    CommandNames value = tag;
};

template<>
sql_string_t call_method(
        MethodTag<CommandNames::dbms_name>,
        UVMonitor<nanodbc::connection>* owner,
        std::tuple<>) {
    return (*owner)([&](nanodbc::connection& connection) {
       return connection.dbms_name();
    });
}

template<>
bool call_method(
        MethodTag<CommandNames::is_connected>,
        UVMonitor<nanodbc::connection>* owner,
        std::tuple<>) {
    return (*owner)([&](nanodbc::connection& connection) {
       return connection.connected();
    });
}

template<>
sql_string_t call_method(
        MethodTag<CommandNames::dbms_version>,
        UVMonitor<nanodbc::connection>* owner,
        std::tuple<>) {
    return (*owner)([&](nanodbc::connection& connection) {
       return connection.dbms_version();
    });
}

template<>
sql_string_t call_method(
        MethodTag<CommandNames::catalog_name>,
        UVMonitor<nanodbc::connection>* owner,
        std::tuple<>) {
    return (*owner)([&](nanodbc::connection& connection) {
       return connection.catalog_name();
    });
}

template<>
sql_string_t call_method(
        MethodTag<CommandNames::database_name>,
        UVMonitor<nanodbc::connection>* owner,
        std::tuple<>) {
    return (*owner)([&](nanodbc::connection& connection) {
       return connection.database_name();
    });
}

template<>
sql_string_t call_method(
        MethodTag<CommandNames::driver_name>,
        UVMonitor<nanodbc::connection>* owner,
        std::tuple<>) {
    return (*owner)([&](nanodbc::connection& connection) {
       return connection.driver_name();
    });
}

template<>
sql_null_t call_method(
        MethodTag<CommandNames::connect>,
        UVMonitor<nanodbc::connection>* owner,
        std::tuple<sql_string_t, sql_long_t> args) {
    (*owner)([&](nanodbc::connection& connection) {
        connection.connect(std::get<0>(args), std::get<1>(args));
    });
    return boost::blank {};
}

template<>
sql_null_t call_method(
        MethodTag<CommandNames::disconnect>,
        UVMonitor<nanodbc::connection>* owner,
        std::tuple<>) {
    (*owner)([&](nanodbc::connection& connection) {
        connection.disconnect();
    });
    return sql_null_t {};
}

template<>
AODBC::sql_result_t call_method(
        MethodTag<CommandNames::query>,
        UVMonitor<nanodbc::connection>* owner,
        std::tuple<QueryArguments> args) {
    return (*owner)([&](nanodbc::connection& connection) {
        const QueryArguments& qargs = std::get<0>(args);
        nanodbc::result result = nanodbc::execute(
            connection, qargs.query, qargs.batch_size, qargs.timeout);
        return fetch_result_eagerly(&result);
    });
}

template<>
sql_null_t call_method(
        MethodTag<CommandNames::execute>,
        UVMonitor<nanodbc::connection>* owner,
        std::tuple<sql_string_t> args) {
    (*owner)([&](nanodbc::connection& connection) {
        nanodbc::just_execute(connection, std::get<0>(args));
    });
    return boost::blank {};
}


}  // namespace AODBC

#endif  /* METHODDISPATCH_HH */
