/*
 * Contains definitions for argument types of various functions 
 */
#ifndef ARGUMENTS_HH
#define ARGUMENTS_HH

#include "sqltypes.hh"


namespace AODBC {

struct QueryArguments {
    sql_string_t query;
    sql_long_t batch_size;
    sql_long_t timeout;

    explicit QueryArguments(
            sql_string_t query,
            sql_long_t batch_size,
            sql_long_t timeout) :
        query(std::move(query)),
        batch_size(batch_size),
        timeout(timeout) {
    }

    QueryArguments(const QueryArguments&) = default;
    QueryArguments(QueryArguments&&) = default;
    virtual ~QueryArguments() = default;
};

}  // namespace AODBC


#endif /* ARGUMETNS_HH */
