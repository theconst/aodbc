
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

#include <iostream>

namespace AODBC {

    
//TODO: implement more descent synchronization

template<typename T>
class UVMonitor {
public:
    class Synchronized {
    public:
            
        Synchronized(UVMonitor* monitor):
            mutex_handle(monitor->mutex_handle)  {
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
    
    
    template<typename ...Args>
    UVMonitor(Args&&... args) : object(std::forward<Args>(args)...) {
        mutex_handle = new uv_mutex_t;
        uv_mutex_init(mutex_handle);
    }
    
    UVMonitor(const UVMonitor& other) = delete;
    
    UVMonitor(UVMonitor&& other) = delete;
    
    T* operator->() {
        return &object;
    }
    
    virtual ~UVMonitor() {
        std::cout << "UVMonitor destroy enter";
        
        uv_mutex_destroy(mutex_handle);

        delete mutex_handle;
        
        std::cout << "UVMonitor destroy exit";
    }

private:
    T object;
    uv_mutex_t* mutex_handle;
};

}

#endif /* NEWCLASS_HH */

