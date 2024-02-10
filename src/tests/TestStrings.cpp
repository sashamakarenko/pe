#include <pe/Measurement.h>
#include <iostream>
#include "Helper.h"

extern std::string result;
extern unsigned smallIntegers[];
extern unsigned largeIntegers[];
extern uint64_t hugeIntegers[];

int main( int argc, char** argv )
{
    pe::Measurement m;
    m.pinToCpuCore( 3 );
    m.addEvent( pe::EventType::cpuCycles );
    m.addEvent( pe::EventType::hwInstructions );
    m.addEvent( pe::EventType::branchInstructions );
    m.addEvent( pe::EventType::memory );
    const unsigned N = 16;
    m.initialize( N*10 );

    std::cout << "\nsmall integers std::to_string:" << std::endl;
    for( int i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        result = std::to_string( smallIntegers[i%N] );
        m.stopCapture();
    }
    m.prepareResults();
    // m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();

    std::cout << "\nlarge integers std::to_string:" << std::endl;
    for( int i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        result = std::to_string( largeIntegers[i%N] );
        m.stopCapture();
    }
    m.prepareResults();
    // m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();

    std::cout << "\nhuge integers std::to_string:" << std::endl;
    for( int i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        result = std::to_string( hugeIntegers[i%N] );
        m.stopCapture();
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();

    std::cout << "\nnew delete 100:" << std::endl;
    for( int i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        char * ptr = new char[100];
        delete ptr;
        m.stopCapture();
    }
    m.prepareResults();
    // m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();

    return 0;
}

