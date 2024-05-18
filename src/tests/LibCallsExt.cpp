#include <sys/time.h>
#include <time.h>
#include <string_view>

#include "LibCalls.h"

timeval tv;
timespec ts;
int vint;
uint64_t vint64;


Base baseObj;
Base & base = baseObj;

Derived derivedObj;
Base & derived = derivedObj;

struct VDerivedComplex: virtual Base, VDerived
{
    virtual int getIntVirtual() const volatile override;
};

VDerivedComplex vderivedObj;
Base & vderived = vderivedObj;

const uint32_t  cOffsets[] = { 0, reinterpret_cast< const char *>( & base.i ) - reinterpret_cast< const char *>( & base ) };

const uint32_t * const Base::offsets = cOffsets;


void voidFunction()
{
}


void voidFunctionInt( int )
{
}

int intFunctionInt( int a )
{
    return a;
}

Base::Base()
: i{0}
, getIntFunctionPtr{ & Base::getInt }
{

}

Base::~Base(){}

int Base::getInt() const volatile { return i; };

int Base::getIntVirtual() const volatile { return i; };

int Derived::getIntVirtual() const volatile { return i; };

int VDerived::getIntVirtual() const volatile { return i; };

int VDerivedComplex::getIntVirtual() const volatile { return i; };

struct ExtensibleImpl: Extensible
{
    int k = 1;
    int i = 2;
};

Extensible * Extensible::newInstance()
{
    return new ExtensibleImpl();
}

Extensible::~Extensible(){}

int Extensible::getInt() const
{
    return static_cast<const ExtensibleImpl*>(this)->i;
}

std::string_view svin( "0123456789ABCD" );

char callStringViewByValue( std::string_view sv )
{
    return sv[0];
}

char callStringViewByRef( const std::string_view & sv )
{
    return sv[0];
}

char callStringViewByValue( std::string_view sv, int count )
{
    return count == 0 ? sv[count] : sv[count] + callStringViewByValue( sv, count-1 );
}

char callStringViewByRef( const std::string_view & sv, int count )
{
    return count == 0 ? sv[count] : sv[count] + callStringViewByRef( sv, count-1 );
}
