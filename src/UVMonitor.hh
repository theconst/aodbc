#ifndef UVMONITOR_HH
#define UVMONITOR_HH

#include <exception>

#include "uv.h"

namespace NC {

template <typename T>
class UVMonitor {
 private:
    struct UVMutexLock {
        explicit UVMutexLock(uv_mutex_t* mutex_handle)
                : mutex_handle(mutex_handle) {
            uv_mutex_lock(mutex_handle);
        }

        UVMutexLock(const UVMutexLock &) = delete;
        UVMutexLock(UVMutexLock &&) = delete;

        virtual ~UVMutexLock() {
            uv_mutex_unlock(mutex_handle);
        }
        uv_mutex_t* mutex_handle;
    };

    T object;
    uv_mutex_t* mutex_handle;

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
        return synchronized(func);
    }

    template<typename F>
    auto synchronized(F func) -> decltype(func(object)) {
        UVMutexLock lock{mutex_handle};
        return func(object);
    }

    virtual ~UVMonitor() {
        uv_mutex_destroy(mutex_handle);

        delete mutex_handle;
    }
};



}  // namespace NC

#endif /* UVMONITOR_HH */
