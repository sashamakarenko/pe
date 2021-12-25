#include <pe/Measurement.h>
#include <iostream>
#include "TemplateParser.h"
#include "Helper.h"

extern int intValue;
extern double doubleValue;
extern const char * smallIntegers[];
extern const char * largeIntegers[];
extern const char * hugeIntegers[];
extern int spinUpCpu();

int main( int argc, char** argv )
{
    unsigned l = 0;
    parseUInt8<long>( "0123456789" "1234567", l );

    for( const char **ptr = smallIntegers; *ptr; ++ptr )
    {
        unsigned v0 = (unsigned)atoi( *ptr );
        unsigned len = 0;
        unsigned v2 = parseUInt2( *ptr, len );
        CHECK( parseUInt2, v0, == v2 )

        len = 0;
        unsigned v8 = parseUInt8( *ptr, len );
        CHECK( parseUInt8, v0, == v8 )

        len = 0;
        unsigned vn1 = naiveParseUInt1( *ptr, len );
        CHECK( naiveParseUInt1, v0, == vn1 )

        len = 0;
        unsigned vn2 = naiveParseUInt2( *ptr, len );
        CHECK( naiveParseUInt2, vn2, == v0 )
    }
    for( const char **ptr = largeIntegers; *ptr; ++ptr )
    {
        unsigned v0 = (unsigned)atoi( *ptr );
        unsigned len = 0;
        unsigned v2 = parseUInt2( *ptr, len );
        CHECK( parseUInt2, v0, == v2 )

        len = 0;
        unsigned v8 = parseUInt8( *ptr, len );
        CHECK( parseUInt8, v0, == v8 )

        len = 0;
        unsigned vn1 = naiveParseUInt1( *ptr, len );
        CHECK( naiveParseUInt1, v0, == vn1 )

        len = 0;
        unsigned vn2 = naiveParseUInt2( *ptr, len );
        CHECK( naiveParseUInt2, vn2, == v0 )
    }

    for( const char **ptr = hugeIntegers; *ptr; ++ptr )
    {
        std::cout << " huge " << *ptr << "\n";
        long v0 = (long)atoll( *ptr );
        unsigned len = 0;

        long v8 = parseUInt8<long>( *ptr, len );
        CHECK( parseUInt8, v8, == v0 )

        len = 0;
        long vn1 = naiveParseUInt1<long>( *ptr, len );
        CHECK( naiveParseUInt1, v0, == vn1 )

        len = 0;
        long vn2 = naiveParseUInt2<long>( *ptr, len );
        CHECK( naiveParseUInt2, vn2, == v0 )
    }

    pe::Measurement m;
    m.pinToCpuCore( 3 );
    m.addEvent( pe::EventType::cpuCycles );
    m.addEvent( pe::EventType::hwInstructions );
    //m.addEvent( pe::EventType::l1dCacheReadMisses );
    //m.addEvent( pe::EventType::l1dCacheWriteMisses );
    //m.addEvent( pe::EventType::l1iCacheReadMisses );
    //m.addEvent( pe::EventType::l1iCacheWriteMisses );
    m.addEvent( pe::EventType::branchInstructions );
    //m.addEvent( pe::EventType::llCacheReadMisses );
    //m.addEvent( pe::EventType::branchMisses );
    // m.addEvent( pe::EventType::memory );
    const unsigned N = 16;
    m.initialize( N*10 );
    
    std::cout << "\nsmall integers atoi:" << std::endl;
    for( int i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        intValue = atoi( smallIntegers[i%N] );
        m.stopCapture();
        //std::cout << intValue << std::endl;
    }
    m.prepareResults();
    // m.printCaptures();
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
    // m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();

    unsigned len;

    std::cout << "\nsmall integers naiveParseUint1:" << std::endl;
    intValue = spinUpCpu();
    for( int i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        intValue = naiveParseUInt1( smallIntegers[i%N], len );
        m.stopCapture();
        //std::cout << intValue << std::endl;
    }
    m.prepareResults();
    // m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();

    std::cout << "\nsmall integers parseUint8:" << std::endl;
    intValue = spinUpCpu();
    for( int i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        intValue = parseUInt8( smallIntegers[i%N], len );
        m.stopCapture();
        //std::cout << intValue << std::endl;
    }
    m.prepareResults();
    // m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();

    std::cout << "\nsmall integers parseUint2:" << std::endl;
    intValue = spinUpCpu();
    for( int i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        intValue = parseUInt2( smallIntegers[i%N], len );
        m.stopCapture();
        //std::cout << intValue << std::endl;
    }
    m.prepareResults();
    // m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();


    std::cout << "\nhuge integers naiveParseUint1:" << std::endl;
    intValue = spinUpCpu();
    for( int i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        intValue = naiveParseUInt1<uint64_t>( hugeIntegers[i%N], len );
        m.stopCapture();
        //std::cout << intValue << std::endl;
    }
    m.prepareResults();
    // m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();

    std::cout << "\nhuge integers naiveParseUint2:" << std::endl;
    intValue = spinUpCpu();
    for( int i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        intValue = naiveParseUInt2<uint64_t>( hugeIntegers[i%N], len );
        m.stopCapture();
        //std::cout << intValue << std::endl;
    }
    m.prepareResults();
    // m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();

    std::cout << "\nhuge integers parseUint8:" << std::endl;
    intValue = spinUpCpu();
    for( int i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        intValue = parseUInt8<uint64_t>( hugeIntegers[i%N], len );
        m.stopCapture();
        //std::cout << intValue << std::endl;
    }
    m.prepareResults();
    // m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();


    std::cout << "\nlarge integers naiveParseUint1:" << std::endl;
    intValue = spinUpCpu();
    for( int i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        intValue = naiveParseUInt1( largeIntegers[i%N], len );
        m.stopCapture();
        //std::cout << intValue << std::endl;
    }
    m.prepareResults();
    // m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();

    std::cout << "\nlarge integers naiveParseUint2:" << std::endl;
    intValue = spinUpCpu();
    for( int i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        intValue = naiveParseUInt2( largeIntegers[i%N], len );
        m.stopCapture();
        //std::cout << intValue << std::endl;
    }
    m.prepareResults();
    // m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();

    std::cout << "\nlarge integers parseUint8:" << std::endl;
    intValue = spinUpCpu();
    for( int i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        intValue = parseUInt8( largeIntegers[i%N], len );
        m.stopCapture();
        //std::cout << intValue << std::endl;
    }
    m.prepareResults();
    // m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();


    std::cout << "\nlarge integers parseUint2:" << std::endl;
    intValue = spinUpCpu();
    for( int i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        intValue = parseUInt2( largeIntegers[i%N], len );
        m.stopCapture();
        //std::cout << intValue << std::endl;
    }
    m.prepareResults();
    // m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();
    

    return 0;
}

