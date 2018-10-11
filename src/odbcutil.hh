#ifndef ODBCUTIL_HH
#define ODBCUTIL_HH

#include "sql.h"
#include "sqlext.h"

namespace NC {

inline bool success(SQLRETURN rc) {
    return rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO;
}

}  // namespace NC


#endif /* ODBCUTIL_HH */
