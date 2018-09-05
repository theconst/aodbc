#ifndef CPPTOJSCONVERTERS_HH
#define CPPTOJSCONVERTERS_HH

#include "nan.h"

#include "sqltypes.hh"

namespace AODBC {

template <typename T>
v8::Local<v8::Value> convert_cpp_type_to_js(const T& arg);

}  // namespace AODBC

#endif /* CPPTOJSCONVERTERS_HH */

