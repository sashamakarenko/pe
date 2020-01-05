#ifndef LIBCALLS_H
#define LIBCALLS_H

#include <stdint.h>

struct C
{
    virtual ~C();
    
    int getI() const volatile;

    virtual int getIv() const volatile;
    
    inline int getIi() const volatile { return i; } 
    
    inline int getIx() const volatile { return *reinterpret_cast< const volatile int *>( reinterpret_cast< const volatile char *>( this ) + offsets[1] ); }
    
    int i;
    
    static const uint32_t * const offsets;
};

struct B: virtual C
{
    virtual int getIv() const volatile;
};

#endif /* LIBCALLS_H */

