#ifndef ARGUMENTS_HH
#define ARGUMENTS_HH

#include "nctypes.hh"


namespace NC {

struct QueryArguments {
    nc_string_t query;
    nc_long_t batch_size;
    nc_long_t timeout;

    explicit QueryArguments(
            nc_string_t query,
            nc_long_t batch_size = 1L,
            nc_long_t timeout = 0L) :
        query(std::move(query)),
        batch_size(batch_size),
        timeout(timeout) {
    }

    QueryArguments(const QueryArguments&) = default;
    QueryArguments(QueryArguments&&) = default;
    virtual ~QueryArguments() = default;
};

}  // namespace NC


#endif /* ARGUMETNS_HH */
