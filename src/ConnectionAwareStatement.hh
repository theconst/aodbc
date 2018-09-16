#ifndef CONNECTIONAWARESTATEMENT_HH
#define CONNECTIONAWARESTATEMENT_HH

#ifndef FREE_STMT_WORKAROUND
#define FREE_STMT_WORKAROUND
#endif


#include <memory>

#include "UVMonitor.hh"

#include "nanodbc.h"

#include "nctypes.hh"


namespace NC {

using NC::UVMonitor;


class ConnectionAwareStatement final {
 public:
    ConnectionAwareStatement(
        std::shared_ptr<UVMonitor<nanodbc::connection>> m);

    void execute(const std::vector<nc_variant_t>& bound_parameters,
            nc_long_t batch_size = 1L, nc_long_t timeout = 0L);

    void prepare(nc_string_t query_string, nc_long_t timeout = 0L);

    nc_result_t query(const std::vector<nc_variant_t>& bound_parameters,
            nc_long_t batch_size = 1L, nc_long_t timeout = 0L);

    ConnectionAwareStatement(const ConnectionAwareStatement&) = delete;
    ConnectionAwareStatement(ConnectionAwareStatement&&) = delete;
    ~ConnectionAwareStatement() = default;

 private:
    void bind_parameters(const std::vector<nc_variant_t>& bound_parameters);

    void close_cursor();

    std::shared_ptr<UVMonitor<nanodbc::connection>> connection_monitor;

    nanodbc::statement statement;
};

}  // namespace NC

#endif /* CONNECTIONAWARESTATEMENT_HH */
