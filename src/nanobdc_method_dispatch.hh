#ifndef NANODBCMETHODDISPATCH_HH
#define NANODBCMETHODDISPATCH_HH

#include "nanodbc.h"

#include "UVMonitor.hh"

#include "method_dispatch.hh"


namespace AODBC {

enum struct CommandNames {
    dbms_name
};

template<CommandNames tag>
struct MethodTag {
};


}  // namespace AODBC


#endif  // NANODBCMETHODDISPATCH_HH
