
/* 
 * File:   ExecuteNanodbcAsyncWorker.cc
 * Author: kko
 * 
 * Created on August 28, 2018, 10:09 PM
 */

#include <sql.h>
//#include <sqlext.h> TODO: cover extensions - like binary type

#include "ExecuteNanodbcAsyncWorker.hh"
#include "converters.hh"

// whats left is progress worker and async statements

// todo: make a stateful cursor executor

namespace AODBC {

    using namespace AODBC;

    ExecuteNanodbcAsyncWorker::ExecuteNanodbcAsyncWorker(
            std::shared_ptr< UVMonitor<nanodbc::connection> > connection_monitor,
            std::string&& query,
            Nan::Callback* callback) :
        NanodbcAsyncWorker(connection_monitor, callback),
        query(std::move(query)),
        rows() {
        
    }

    void ExecuteNanodbcAsyncWorker::DoExecute(nanodbc::connection* connection) {
        nanodbc::result result = nanodbc::execute(*connection, query);
        
        //todo : we can make it lazy
        for (int row_no = 0; result.next(); ++row_no) {
            sql_row_t row = sql_row_t();

            for (short col_no = 0; col_no < result.columns(); ++col_no) {
                
                const nanodbc::string& column_name = result.column_name(col_no);
                
                if (result.is_null(column_name)) {
                    {
                        sql_col_value_t blank = boost::blank();
                        row.emplace_back(column_name, blank);
                    }
                    break;
                }
                
                int datatype = result.column_datatype(col_no);
                //TODO: switch on c types - they will eliminate most cases?
                switch(datatype)  {
                case SQL_INTEGER:
                case SQL_SMALLINT:
                    {
                        sql_col_value_t intcol = result.get<int>(col_no);
                        row.emplace_back(column_name, intcol);
                    }
                    break;
                case SQL_FLOAT:
                    {
                        sql_col_value_t floatcol = result.get<float>(col_no);
                        row.emplace_back(column_name, floatcol);
                    }
                    break;
                case SQL_NUMERIC:
                case SQL_DECIMAL:
                case SQL_REAL:        
                case SQL_DOUBLE:
                    {
                        sql_col_value_t doublecol = result.get<double>(col_no);
                        row.emplace_back(column_name, doublecol);
                    }
                    break;
                case SQL_VARCHAR: 
                    {
                        sql_col_value_t strcol = result.get<nanodbc::string>(col_no);
                        row.emplace_back(column_name, strcol);
                    }
                    break;
                case SQL_TYPE_DATE:
                    {
                        sql_col_value_t datecol = result.get<nanodbc::date>(col_no);
                        row.emplace_back(column_name, datecol);
                    }
                    break;
                case SQL_TYPE_TIME:
                    {
                        sql_col_value_t timecol = result.get<nanodbc::time>(col_no);
                        row.emplace_back(column_name, timecol);
                    }
                    break;
                case SQL_TYPE_TIMESTAMP:
                    {
                        sql_col_value_t timestampcol = result.get<nanodbc::timestamp>(col_no);
                        row.emplace_back(column_name, timestampcol);
                    }
                    break;    
                default: 
                    {
                        sql_col_value_t binarycol = result.get<binary_t>(col_no);
                        row.emplace_back(column_name, binarycol);
                    }
                    break;
                }
            }
            rows.emplace_back(row);
        }
    }

    v8::Local<v8::Value> ExecuteNanodbcAsyncWorker::DoGetResult() {
        Nan::EscapableHandleScope scope;        
        SQLColumnVisitor visitor;
        
        v8::Local<v8::Array> result = Nan::New<v8::Array>(rows.size());
        int c = 0;
        for (const auto& row : rows) {
            v8::Local<v8::Object> result_row = Nan::New<v8::Object>();
            for (const auto& column : row) {
                const nanodbc::string& col_name = column.first;
                Nan::Set(result_row,
                    Nan::New<v8::String>(col_name).ToLocalChecked(),
                    column.second.apply_visitor(visitor)
                );
            }
            result->Set(c++, result_row);
        }
        return scope.Escape(result);
    }
}
