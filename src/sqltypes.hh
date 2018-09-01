#ifndef SQLTYPES_HH
#define SQLTYPES_HH

#include <vector>
#include "boost/variant.hpp"
#include "boost/blank.hpp"

#include "v8.h"
#include "nan.h"

#include "nanodbc.h"
//TODO: give better names 

namespace AODBC {
    
using namespace AODBC;

typedef std::vector<u_int8_t> binary_t;

typedef boost::variant<
    boost::blank,
    double,
    nanodbc::string,
    nanodbc::date,
    nanodbc::time,
    nanodbc::timestamp,
    binary_t
> sql_column_t;

typedef nanodbc::string sql_col_name_t;
typedef std::pair<sql_col_name_t, sql_column_t> sql_col_t;
typedef std::vector<sql_col_t> sql_row_t;
typedef std::vector<sql_row_t> sql_result_t;

}

#endif /* SQLRESULTSET_HH */

