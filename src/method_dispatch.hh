#ifndef METHODDISPATCH_HH
#define METHODDISPATCH_HH

#include <memory>

#include "nctypes.hh"

#include "arguments.hh"

#include "ConnectionAwareStatement.hh"

#include "ConnectionCommands.hh"
#include "StatementCommands.hh"

namespace NC {

using NC::QueryArguments;

using NC::ConnectionCommands;
using NC::StatementCommands;

template<ConnectionCommands tag>
struct ConnectionMethodTag {
    static constexpr ConnectionCommands value = tag;
};

template<StatementCommands tag>
struct StatementMethodTag {
    static constexpr StatementCommands value = tag;
};

template <
    typename OwnerT,
    typename ResultT,
    typename MethodT,
    typename ArgsTupleT
>
inline ResultT call_method(MethodT tag, std::shared_ptr<OwnerT> owner,
    const ArgsTupleT& args);


template<>
nc_string_t call_method(
        ConnectionMethodTag<ConnectionCommands::dbms_name>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<>&);

template<>
bool call_method(
        ConnectionMethodTag<ConnectionCommands::is_connected>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<>&);

template<>
nc_string_t call_method(
        ConnectionMethodTag<ConnectionCommands::dbms_version>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<>&);

template<>
nc_string_t call_method(
        ConnectionMethodTag<ConnectionCommands::catalog_name>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<>&);

template<>
nc_string_t call_method(
        ConnectionMethodTag<ConnectionCommands::database_name>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<>&);

template<>
nc_string_t call_method(
        ConnectionMethodTag<ConnectionCommands::driver_name>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<>&);

template<>
nc_null_t call_method(
        ConnectionMethodTag<ConnectionCommands::connect>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<nc_string_t, nc_long_t>& args);

template<>
nc_null_t call_method(
        ConnectionMethodTag<ConnectionCommands::connect>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<nc_string_t>& args);

template<>
nc_null_t call_method(
        ConnectionMethodTag<ConnectionCommands::disconnect>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<>&);

template<>
nc_result_t call_method(
        ConnectionMethodTag<ConnectionCommands::query>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<QueryArguments>& args);

template<>
nc_null_t call_method(
        ConnectionMethodTag<ConnectionCommands::execute>,
        std::shared_ptr<UVMonitor<nanodbc::connection>> owner,
        const std::tuple<nc_string_t>& args);

template<>
nc_null_t call_method(
        StatementMethodTag<StatementCommands::prepare>,
        std::shared_ptr<ConnectionAwareStatement> owner,
        const std::tuple<nc_string_t>& args);

template<>
nc_result_t call_method(
        StatementMethodTag<StatementCommands::query>,
        std::shared_ptr<ConnectionAwareStatement> owner,
        const std::tuple<PreparedStatementArguments>& args);

template<>
nc_null_t call_method(
        StatementMethodTag<StatementCommands::execute>,
        std::shared_ptr<ConnectionAwareStatement> owner,
        const std::tuple<PreparedStatementArguments>& args);

template<>
nc_null_t call_method(
        StatementMethodTag<StatementCommands::close>,
        std::shared_ptr<ConnectionAwareStatement> owner,
        const std::tuple<>&);

}  // namespace NC

#endif  /* METHODDISPATCH_HH */
