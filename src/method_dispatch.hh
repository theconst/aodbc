#ifndef METHODDISPATCH_HH
#define METHODDISPATCH_HH

#include "sqltypes.hh"
#include "fetch.hh"

namespace AODBC {

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
nanodbc::string call_method(
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
nanodbc::string call_method(
        MethodTag<CommandNames::dbms_version>,
        UVMonitor<nanodbc::connection>* owner,
        std::tuple<>) {
    return (*owner)([&](nanodbc::connection& connection) {
       return connection.dbms_version();
    });
}

template<>
nanodbc::string call_method(
        MethodTag<CommandNames::catalog_name>,
        UVMonitor<nanodbc::connection>* owner,
        std::tuple<>) {
    return (*owner)([&](nanodbc::connection& connection) {
       return connection.catalog_name();
    });
}

template<>
nanodbc::string call_method(
        MethodTag<CommandNames::database_name>,
        UVMonitor<nanodbc::connection>* owner,
        std::tuple<>) {
    return (*owner)([&](nanodbc::connection& connection) {
       return connection.database_name();
    });
}

template<>
nanodbc::string call_method(
        MethodTag<CommandNames::driver_name>,
        UVMonitor<nanodbc::connection>* owner,
        std::tuple<>) {
    return (*owner)([&](nanodbc::connection& connection) {
       return connection.driver_name();
    });
}

template<>
boost::blank call_method(
        MethodTag<CommandNames::connect>,
        UVMonitor<nanodbc::connection>* owner,
        std::tuple<sql_string_t, sql_long_t> args) {
    (*owner)([&](nanodbc::connection& connection) {
        connection.connect(std::get<0>(args), std::get<1>(args));
    });
    return boost::blank {};
}

template<>
boost::blank call_method(
        MethodTag<CommandNames::disconnect>,
        UVMonitor<nanodbc::connection>* owner,
        std::tuple<>) {
    (*owner)([&](nanodbc::connection& connection) {
        connection.disconnect();
    });
    return boost::blank {};
}

template<>
AODBC::sql_result_t call_method(
        MethodTag<CommandNames::query>,
        UVMonitor<nanodbc::connection>* owner,
        std::tuple<sql_string_t> args) {
    return (*owner)([&](nanodbc::connection& connection) {
        nanodbc::result result = nanodbc::execute(
            connection, std::get<0>(args));
        return fetch_result_eagerly(&result);
    });
}

template<>
boost::blank call_method(
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
