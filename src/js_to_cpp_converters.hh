#ifndef JSTOCPPCONVERTERS_HH
#define JSTOCPPCONVERTERS_HH

#include <vector>

#include <boost/optional.hpp>

#include "nan.h"

#include "arguments.hh"

#include "nctypes.hh"

namespace NC {

using NC::QueryArguments;


template<typename T>
boost::optional<T> convert_js_type_to_cpp(v8::Local<v8::Value> value);

template<>
boost::optional<nc_string_t>
        convert_js_type_to_cpp<nc_string_t>(v8::Local<v8::Value> local);

template<>
boost::optional<nc_null_t>
        convert_js_type_to_cpp<nc_null_t>(v8::Local<v8::Value> local);

template<>
boost::optional<nc_number_t>
        convert_js_type_to_cpp<nc_number_t>(v8::Local<v8::Value> local);

template<>
boost::optional<nc_long_t>
        convert_js_type_to_cpp<nc_long_t>(v8::Local<v8::Value> local);

template<>
boost::optional<QueryArguments> convert_js_type_to_cpp<QueryArguments>(
        v8::Local<v8::Value> local);

template<>
boost::optional<nc_variant_t> convert_js_type_to_cpp<nc_variant_t>(
        v8::Local<v8::Value> local);

template<>
boost::optional<std::vector<nc_variant_t>> convert_js_type_to_cpp<
                std::vector<nc_variant_t>>(v8::Local<v8::Value> local);


}  // namespace NC


#endif  /* JSTOCPPCONVERTERS_HH */
