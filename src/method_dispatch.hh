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
};


template<>
nanodbc::string call_method(
        MethodTag<CommandNames::dbms_name>,
        UVMonitor<nanodbc::connection>* owner,
        std::tuple<>) {
    Synchronized lock {owner};
    return owner->get()->dbms_name();
}

template<>
bool call_method(
        MethodTag<CommandNames::is_connected>,
        UVMonitor<nanodbc::connection>* owner,
        std::tuple<std::string, long> args) {  // NOLINT(runtime/int)
    Synchronized lock {owner};
    return owner->get()->connected();
}

template<>
nanodbc::string call_method(
        MethodTag<CommandNames::dbms_version>,
        UVMonitor<nanodbc::connection>* owner,
        std::tuple<>) {
    Synchronized lock {owner};
    return owner->get()->dbms_version();
}

template<>
nanodbc::string call_method(
        MethodTag<CommandNames::catalog_name>,
        UVMonitor<nanodbc::connection>* owner,
        std::tuple<>) {
    Synchronized lock {owner};
    return owner->get()->catalog_name();
}

template<>
nanodbc::string call_method(
        MethodTag<CommandNames::database_name>,
        UVMonitor<nanodbc::connection>* owner,
        std::tuple<std::string, long> args) {  // NOLINT(runtime/int)
    Synchronized lock {owner};
    return owner->get()->database_name();
}

template<>
nanodbc::string call_method(
        MethodTag<CommandNames::driver_name>,
        UVMonitor<nanodbc::connection>* owner,
        std::tuple<> tuple) {
    Synchronized lock {owner};
    return owner->get()->driver_name();
}

template<>
boost::blank call_method(
        MethodTag<CommandNames::connect>,
        UVMonitor<nanodbc::connection>* owner,
        std::tuple<std::string, long> args) {  // NOLINT(runtime/int)
    Synchronized lock {owner};
    owner->get()->connect(std::get<0>(args), std::get<1>(args));

    return boost::blank {};
}

template<>
boost::blank call_method(
        MethodTag<CommandNames::disconnect>,
        UVMonitor<nanodbc::connection>* owner,
        std::tuple<>) {
    Synchronized lock {owner};
    owner->get()->disconnect();

    return boost::blank {};
}

template<>
AODBC::sql_result_t call_method(
        MethodTag<CommandNames::query>,
        UVMonitor<nanodbc::connection>* owner,
        std::tuple<std::string> args) {  // NOLINT(runtime/int)
    Synchronized lock {owner};
    nanodbc::result result = nanodbc::execute(*owner->get(), std::get<0>(args));
    return fetch_result_eagerly(&result);
}

template<>
boost::blank call_method(
        MethodTag<CommandNames::execute>,
        UVMonitor<nanodbc::connection>* owner,
        std::tuple<std::string> args) {  // NOLINT(runtime/int)
    Synchronized lock {owner};
    nanodbc::just_execute(*owner->get(), std::get<0>(args));
    return boost::blank {};
}


}  // namespace AODBC

#endif  /* METHODDISPATCH_HH */
