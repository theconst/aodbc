#ifndef ARGUMENTS_HH
#define ARGUMENTS_HH

#include "nctypes.hh"

#include <stdexcept>

#include "boost/optional.hpp"


namespace NC {

class QueryArguments {

    static constexpr nc_long_t default_batch_size = 1L;
    static constexpr nc_long_t default_timeout = 0L;

    nc_string_t query;
    nc_long_t batch_size;
    nc_long_t timeout;

 public:
    explicit QueryArguments(nc_string_t query) :
        query(std::move(query)),
        batch_size(default_batch_size),
        timeout(default_timeout) {
    }

    const nc_string_t& GetQuery() const noexcept {
        return query;
    }

    nc_long_t GetBatchSize() const noexcept {
        return batch_size;
    }

    nc_long_t GetTimeout() const noexcept {
        return timeout;
    }

    void SetBatchSize(const boost::optional<nc_long_t>& bs) {
        if (bs) {
            nc_long_t val = *bs;
            if (val < default_batch_size) {
                throw std::invalid_argument("Illegal batch size value");
            }
            batch_size = val;
        }
    }

    void SetTimeout(const boost::optional<nc_long_t>& t) {
        assert(t > default_batch_size);
        if (t) {
            nc_long_t val = *t;
            if (val < default_timeout) {
                throw std::invalid_argument("Illegal timeout value");
            }
            batch_size = val;
        }
    }

    QueryArguments(const QueryArguments&) = default;
    QueryArguments(QueryArguments&&) = default;
    virtual ~QueryArguments() = default;
};

class PreparedStatementArguments {

    // TODO(kko): remove this copy-paste
    static constexpr nc_long_t default_batch_size = 1L;
    static constexpr nc_long_t default_timeout = 0L;

    std::vector<nc_variant_t> bindings;
    nc_long_t batch_size;
    nc_long_t timeout;

 public:
    template <typename T>
    explicit PreparedStatementArguments(T&& bindings) :
        bindings(std::forward<std::vector<nc_variant_t>>(bindings)),
        batch_size(default_batch_size),
        timeout(default_timeout) {
    }

    const std::vector<nc_variant_t>& GetBindings() const noexcept {
        return bindings;
    }

    nc_long_t GetBatchSize() const noexcept {
        return batch_size;
    }

    nc_long_t GetTimeout() const noexcept {
        return timeout;
    }

    // TODO(kko) - compose with some timeout args
    void SetBatchSize(const boost::optional<nc_long_t>& bs) {
        if (bs) {
            nc_long_t val = *bs;
            if (val < default_batch_size) {
                throw std::invalid_argument("Illegal batch size value");
            }
            batch_size = val;
        }
    }

    void SetTimeout(const boost::optional<nc_long_t>& t) {
        assert(t > default_batch_size);
        if (t) {
            nc_long_t val = *t;
            if (val < default_timeout) {
                throw std::invalid_argument("Illegal timeout value");
            }
            batch_size = val;
        }
    }

    PreparedStatementArguments(const PreparedStatementArguments&) = default;
    PreparedStatementArguments(PreparedStatementArguments&&) = default;
    virtual ~PreparedStatementArguments() = default;
};

}  // namespace NC


#endif /* ARGUMETNS_HH */
