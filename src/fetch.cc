#include "fetch.hh"

#include <sql.h>
#include "nctypes.hh"

namespace NC {

nc_result_t fetch_result_eagerly(nanodbc::result* result) {
    nc_result_t sql_result;
    for (auto row_no = 0; result->next(); ++row_no) {
        nc_row_t row = nc_row_t {};

        for (auto col_no = 0; col_no < result->columns(); ++col_no) {
            const nc_col_name_t& column_name = result->column_name(col_no);

            if (result->is_null(column_name)) {
                nc_column_t blank = boost::blank();
                row.emplace_back(column_name, blank);

                continue;
            }

            auto datatype = result->column_datatype(col_no);
            // TODO(kko): switch on c types - they will eliminate most cases?
            switch (datatype) {
            case SQL_INTEGER:
            case SQL_SMALLINT:
            {
                nc_column_t intcol = result->get<int>(col_no);
                row.emplace_back(column_name, intcol);
            }
                break;
            case SQL_FLOAT:
            {
                nc_column_t floatcol = result->get<float>(col_no);
                row.emplace_back(column_name, floatcol);
            }
                break;
            case SQL_NUMERIC:
            case SQL_DECIMAL:
            case SQL_REAL:
            case SQL_DOUBLE:
            {
                nc_column_t doublecol = result->get<double>(col_no);
                row.emplace_back(column_name, doublecol);
            }
                break;
            case SQL_VARCHAR:
            {
                nc_column_t strcol = result->get<nanodbc::string>(col_no);
                row.emplace_back(column_name, strcol);
            }
                break;
            case SQL_TYPE_DATE:
            {
                nc_column_t datecol = result->get<nanodbc::date>(col_no);
                row.emplace_back(column_name, datecol);
            }
                break;
            case SQL_TYPE_TIME:
            {
                nc_column_t timecol = result->get<nanodbc::time>(col_no);
                row.emplace_back(column_name, timecol);
            }
                break;
            case SQL_TYPE_TIMESTAMP:
            {
                nc_column_t timestampcol =
                    result->get<nanodbc::timestamp>(col_no);
                row.emplace_back(column_name, timestampcol);
            }
                break;
            default:
            {
                nc_column_t binarycol = result->get<nc_binary_t>(col_no);
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
