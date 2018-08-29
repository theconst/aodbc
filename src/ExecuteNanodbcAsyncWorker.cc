
/* 
 * File:   ExecuteNanodbcAsyncWorker.cc
 * Author: kko
 * 
 * Created on August 28, 2018, 10:09 PM
 */

//unfotunally we need this heade for constants
#include <sql.h>

#include "ExecuteNanodbcAsyncWorker.hh"


//TODO: Make CPP -> JS template conversion for timestamps and etc
//this is getting ugly... very ugly

//very ugly

namespace AODBC {

    using namespace AODBC;

    ExecuteNanodbcAsyncWorker::ExecuteNanodbcAsyncWorker(
            std::shared_ptr< UVMonitor<nanodbc::connection> > connection_monitor,
            std::string&& query,
            Nan::Callback* callback) :
        NanodbcAsyncWorker(connection_monitor, callback),
        query(std::move(query)),
        row_numbers(),
        row_strings(),
        row_dates(),
        row_times(),
        row_timestamps(),
        row_nulls() {
        
    }

    void ExecuteNanodbcAsyncWorker::DoExecute(nanodbc::connection* connection) {
        nanodbc::result query_result = nanodbc::execute(*connection, query);
        
        for (int row = 0; query_result.next(); ++row) {
            short columns = query_result.columns();
            
            std::vector<nanodbc::string> nulls;
            std::vector<std::pair<nanodbc::string, double> > numbers;
            std::vector<std::pair<nanodbc::string, nanodbc::string> > strings;
            std::vector<std::pair<nanodbc::string, nanodbc::date> > dates;
            std::vector<std::pair<nanodbc::string, nanodbc::time> > times;
            std::vector<std::pair<nanodbc::string, nanodbc::timestamp> > timestamps;
            
            for (short col = 0; col < columns; ++col) {
                //there is no way we can fetch the datatype
                //this is a temporary workaround
                
                const nanodbc::string& column_name = query_result.column_name(col);

                if (query_result.is_null(column_name)) {
                    nulls.emplace_back(column_name);
                } else {
                    int datatype = query_result.column_datatype(col);

                    switch(datatype)  {
                    case SQL_INTEGER:
                    case SQL_SMALLINT:
                        numbers.emplace_back(column_name, query_result.get<int>(col));
                        break;
                    case SQL_FLOAT:
                        numbers.emplace_back(column_name, query_result.get<float>(col));
                        break;
                    case SQL_NUMERIC:
                    case SQL_DECIMAL:
                    case SQL_REAL:        
                    case SQL_DOUBLE:
                        numbers.emplace_back(column_name, query_result.get<double>(col));
                        break;
                    case SQL_VARCHAR: 
                        strings.emplace_back(column_name, query_result.get<nanodbc::string>(col));
                        break;
                    case SQL_TYPE_DATE:
                        dates.emplace_back(column_name, query_result.get<nanodbc::date>(col));
                        break;
                    case SQL_TYPE_TIME:
                        times.emplace_back(column_name, query_result.get<nanodbc::time>(col));
                        break;
                    case SQL_TYPE_TIMESTAMP:
                        timestamps.emplace_back(column_name, query_result.get<nanodbc::timestamp>(col));
                        break;
                    default:
                        throw std::runtime_error("Fatal Unsupported SQL type");
                        break;
                    }
                }
            }
            row_numbers.emplace_back(std::move(numbers));
            row_strings.emplace_back(std::move(strings));
            row_dates.emplace_back(std::move(dates));
            row_times.emplace_back(std::move(times));
            row_timestamps.emplace_back(std::move(timestamps));
            row_nulls.emplace_back(std::move(nulls));
        }
    }

