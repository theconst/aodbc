#ifndef SQLTYPES_HH
#define SQLTYPES_HH

#include <vector>
#include "boost/variant.hpp"
#include "boost/blank.hpp"

#include "v8.h"
#include "nan.h"

#include "nanodbc.h"

namespace AODBC {

enum struct DateTypes {
    date, time, datetime
};

template <DateTypes t>
struct DateTag {
    const DateTypes value = t;
};

typedef std::vector<u_int8_t> sql_binary_t;

typedef boost::blank sql_null_t;

typedef nanodbc::string sql_string_t;

typedef nanodbc::date sql_date_t;

typedef nanodbc::time sql_time_t;

typedef nanodbc::timestamp sql_timestamp_t;

typedef double sql_number_t;

typedef long sql_long_t;  // NOLINT(runtime/int)

typedef boost::variant<
    sql_null_t,
    sql_number_t,
    sql_string_t,
    sql_date_t,
    sql_time_t,
    sql_timestamp_t,
    sql_binary_t
> sql_column_t;

typedef nanodbc::string sql_col_name_t;
typedef std::pair<sql_col_name_t, sql_column_t> sql_col_t;
typedef std::vector<sql_col_t> sql_row_t;
typedef std::vector<sql_row_t> sql_result_t;

}  // namespace AODBC

#endif /* SQLRESULTSET_HH */

