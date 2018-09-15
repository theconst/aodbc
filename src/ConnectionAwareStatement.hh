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


namespace NC {

using NC::UVMonitor;

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

        template<typename T>
        void operator()(const T& v) const {
            statement_ptr->bind(position, &v);
        }
    };

    std::shared_ptr<UVMonitor<nanodbc::connection>> connection_monitor;

    nanodbc::statement statement;

 public:
    template <typename... T>
    ConnectionAwareStatement(
            std::shared_ptr<UVMonitor<nanodbc::connection>>
            connection_monitor, T&&... args) :
        connection_monitor(connection_monitor),
        statement(std::forward(args)...) {
    }

    // TODO(kko): move constants
    template <typename... T>
    void execute(const std::vector<nc_variant_t>& bound_parameters,
            nc_long_t batch_size = 1L, nc_long_t timeout = 0L) {
        // TODO(kko) : consider overall sync
        return (*connection_monitor)([&](const nanodbc::connection&) {
           for (std::size_t pos = 0u; pos < bound_parameters.size(); ++pos) {
                BindingVisitor visitor {&statement, pos};
                boost::apply_visitor(visitor, bound_parameters[pos]);
            }
            statement.just_execute(batch_size, timeout);
        });
    }

    void prepare(nc_string_t query_string, nc_long_t timeout = 0) {
        (*connection_monitor)([&](const nanodbc::connection& conn) {
            statement.prepare(query_string, timeout);
        });
    }

    nc_result_t query(const std::vector<nc_variant_t>& bound_parameters,
            nc_long_t batch_size = 1L, nc_long_t timeout = 0L) {
        return (*connection_monitor)([&](const nanodbc::connection&) {
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
