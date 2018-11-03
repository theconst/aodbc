#ifndef CONNECTIONAWARESTATEMENT_HH
#define CONNECTIONAWARESTATEMENT_HH

#ifndef FREE_STMT_WORKAROUND_OFF
#define FREE_STMT_WORKAROUND_ON
#endif


#include <memory>

#include "nanodbc/nanodbc.h"

#include "UVMonitor.hh"
#include "nctypes.hh"


namespace NC {

using NC::UVMonitor;


class ConnectionAwareStatement final {
 public:
    ConnectionAwareStatement(
        std::shared_ptr<UVMonitor<nanodbc::connection>> m);

    void Prepare(nc_string_t query_string, nc_long_t timeout = 0L);

    void Execute(const nc_bindings_t& bound_parameters,
        nc_long_t batch_size = 1L, nc_long_t timeout = 0L);

    nc_result_t Query(const nc_bindings_t& bound_parameters,
        nc_long_t batch_size = 1L, nc_long_t timeout = 0L);

    bool IsOpen();

    void Close();

    ConnectionAwareStatement(const ConnectionAwareStatement&) = delete;
    ConnectionAwareStatement(ConnectionAwareStatement&&) = delete;
    virtual ~ConnectionAwareStatement() = default;

 private:
    void BindParameters(const nc_bindings_t& bound_parameters);

    void CloseCursor();

    std::shared_ptr<UVMonitor<nanodbc::connection>> connection_monitor;

    nanodbc::statement statement;
};

}  // namespace NC

#endif /* CONNECTIONAWARESTATEMENT_HH */
