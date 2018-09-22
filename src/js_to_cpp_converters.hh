#ifndef JSTOCPPCONVERTERS_HH
#define JSTOCPPCONVERTERS_HH

#include <vector>
#include <cstdint>
#include <boost/optional.hpp>

#include "nan.h"

#include "arguments.hh"

#include "nctypes.hh"

namespace NC {

using NC::QueryArguments;


template<typename T>
boost::optional<T> convert_js_type_to_cpp(v8::Local<v8::Value>);

boost::optional<nc_variant_t> convert_js_date_to_cpp(v8::Local<v8::Value>);

template<>
boost::optional<nc_string_t> convert_js_type_to_cpp(v8::Local<v8::Value>);

template<>
boost::optional<nc_null_t> convert_js_type_to_cpp(v8::Local<v8::Value>);

template<>
boost::optional<nc_number_t> convert_js_type_to_cpp(v8::Local<v8::Value>);

template<>
boost::optional<std::int16_t> convert_js_type_to_cpp(v8::Local<v8::Value>);

template<>
boost::optional<nc_long_t> convert_js_type_to_cpp(v8::Local<v8::Value>);

template<>
boost::optional<QueryArguments> convert_js_type_to_cpp(v8::Local<v8::Value>);

template<>
boost::optional<nc_variant_t> convert_js_type_to_cpp(v8::Local<v8::Value>);

template<>
boost::optional<std::vector<nc_variant_t>> convert_js_type_to_cpp(
        v8::Local<v8::Value>);

template<>
boost::optional<PreparedStatementArguments> convert_js_type_to_cpp(
        v8::Local<v8::Value>);


}  // namespace NC


#endif  /* JSTOCPPCONVERTERS_HH */
