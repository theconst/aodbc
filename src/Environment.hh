#ifndef ENVIRONMENT_HH
#define ENVIRONMENT_HH

#include "UVMonitor.hh"

namespace NC {

// Dummy object that represents environment
// Used for locking operations that use Env handle
struct Environment {

    static UVMonitor<Environment>& Get() {
        static UVMonitor<Environment> env_monitor;

        return env_monitor;
    }

};


}  // namespace NC

#endif  /* ENVIRONMENT_HH */