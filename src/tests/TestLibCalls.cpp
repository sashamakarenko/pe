#include <pe/Measurement.h>
#include <iostream>
#include <sys/time.h>
#include <time.h>
#include <string_view>

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

extern std::string_view svin;
extern char callStringViewByValue( std::string_view sv );
extern char callStringViewByRef( const std::string_view & sv );
extern char callStringViewByValue( std::string_view sv, int count );
extern char callStringViewByRef( const std::string_view & sv, int count );

static __inline__ uint64_t rdtsc() {
    uint32_t lo, hi;
    __asm__ __volatile__( "lfence\n\trdtsc" : "=a" (lo), "=d" (hi) );
    return (uint64_t)hi << 32 | lo;
}


#define RUN_TEST(T) std::cout << "\n ------ " #T " -----" << std::endl;\
    for( unsigned i = 0; i < m.getMaxCaptures(); ++i )\
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
    m.printMarkdownCaptures( 50 );\
    m.rewind();

    //m.printCaptures();

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

    Extensible * ex = Extensible::newInstance();

    RUN_TEST( vint += ex->getInt() )

    RUN_TEST( vint += callStringViewByValue( svin ) )

    RUN_TEST( vint += callStringViewByRef( svin ) )

    RUN_TEST( vint += callStringViewByValue( svin, 10 ) )

    RUN_TEST( vint += callStringViewByRef( svin, 10 ) )

    return 0;
}

