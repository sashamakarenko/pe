#include <pe/Measurement.h>
#include <iostream>
#include "TemplateParser.h"

extern int intValue;
extern double doubleValue;
extern const char * smallIntegers[];
extern const char * largeIntegers[];
extern const char * hugeIntegers[];
extern int spinUpCpu();

int main( int argc, char** argv )
{
    pe::Measurement m;
    m.pinToCpuCore( 3 );
    m.addEvent( pe::EventType::cpuCycles );
    m.addEvent( pe::EventType::hwInstructions );
    m.addEvent( pe::EventType::l1dCacheReadMisses );
    m.addEvent( pe::EventType::l1dCacheWriteMisses );
    m.addEvent( pe::EventType::l1iCacheReadMisses );
    m.addEvent( pe::EventType::l1iCacheWriteMisses );
    m.addEvent( pe::EventType::branchInstructions );
    m.addEvent( pe::EventType::llCacheReadMisses );
    m.addEvent( pe::EventType::branchMisses );
    // m.addEvent( pe::EventType::memory );
    const unsigned N = 16;
    m.initialize( 30 );
    
    std::cout << "\nsmall integers atoi:" << std::endl;
    for( int i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        intValue = atoi( smallIntegers[i%N] );
        m.stopCapture();
        //std::cout << intValue << std::endl;
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();


    std::cout << "\nlarge integers atoi:" << std::endl;
    intValue = spinUpCpu();
    for( int i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        intValue = atoi( largeIntegers[i%N] );
        m.stopCapture();
        //std::cout << intValue << std::endl;
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();

    unsigned len;

    std::cout << "\nsmall integers naiveParseUint:" << std::endl;
    intValue = spinUpCpu();
    for( int i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        intValue = naiveParseUInt( smallIntegers[i%N], len );
        m.stopCapture();
        //std::cout << intValue << std::endl;
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();

    std::cout << "\nsmall integers parseUint:" << std::endl;
    intValue = spinUpCpu();
    for( int i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        intValue = parseUInt( smallIntegers[i%N], len );
        m.stopCapture();
        //std::cout << intValue << std::endl;
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();


    std::cout << "\nhuge integers naiveParseUint:" << std::endl;
    intValue = spinUpCpu();
    for( int i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        intValue = naiveParseUInt<uint64_t>( hugeIntegers[i%N], len );
        m.stopCapture();
        //std::cout << intValue << std::endl;
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();

    std::cout << "\nhuge integers parseUint:" << std::endl;
    intValue = spinUpCpu();
    for( int i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        intValue = parseUInt<uint64_t>( hugeIntegers[i%N], len );
        m.stopCapture();
        //std::cout << intValue << std::endl;
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();



    std::cout << "\nlarge integers naiveParseUint:" << std::endl;
    intValue = spinUpCpu();
    for( int i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        intValue = naiveParseUInt( largeIntegers[i%N], len );
        m.stopCapture();
        //std::cout << intValue << std::endl;
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();

    std::cout << "\nlarge integers parseUint:" << std::endl;
    intValue = spinUpCpu();
    for( int i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        intValue = parseUInt( largeIntegers[i%N], len );
        m.stopCapture();
        //std::cout << intValue << std::endl;
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();
    
    return 0;
}

