#ifndef ERRORS_HH
#define ERRORS_HH

#include <stdexcept>

namespace NC {

struct TypeError : std::runtime_error {
    using std::runtime_error::runtime_error;
};

struct RangeError : std::runtime_error {
    using std::runtime_error::runtime_error;
};

struct Error : std::runtime_error {
    using std::runtime_error::runtime_error;
};


inline void handle_error()
try {
    throw;
} catch(const TypeError& te) {
    return Nan::ThrowTypeError(te.what());
} catch(const RangeError& re) {
    return Nan::ThrowRangeError(re.what());
} catch (const std::exception& e) {
    return Nan::ThrowError(e.what());
}

}  // namespace NC

#endif  /* ERRORS_HH */
