
/* 
 * File:   ExecuteNanodbcAsyncWorker.hh
 * Author: kko
 *
 * Created on August 28, 2018, 10:09 PM
 */

#ifndef EXECUTENANODBCASYNCWORKER_HH
#define EXECUTENANODBCASYNCWORKER_HH

#include <memory>
#include <vector>

#include "NanobcAsyncWorker.hh"

#include "nan.h"

namespace AODBC {

    using namespace AODBC;

    class ExecuteNanodbcAsyncWorker final : public NanodbcAsyncWorker {
    public:
        explicit ExecuteNanodbcAsyncWorker(
            std::shared_ptr< UVMonitor<nanodbc::connection> > connection_monitor,
            std::string&& query,
            Nan::Callback* callback
        );

        ExecuteNanodbcAsyncWorker(const ExecuteNanodbcAsyncWorker& orig) = delete;

        ExecuteNanodbcAsyncWorker(ExecuteNanodbcAsyncWorker&& orig) = delete;

        virtual ~ExecuteNanodbcAsyncWorker() = default;

    protected:

        virtual v8::Local<v8::Value> DoGetResult() override;

        virtual void DoExecute(nanodbc::connection* connection) override;
    private:
        std::string query;
        
        //TODO: replace with variant datatype ard visitor pattern
        
        std::vector<std::vector<std::pair<nanodbc::string, double> > > row_numbers;
        
        std::vector<std::vector<std::pair<nanodbc::string, nanodbc::string> > > row_strings;
        
        std::vector<std::vector<std::pair<nanodbc::string, nanodbc::date> > > row_dates;
        
        std::vector<std::vector<std::pair<nanodbc::string, nanodbc::time> > > row_times;
        
        std::vector<std::vector<std::pair<nanodbc::string, nanodbc::timestamp> > > row_timestamps;
        
        std::vector<std::vector<nanodbc::string> > row_nulls;
    };

}

#endif /* EXECUTENANODBCASYNCWORKER_HH */

