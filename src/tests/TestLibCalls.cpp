#include <pe/Measurement.h>
#include <iostream>
#include <sys/time.h>
#include <time.h>

extern timeval  tv;
extern timespec ts;

int main( int argc, char** argv )
{
    pe::Measurement m;
    m.pinToCpuCore( 3 );
    m.addEvent( pe::EventType::cpuCycles );
    m.addEvent( pe::EventType::hwInstructions );
    m.addEvent( pe::EventType::branchInstructions );
    m.addEvent( pe::EventType::llCacheReadMisses );
    m.addEvent( pe::EventType::branchMisses );
    m.initialize( 20 );
    
    m.showNopeCost( std::cout );
    m.showSelfCost( std::cout );
    
    std::cout << "\ngettimeofday:" << std::endl;
    for( int i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        gettimeofday( &tv, nullptr );
        m.stopCapture();
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();

    std::cout << "\nclock_gettime:" << std::endl;
    for( int i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        clock_gettime( CLOCK_MONOTONIC, &ts );
        m.stopCapture();
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();
    
    return 0;
}

