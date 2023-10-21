#ifndef LIBCALLS_H
#define LIBCALLS_H

#include <stdint.h>

struct Base
{
    Base();

    virtual ~Base();

    int getInt() const volatile;

    virtual int getIntVirtual() const volatile;

    inline int getIntInlined() const volatile { return i; }

    inline int getIntInlinedIndirect() const volatile { return *reinterpret_cast< const volatile int *>( reinterpret_cast< const volatile char *>( this ) + offsets[1] ); }

    int i;

    int (Base::*getIntFunctionPtr)() const volatile;

    static const uint32_t * const offsets;
};

struct Derived: Base
{
    virtual int getIntVirtual() const volatile override;
};

struct VDerived: virtual Base
{
    virtual int getIntVirtual() const volatile override;
};

#endif /* LIBCALLS_H */

