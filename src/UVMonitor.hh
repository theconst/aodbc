
/* 
 * File:   newClass.hh
 * Author: kko
 *
 * Created on August 26, 2018, 2:52 PM
 */

#ifndef NEWCLASS_HH
#define NEWCLASS_HH

#include <exception>

#include "uv.h"

namespace AODBC {
    
    
using namespace AODBC;

    
//TODO: implement more descent synchronization


class Synchronized;

template<typename T>
class UVMonitor {
public:
    
    
    template<typename ...Args>
    UVMonitor(Args&&... args) : object(std::forward<Args>(args)...) {
        mutex_handle = new uv_mutex_t;
        uv_mutex_init_recursive(mutex_handle);
    }
    
    UVMonitor(const UVMonitor& other) = delete;
    
    UVMonitor(UVMonitor&& other) = delete;
    
    T* get() {
        return &object;
    }
    
    virtual ~UVMonitor() {
        uv_mutex_destroy(mutex_handle);

        delete mutex_handle;
    }

private:
    friend class Synchronized;
    
    T object;
    uv_mutex_t* mutex_handle;
};


class Synchronized {
public:

    template<typename T> 
    Synchronized(UVMonitor<T>* monitor) :
        mutex_handle(monitor->mutex_handle) {
        uv_mutex_lock(mutex_handle);
    }

    Synchronized(const Synchronized&) = delete;

    Synchronized(Synchronized&&) = delete;

    virtual ~Synchronized() {
        uv_mutex_unlock(mutex_handle);
    }
private:
    uv_mutex_t* mutex_handle;
};

}

#endif /* NEWCLASS_HH */

