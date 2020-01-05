#include <pe/Measurement.h>
#include <iostream>
#include <sys/time.h>
#include <time.h>

#include "LibCalls.h"

extern timeval  tv;
extern timespec ts;

extern void voidFunction();
extern void voidFunction1( int );
extern int  intFunction1( int );
extern volatile int vint;
extern volatile uint64_t vint64;
extern volatile C c;
extern volatile B b;

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
        m.stopCapture();\
    }\
    m.prepareResults();\
    m.printCaptures();\
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
    m.initialize( 8 );
    
    m.showNopeCost( std::cout );
    m.showSelfCost( std::cout );
    

    RUN_TEST( gettimeofday( &tv, nullptr ) )

    RUN_TEST( clock_gettime( CLOCK_MONOTONIC, &ts ) )

    RUN_TEST( voidFunction() )
    
    RUN_TEST( voidFunction1(i) )
    
    RUN_TEST( vint = intFunction1(i) )

    RUN_TEST( vint = c.getIi() )

    RUN_TEST( vint = c.getI() )

    RUN_TEST( vint = c.getIv() )

    RUN_TEST( vint = b.getIv() )

    RUN_TEST( vint = b.getIx() )

    RUN_TEST( vint64 = rdtsc() )
 
    return 0;
}

