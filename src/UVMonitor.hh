#ifndef UVMONITOR_HH
#define UVMONITOR_HH

#include <exception>

#include "uv.h"

namespace AODBC {

// TODO(kko): implement wrapper pattern as per http://www.stroustrup.com/wrapper.pdf

class Synchronized;

template <typename T>
class UVMonitor {
 public:
    template <typename... Args>
    explicit UVMonitor(Args &&... args) : object(std::forward<Args>(args)...) {
        mutex_handle = new uv_mutex_t;
        uv_mutex_init_recursive(mutex_handle);
    }

    UVMonitor(const UVMonitor &other) = delete;

    UVMonitor(UVMonitor &&other) = delete;

    T *get() {
        return &object;
    }

    virtual ~UVMonitor() {
        uv_mutex_destroy(mutex_handle);

        delete mutex_handle;
    }

 private:
    friend class Synchronized;

    T object;
    uv_mutex_t *mutex_handle;
};

class Synchronized {
 public:
    template <typename T>
    explicit Synchronized(UVMonitor<T> *monitor)
            : mutex_handle(monitor->mutex_handle) {
        uv_mutex_lock(mutex_handle);
    }

    Synchronized(const Synchronized &) = delete;
    Synchronized(Synchronized &&) = delete;

    virtual ~Synchronized() {
        uv_mutex_unlock(mutex_handle);
    }

 private:
    uv_mutex_t *mutex_handle;
};

}  // namespace AODBC

#endif /* UVMONITOR_HH */
