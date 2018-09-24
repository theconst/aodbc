#include "ConnectionAwareStatement.hh"

#include "fetch.hh"

#include <utility>
#include <cstddef>

#include "sql.h"

namespace NC {

class BindingVisitor final : public boost::static_visitor<> {
 private:
    nanodbc::statement* statement_ptr;
    std::size_t position;

 public:
    explicit BindingVisitor(
            nanodbc::statement* statement,
            std::size_t position) :
        statement_ptr(statement),
        position(position) {
    }

    BindingVisitor(BindingVisitor&&) = delete;
    BindingVisitor(const BindingVisitor&) = delete;
    ~BindingVisitor() = default;

    void operator()(const nc_null_t& v) const {
        statement_ptr->bind_null(position);
    }

    void operator()(const nc_string_t& str) const {
        statement_ptr->bind(position, str.c_str());
    }

    template<typename T>
    void operator()(const T& v) const {
        statement_ptr->bind(position, &v);
    }
};

ConnectionAwareStatement::ConnectionAwareStatement(
            std::shared_ptr<UVMonitor<nanodbc::connection>> m):
        connection_monitor(m),
        statement() {
}

void ConnectionAwareStatement::BindParameters(
        const nc_bindings_t& bound_parameters) {
    for (std::size_t pos = 0u; pos < bound_parameters.size(); ++pos) {
        BindingVisitor visitor {&statement, pos};
        boost::apply_visitor(visitor, bound_parameters[pos]);
    }
}

inline bool success(SQLRETURN rc) {
    return rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO;
}

void ConnectionAwareStatement::CloseCursor() {
// ORIGINAL CODE FROM nanodbc:
// if (statement.open())
// {
//   Looks like some
//   The ODBC cursor must be closed before subsequent executions, as described
//   here
//   http://msdn.microsoft.com/en-us/library/windows/desktop/ms713584%28v=vs.85%29.aspx
//
//   However, we don't necessarily want to call SQLCloseCursor() because that
//   will cause an invalid cursor state in the case
//   that no cursor is currently open.
//   A better solution is to use SQLFreeStmt() with
//   the SQL_CLOSE option, which has

//   SQLRetrun retcode = SQLCloseCursor(statement.native_statement_handle());
//   the same effect without the undesired limitations.
//   NANODBC_CALL_RC(SQLFreeStmt, rc, stmt_, SQL_CLOSE);
//   if (!success(rc))
//      NANODBC_THROW_DATABASE_ERROR(stmt_, SQL_HANDLE_STMT);
// }

// END OF ORIGINAL CODE
// Looks like for some databases we still need to close cursor
// SQLFreeStmt will have no effect in this case and SQLCloseCursor will
// not fire with error, because we know that it is still open.
#ifdef FREE_STMT_WORKAROUND_ON
    if (statement.open()) {
        SQLHANDLE handle = statement.native_statement_handle();
        SQLRETURN rc = SQLCloseCursor(handle);
        if (!success(rc)) {
            throw nanodbc::database_error(handle, SQL_HANDLE_STMT,
                std::string("Failed to close cursor"));
        }
    }
#endif
}

void ConnectionAwareStatement::Execute(
        const nc_bindings_t& bound_parameters,
        nc_long_t batch_size,
        nc_long_t timeout) {
    return connection_monitor->Synchronized([&](const nanodbc::connection&) {
        BindParameters(bound_parameters);
        statement.just_execute(batch_size, timeout);
    });
}

void ConnectionAwareStatement::Prepare(
        nc_string_t query_string,
        nc_long_t timeout) {
    connection_monitor->Synchronized([&](nanodbc::connection& conn) {
        statement.prepare(conn, query_string, timeout);
    });
}

nc_result_t ConnectionAwareStatement::Query(
        const nc_bindings_t& bound_parameters,
        nc_long_t batch_size,
        nc_long_t timeout) {
    return connection_monitor->Synchronized([&](const nanodbc::connection&) {
        BindParameters(bound_parameters);
        nanodbc::result result { statement.execute(batch_size, timeout) };
        nc_result_t fetched_result = fetch_result_eagerly(&result);
        CloseCursor();
        return fetched_result;
    });
}

void ConnectionAwareStatement::Close() {
    connection_monitor->Synchronized([&](const nanodbc::connection&) {
        statement.close();
    });
}

}  // namespace NC
