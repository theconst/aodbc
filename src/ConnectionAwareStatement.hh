#ifndef CONNECTIONAWARESTATEMENT_HH
#define CONNECTIONAWARESTATEMENT_HH


#include <memory>
#include <utility>
#include <cstddef>

#include "nan.h"

#include "UVMonitor.hh"

#include "nanodbc.h"

#include "nctypes.hh"

#include "fetch.hh"

#include <iostream>


namespace NC {

using NC::UVMonitor;


// TODO(kko) : examine lifetime one more time
class ConnectionAwareStatement final {
 private:
    class BindingVisitor final : public boost::static_visitor<> {
     private:
        nanodbc::statement* statement_ptr;
        std::size_t position;

     public:
        // TODO(kko): define in nctypes
        explicit BindingVisitor(
                nanodbc::statement* statement,
                std::size_t position) :
            statement_ptr(statement),
            position(position) {
        }

        BindingVisitor(BindingVisitor&&) = delete;
        BindingVisitor(const BindingVisitor&) = delete;
        virtual ~BindingVisitor() = default;

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

    std::shared_ptr<UVMonitor<nanodbc::connection>> connection_monitor;

    nanodbc::statement statement;

 public:
    ConnectionAwareStatement(
            std::shared_ptr<UVMonitor<nanodbc::connection>> m) :
        connection_monitor(m),
        statement() {
    }

    template <typename... T>
    void execute(const std::vector<nc_variant_t>& bound_parameters,
            nc_long_t batch_size = 1L, nc_long_t timeout = 0L) {
        return (*connection_monitor)([&](const nanodbc::connection&) {
            statement.reset_parameters();
            for (std::size_t pos = 0u; pos < bound_parameters.size(); ++pos) {
                BindingVisitor visitor {&statement, pos};
                boost::apply_visitor(visitor, bound_parameters[pos]);
            }
            statement.just_execute(batch_size, timeout);
        });
    }

    void prepare(nc_string_t query_string, nc_long_t timeout = 0) {
        (*connection_monitor)([&](nanodbc::connection& conn) {
            statement.prepare(conn, query_string, timeout);
        });
    }

    nc_result_t query(const std::vector<nc_variant_t>& bound_parameters,
            nc_long_t batch_size = 1L, nc_long_t timeout = 0L) {
        return (*connection_monitor)([&](const nanodbc::connection&) {
            statement.reset_parameters();
            for (std::size_t pos = 0u; pos < bound_parameters.size(); ++pos) {
                BindingVisitor visitor {&statement, pos};
                boost::apply_visitor(visitor, bound_parameters[pos]);
            }
            nanodbc::result result { statement.execute(batch_size, timeout) };

            return fetch_result_eagerly(&result);
        });
    }

    ConnectionAwareStatement(const ConnectionAwareStatement&) = delete;
    ConnectionAwareStatement(ConnectionAwareStatement&&) = delete;
    ~ConnectionAwareStatement() = default;
};

}  // namespace NC

#endif /* CONNECTIONAWARESTATEMENT_HH */
