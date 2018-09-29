#include "fetch.hh"

#include "sql.h"
#include "sqlext.h"

#include "nctypes.hh"

namespace NC {

inline nc_variant_t get_value_at(nanodbc::result* result, int col_no) {
    if (result->is_null(col_no)) {
        return boost::blank();
    }

    int datatype { result->column_datatype(col_no) };
    switch (datatype) {
    case SQL_TINYINT:  // less than 16-bit
    case SQL_SMALLINT:
    case SQL_BIT:
        return result->get<int>(col_no);

    case SQL_INTEGER:  // 32-bit integer
        return result->get<nc_long_t>(col_no);

    case SQL_FLOAT:
        return result->get<float>(col_no);

    case SQL_REAL:
    case SQL_DOUBLE:
        return result->get<double>(col_no);

    case SQL_NUMERIC:   // higher-precision values stored as string
    case SQL_DECIMAL:
    case SQL_BIGINT:
        return result->get<nc_string_t>(col_no);

    case SQL_VARCHAR:
    case SQL_LONGVARCHAR:
        return result->get<nc_string_t>(col_no);

    case SQL_TYPE_DATE:
        return result->get<nc_date_t>(col_no);

    case SQL_TYPE_TIME:
    case SQL_TIME:
        return result->get<nc_time_t>(col_no);

    case SQL_TYPE_TIMESTAMP:
    case SQL_TIMESTAMP:
        return result->get<nc_timestamp_t>(col_no);

    case SQL_BINARY:
    case SQL_VARBINARY:
    case SQL_LONGVARBINARY:
    // fallthrough is intentional - try to handle
    // data as binary as last resort
    default:
        return result->get<nc_binary_t>(col_no);
    }
}

nc_result_t fetch_result_eagerly(nanodbc::result* result) {
    int columns = result->columns();

    std::vector<nc_string_t> column_names {};
    column_names.reserve(columns);
    for (nc_short_t i = 0; i < columns; ++i) {
        column_names.push_back(result->column_name(i));
    }

    nc_result_t sql_result {};
    sql_result.reserve(result->rowset_size());
    while (result->next()) {
        nc_row_t row = nc_row_t {};
        for (int i = 0; i < columns; ++i) {
            row.emplace_back(column_names[i], get_value_at(result, i));
        }
        sql_result.push_back(std::move(row));
    }
    return sql_result;
}

}  // namespace NC
