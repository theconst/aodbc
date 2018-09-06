#ifndef UVMONITOR_HH
#define UVMONITOR_HH

#include <exception>

#include "uv.h"

namespace AODBC {

template <typename T>
class UVMonitor {
 private:
    struct Synchronized {
        explicit Synchronized(UVMonitor<T> *monitor)
                : mutex_handle(monitor->mutex_handle) {
            uv_mutex_lock(mutex_handle);
        }

        Synchronized(const Synchronized &) = delete;
        Synchronized(Synchronized &&) = delete;

        virtual ~Synchronized() {
            uv_mutex_unlock(mutex_handle);
        }
        uv_mutex_t *mutex_handle;
    };

    T object;
    uv_mutex_t *mutex_handle;

 public:
    template <typename... Args>
    explicit UVMonitor(Args &&... args) : object(std::forward<Args>(args)...) {
        mutex_handle = new uv_mutex_t;
        uv_mutex_init_recursive(mutex_handle);
    }

    UVMonitor(const UVMonitor &other) = delete;

    UVMonitor(UVMonitor &&other) = delete;

    template <typename F>
    auto operator()(F func) -> decltype(func(object)) {
        Synchronized lock{this};
        return func(object);
    }

    virtual ~UVMonitor() {
        uv_mutex_destroy(mutex_handle);

        delete mutex_handle;
    }
};



}  // namespace AODBC

#endif /* UVMONITOR_HH */
