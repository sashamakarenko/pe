#include <pe/Measurement.h>
#include <iostream>
#include <sys/time.h>
#include <time.h>

#include "LibCalls.h"

extern timeval  tv;
extern timespec ts;

extern void voidFunction();
extern void voidFunctionInt( int );
extern int  intFunctionInt( int );
extern volatile int vint;
extern volatile uint64_t vint64;
extern Base & base;
extern Base & derived;
extern Base & vderived;

static __inline__ uint64_t rdtsc() {
    uint32_t lo, hi;
    __asm__ __volatile__( "lfence\n\trdtsc" : "=a" (lo), "=d" (hi) );
    return (uint64_t)hi << 32 | lo;
}


#define RUN_TEST(T) std::cout << "\n ------ " #T " -----" << std::endl;\
    for( int i = 0; i < m.getMaxCaptures(); ++i )\
    {\
        m.startCapture();\
        T;\
        T;\
        T;\
        T;\
        T;\
        T;\
        T;\
        T;\
        T;\
        T;\
        m.stopCapture();\
    }\
    m.prepareResults();\
    m.printCaptures();\
    m.printMarkdownCaptures( 50 );\
    m.rewind();


int main( int argc, char** argv )
{
    pe::Measurement m;
    m.pinToCpuCore( 3 );
    m.addEvent( pe::EventType::cpuCycles );
    m.addEvent( pe::EventType::hwInstructions );
    m.addEvent( pe::EventType::branchInstructions );
    m.addEvent( pe::EventType::llCacheReadMisses );
    m.addEvent( pe::EventType::branchMisses );
    m.addEvent( pe::EventType::busCycles );
    m.addEvent( pe::EventType::l1dCacheReadMisses );
    m.addEvent( pe::EventType::l1iCacheReadMisses );
    m.initialize( 10 );

    m.showNopeCost( std::cout );
    m.showSelfCost( std::cout );

    RUN_TEST( gettimeofday( &tv, nullptr ) )

    RUN_TEST( clock_gettime( CLOCK_MONOTONIC, &ts ) )

    RUN_TEST( vint64 = rdtsc() )

    RUN_TEST( voidFunction() )

    RUN_TEST( voidFunctionInt(i) )

    RUN_TEST( vint += intFunctionInt(i) )

    RUN_TEST( vint += base.getIntInlined() )

    RUN_TEST( vint += base.getInt() )

    RUN_TEST( vint += base.getIntVirtual() )

    RUN_TEST( vint += (base.*base.getIntFunctionPtr)() )

    RUN_TEST( vint += derived.getIntVirtual() )

    RUN_TEST( vint += vderived.getIntVirtual() )

    RUN_TEST( vint += base.getIntInlinedIndirect() )

    RUN_TEST( vint += derived.getIntInlinedIndirect() )


    return 0;
}

