#include "fetch.hh"

#include "sql.h"
#include "sqlext.h"

#include "nctypes.hh"

namespace NC {

nc_result_t fetch_result_eagerly(nanodbc::result* result) {
    nc_result_t sql_result {};
    for (auto row_no = 0; result->next(); ++row_no) {
        nc_row_t row = nc_row_t {};

        for (auto col_no = 0; col_no < result->columns(); ++col_no) {
            const nc_string_t& column_name = result->column_name(col_no);

            if (result->is_null(column_name)) {
                nc_variant_t blank = boost::blank();
                row.emplace_back(column_name, blank);

                continue;
            }

            auto datatype = result->column_datatype(col_no);
            // TODO(kko): switch on c types - they will eliminate most cases?
            switch (datatype) {
            case SQL_TINYINT:
            case SQL_INTEGER:
            case SQL_SMALLINT:
            case SQL_BIT:       // TODO(kko): check if this works
            {
                nc_variant_t intcol { result->get<int>(col_no) };
                row.emplace_back(column_name, intcol);
            }
                break;
            case SQL_FLOAT:
            {
                nc_variant_t floatcol { result->get<float>(col_no) };
                row.emplace_back(column_name, floatcol);
            }
                break;
            case SQL_NUMERIC:
            case SQL_DECIMAL:
            case SQL_REAL:
            case SQL_DOUBLE:
            case SQL_BIGINT:  // TODO(kko): check if this works
            {
                nc_variant_t doublecol { result->get<double>(col_no) };
                row.emplace_back(column_name, doublecol);
            }
                break;
            case SQL_VARCHAR:
            case SQL_LONGVARCHAR:
            {
                nc_variant_t&& strcol { result->get<nc_string_t>(col_no) };
                row.emplace_back(column_name, strcol);
            }
                break;
            case SQL_TYPE_DATE:
            {
                nc_variant_t&& datecol { result->get<nc_date_t>(col_no) };
                row.emplace_back(column_name, datecol);
            }
                break;
            case SQL_TYPE_TIME:
            case SQL_TIME:
            {
                nc_variant_t&& timecol {result->get<nc_time_t>(col_no) };
                row.emplace_back(column_name, timecol);
            }
                break;
            case SQL_TYPE_TIMESTAMP:
            case SQL_TIMESTAMP:
            {
                nc_variant_t&& timestampcol
                    { result->get<nc_timestamp_t>(col_no) };
                row.emplace_back(column_name, timestampcol);
            }
                break;
            case SQL_BINARY:
            case SQL_VARBINARY:
            case SQL_LONGVARBINARY:
            // fallthough is intentional - try to handle
            // data as binary as last resort
            default:
            {
                nc_variant_t&& binarycol { result->get<nc_binary_t>(col_no) };
                row.emplace_back(column_name, binarycol);
            }
                break;
            }
        }
        sql_result.emplace_back(row);
    }
    return sql_result;
}

}  // namespace NC
