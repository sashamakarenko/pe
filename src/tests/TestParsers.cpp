#include <pe/Measurement.h>
#include <iostream>
#include "TemplateParser.h"

extern int intValue;
extern double doubleValue;
extern const char * smallIntegers[];
extern const char * largeIntegers[];

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
    
    std::cout << "\nsmall integers atoi:" << std::endl;
    for( int i = 0; i < m.getMaxCaptures() && smallIntegers[i]; ++i )
    {
        m.startCapture();
        intValue = atoi( smallIntegers[i] );
        m.stopCapture();
        //std::cout << intValue << std::endl;
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();


    std::cout << "\nlarge integers atoi:" << std::endl;
    for( int i = 0; i < m.getMaxCaptures() && largeIntegers[i]; ++i )
    {
        m.startCapture();
        intValue = atoi( largeIntegers[i] );
        m.stopCapture();
        //std::cout << intValue << std::endl;
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();
    
    unsigned len;
    std::cout << "\nsmall integers parseUint:" << std::endl;
    for( int i = 0; i < m.getMaxCaptures() && smallIntegers[i]; ++i )
    {
        m.startCapture();
        intValue = parseUInt( smallIntegers[i], len );
        m.stopCapture();
        //std::cout << intValue << std::endl;
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();


    std::cout << "\nlarge integers parseUint:" << std::endl;
    for( int i = 0; i < m.getMaxCaptures() && largeIntegers[i]; ++i )
    {
        m.startCapture();
        intValue = parseUInt( largeIntegers[i], len );
        m.stopCapture();
        //std::cout << intValue << std::endl;
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();

    std::cout << "\nsmall integers naiveParseUint:" << std::endl;
    for( int i = 0; i < m.getMaxCaptures() && smallIntegers[i]; ++i )
    {
        m.startCapture();
        intValue = naiveParseUInt( smallIntegers[i], len );
        m.stopCapture();
        //std::cout << intValue << std::endl;
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();


    std::cout << "\nlarge integers naiveParseUint:" << std::endl;
    for( int i = 0; i < m.getMaxCaptures() && largeIntegers[i]; ++i )
    {
        m.startCapture();
        intValue = naiveParseUInt( largeIntegers[i], len );
        m.stopCapture();
        //std::cout << intValue << std::endl;
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();
    
    return 0;
}

