#ifndef JSKEYS_HH
#define JSKEYS_HH

namespace NC {

namespace DateKeys {

    constexpr const char* year = "year";
    constexpr const char* month = "month";
    constexpr const char* day = "day";
    constexpr const char* hours = "hours";
    constexpr const char* minutes = "minutes";
    constexpr const char* seconds = "seconds";
    constexpr const char* fractionalSeconds = "fractionalSeconds";

}  // namespace DateKeys

namespace ArgumentKeys {

    constexpr const char* bindings = "bindings";
    constexpr const char* query = "query";
    constexpr const char* batch_size = "batchSize";
    constexpr const char* timeout = "timeout";

}  // namespace ArgumentKeys

}  // namespace NC

#endif  /* JSKEYS_HH */
