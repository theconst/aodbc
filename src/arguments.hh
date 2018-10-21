#ifndef ARGUMENTS_HH
#define ARGUMENTS_HH

#include "nctypes.hh"

#include <stdexcept>
#include <tuple>

#include "boost/optional.hpp"

#include "errors.hh"

namespace NC {

using NC::RangeError;

class TimeoutArg final {
    static constexpr const nc_long_t default_timeout = 0L;

    nc_long_t timeout = default_timeout;

 public:
    static const TimeoutArg& DefaultValue() {
        static TimeoutArg default_value { boost::none };
        return default_value;
    }

    explicit TimeoutArg(const boost::optional<nc_long_t>& t) {
        if (t) {
            nc_long_t val = *t;
            if (val < default_timeout) {
                throw RangeError("Illegal timeout value. Should be > 0");
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
    static const BatchSizeArg& DefaultValue() {
        static BatchSizeArg default_value { boost::none };
        return default_value;
    }

    explicit BatchSizeArg(const boost::optional<nc_long_t>& bs) {
        if (bs) {
            nc_long_t val = *bs;
            if (val < default_batch_size) {
                throw RangeError("Illegal batch size value. Should be > 1");
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
    template<typename T>
    explicit QueryStringArg(T&& q) : query(std::forward<T>(q)) {
    }

    operator const nc_string_t&() const noexcept {
        return query;
    }

    QueryStringArg(const QueryStringArg&) = default;
    QueryStringArg(QueryStringArg&&) = default;
    ~QueryStringArg() = default;
};

class BindingsArg final {
    nc_bindings_t bindings;

 public:
    static const BindingsArg& DefaultValue() {
        static BindingsArg default_value { nc_bindings_t(0) };
        return default_value;
    }

    template <typename T>
    explicit BindingsArg(T&& bindings) :
        bindings(std::forward<nc_bindings_t>(bindings)) {
    }

    operator const nc_bindings_t&() const noexcept {
        return bindings;
    }

    BindingsArg(const BindingsArg&) = default;
    BindingsArg(BindingsArg&&) = default;
    ~BindingsArg() = default;
};

class PreparedStatementArguments {
    BindingsArg bindings;
    BatchSizeArg batch_size;
    TimeoutArg timeout;

 public:
    PreparedStatementArguments(
            BindingsArg ba,
            BatchSizeArg bs,
            TimeoutArg t)
        : bindings(std::move(ba)),
          batch_size(std::move(bs)),
          timeout(std::move(t))  {
    }

    const BindingsArg& GetBindings() const noexcept {
        return bindings;
    }

    const BatchSizeArg& GetBatchSize() const noexcept {
        return batch_size;
    }

    const TimeoutArg& GetTimeout() const noexcept {
        return timeout;
    }

    PreparedStatementArguments(const PreparedStatementArguments&) = default;
    PreparedStatementArguments(PreparedStatementArguments&&) = default;
    ~PreparedStatementArguments() = default;
};


class QueryArguments {
    QueryStringArg query_string;
    BatchSizeArg batch_size;
    TimeoutArg timeout;

 public:
    QueryArguments(
            QueryStringArg qs,
            BatchSizeArg bs,
            TimeoutArg t)
        : query_string(std::move(qs)),
          batch_size(std::move(bs)),
          timeout(std::move(t))  {
    }

    const QueryStringArg& GetQuery() const noexcept {
        return query_string;
    }

    const BatchSizeArg& GetBatchSize() const noexcept {
        return batch_size;
    }

    const TimeoutArg& GetTimeout() const noexcept {
        return timeout;
    }

    QueryArguments(const QueryArguments&) = default;
    QueryArguments(QueryArguments&&) = default;
    ~QueryArguments() = default;
};


}  // namespace NC


#endif /* ARGUMETNS_HH */
