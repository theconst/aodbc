
/* 
 * File:   VersionWorker.hh
 * Author: kko
 *
 * Created on August 25, 2018, 1:52 PM
 */

#ifndef VERSIONWORKER_HH
#define VERSIONWORKER_HH

#include "nan.h"

namespace AODBC {

//demo worker 
class VersionWorker final : public Nan::AsyncWorker {
    
    public:
        VersionWorker(Nan::Callback*);
        
        virtual ~VersionWorker();
        
        virtual void HandleOKCallback();
        
        virtual void Execute();
        
    private:
        static const std::string version;
            
};

}
#endif /* VERSIONWORKER_HH */

