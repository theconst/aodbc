#ifndef ARGUMENTS_HH
#define ARGUMENTS_HH

#include "nctypes.hh"

#include <stdexcept>

#include "boost/optional.hpp"


namespace NC {


class TimeoutArg final {
    static constexpr nc_long_t default_timeout = 0L;

    nc_long_t timeout = default_timeout;
 public:
    explicit TimeoutArg(boost::optional<nc_long_t> t) {
         if (t) {
            nc_long_t val = *t;
            if (val < default_timeout) {
                throw std::invalid_argument("Illegal timeout value");
            }
            timeout = val;
        }
    }

    operator nc_long_t() const noexcept {
        return timeout;
    }

    TimeoutArg(const TimeoutArg&) = default;
    TimeoutArg(TimeoutArg&&) = default;
    ~TimeoutArg() = default;
};

class BatchSizeArg final {
    static constexpr nc_long_t default_batch_size = 1L;

    nc_long_t batch_size = default_batch_size;
 public:
    explicit BatchSizeArg(const boost::optional<nc_long_t>& bs) {
        if (bs) {
            nc_long_t val = *bs;
            if (val < default_batch_size) {
                throw std::invalid_argument("Illegal batch size value");
            }
            batch_size = val;
        }
    }

    operator nc_long_t() const noexcept {
        return batch_size;
    }

    BatchSizeArg(const BatchSizeArg&) = default;
    BatchSizeArg(BatchSizeArg&&) = default;
    ~BatchSizeArg() = default;
};

class QueryStringArg final {
    nc_string_t query;

 public:
    explicit QueryStringArg(nc_string_t q)
        : query(std::move(q)) {
    }

    operator const nc_string_t&() const noexcept {
        return query;
    }

    QueryStringArg(const QueryStringArg&) = default;
    QueryStringArg(QueryStringArg&&) = default;
    ~QueryStringArg() = default;
};

class BindingsArg final {
    std::vector<nc_variant_t> bindings;

 public:
    template <typename T>
    explicit BindingsArg(T&& bindings) :
        bindings(std::forward<std::vector<nc_variant_t>>(bindings)) {
    }

    operator const std::vector<nc_variant_t>&() const noexcept {
        return bindings;
    }

    BindingsArg(const BindingsArg&) = default;
    BindingsArg(BindingsArg&&) = default;
    ~BindingsArg() = default;
};

using PreparedStatementArguments =
    std::tuple<BindingsArg, BatchSizeArg, TimeoutArg>;
using QueryArguments =
    std::tuple<QueryStringArg, BatchSizeArg, TimeoutArg>;

}  // namespace NC


#endif /* ARGUMETNS_HH */
