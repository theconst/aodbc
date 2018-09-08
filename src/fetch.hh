#ifndef FETCH_HH
#define FETCH_HH

#include "sqltypes.hh"

namespace AODBC {
    nc_result_t fetch_result_eagerly(nanodbc::result* result);
}


#endif /* FETCH_HH */

