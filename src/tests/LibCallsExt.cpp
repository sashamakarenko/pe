#include <sys/time.h>
#include <time.h>

#include "LibCalls.h"

timeval tv;
timespec ts;
int vint;
uint64_t vint64;
C c;
B b;


const uint32_t  cOffsets[] = { 0, reinterpret_cast< const char *>( & c.i ) - reinterpret_cast< const char *>( & c ) };

const uint32_t * const C::offsets = cOffsets;


void voidFunction()
{
}


void voidFunction1( int )
{
}

int  intFunction1( int a )
{
    return a;
}

C::~C(){}
    
int C::getI() const volatile { return i; };

int C::getIv() const volatile { return i; };

int B::getIv() const volatile { return i; };

