#ifndef NCTYPES_HH
#define NCTYPES_HH

// include withoud std namespace
#include <stdint.h>
#include <vector>
#include "boost/variant.hpp"
#include "boost/blank.hpp"

#include "v8.h"
#include "nan.h"

#include "nanodbc/nanodbc.h"


//TODO(kko): remove this stub
#ifndef NANODBC_ENABLE_UNICODE
#include <type_traits>
   static_assert(std::false_type, "Unicode should be enabled")
#endif

namespace NC {

enum struct DateTypes {
    date, time, datetime
};

template <DateTypes t>
struct DateTag {
    const DateTypes value = t;
};

typedef std::vector<uint8_t> nc_binary_t;

typedef boost::blank nc_null_t;

typedef nanodbc::string nc_string_t;

typedef nanodbc::date nc_date_t;

typedef nanodbc::time nc_time_t;

typedef nanodbc::timestamp nc_timestamp_t;

typedef double nc_number_t;

typedef long nc_long_t;  // NOLINT(runtime/int)

typedef short nc_short_t;  // NOLINT(runtime/int)

typedef boost::variant<
    nc_null_t,
    nc_long_t,
    nc_number_t,
    nc_string_t,
    nc_date_t,
    nc_time_t,
    nc_timestamp_t,
    nc_binary_t
> nc_variant_t;

typedef std::pair<nc_string_t, nc_variant_t> nc_column_t;
typedef std::vector<nc_column_t> nc_row_t;
typedef std::vector<nc_row_t> nc_result_t;

typedef std::vector<nc_variant_t> nc_bindings_t;

}  // namespace NC

#endif /* NCTYPES_HH */