    v8::Local<v8::Value> ExecuteNanodbcAsyncWorker::DoGetResult() {
        Nan::EscapableHandleScope scope;
        
        //TODO: refactor to functions at least
        
        //should have the same size
        int size = row_numbers.size();
        
        v8::Local<v8::Array> result = Nan::New<v8::Array>(size);
        
        for (int i = 0; i < size; i++) {
            v8::Local<v8::Object> row = Nan::New<v8::Object>();

            for (auto&& element : row_numbers[i]) {
                Nan::Set(row,
                    Nan::New<v8::String>(element.first).ToLocalChecked(),
                    Nan::New<v8::Number>(element.second)
                );
            }

            for (auto&& element : row_strings[i]) {
                Nan::Set(row, 
                    Nan::New<v8::String>(element.first).ToLocalChecked(), 
                    Nan::New<v8::String>(element.second).ToLocalChecked()
                );
            }

            for (auto&& element : row_times[i]) {
                v8::Local<v8::Object> time_result = Nan::New<v8::Object>();

                auto const& time = element.second; 

                Nan::Set(time_result, 
                    Nan::New<v8::String>("hour").ToLocalChecked(),
                    Nan::New<v8::Number>(time.hour)
                );

                Nan::Set(time_result,
                    Nan::New<v8::String>("minute").ToLocalChecked(),
                    Nan::New<v8::Number>(time.min));

                Nan::Set(time_result,
                    Nan::New<v8::String>("second").ToLocalChecked(),
                    Nan::New<v8::Number>(time.sec)
                );

                Nan::Set(row, 
                    Nan::New<v8::String>(element.first).ToLocalChecked(),
                    time_result
                );
            }

            for (auto&& element : row_dates[i]) {
                v8::Local<v8::Object> date_result = Nan::New<v8::Object>();

                auto&& date = element.second;

                Nan::Set(date_result,
                    Nan::New<v8::String>("day").ToLocalChecked(),
                    Nan::New<v8::Number>(date.day)
                );

                Nan::Set(date_result,
                    Nan::New<v8::String>("month").ToLocalChecked(),
                    Nan::New<v8::Number>(date.month)
                );

                Nan::Set(date_result,
                    Nan::New<v8::String>("year").ToLocalChecked(),
                    Nan::New<v8::Number>(date.year)
                );

                Nan::Set(row,
                    Nan::New<v8::String>(element.first).ToLocalChecked(),
                    date_result
                );
            }

            for (auto&& element : row_timestamps[i]) {
                v8::Local<v8::Object> timestamp_result = Nan::New<v8::Object>();

                auto&& timestamp = element.second;

                Nan::Set(timestamp_result,
                    Nan::New<v8::String>("day").ToLocalChecked(),
                    Nan::New<v8::Number>(timestamp.day)
                );

                Nan::Set(timestamp_result,
                    Nan::New<v8::String>("month").ToLocalChecked(),
                    Nan::New<v8::Number>(timestamp.month)
                );

                Nan::Set(timestamp_result,
                    Nan::New<v8::String>("year").ToLocalChecked(),
                    Nan::New<v8::Number>(timestamp.year)
                );

                Nan::Set(timestamp_result,
                    Nan::New<v8::String>("hour").ToLocalChecked(),
                    Nan::New<v8::Number>(timestamp.hour)
                );

                Nan::Set(timestamp_result,
                    Nan::New<v8::String>("minute").ToLocalChecked(),
                    Nan::New<v8::Number>(timestamp.min)
                );

                Nan::Set(timestamp_result,
                    Nan::New<v8::String>("second").ToLocalChecked(),
                    Nan::New<v8::Number>(timestamp.sec)
                );

                Nan::Set(row,
                    Nan::New<v8::String>(element.first).ToLocalChecked(),
                    timestamp_result
                );
            }
            
            for (auto&& element : row_nulls[i]) {
                Nan::Set(row,
                    Nan::New<v8::String>(element).ToLocalChecked(),
                    Nan::Null()
                );
            }
            
            result->Set(i, row);
        }
        
        return scope.Escape(result);
    }
    
}
