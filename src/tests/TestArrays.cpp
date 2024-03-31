#include <pe/Measurement.h>
#include <iostream>
#include <utests/TrivialHelper.h>
#include <vector>
#include <array>

void userHook()
{

}

size_t vectorInsertUser( const void * user );
size_t vectorRemoveUser( const void * user );
size_t arrayInsertUser( const void * user );
size_t arrayRemoveUser( const void * user );

const void * u1 = "u1";
const void * u2 = "u2";
const void * u3 = "u3";
const void * u4 = "u4";

int main( int argc, char** argv )
{
    pe::Measurement m;
    m.pinToCpuCore( 3 );
    m.addEvent( pe::EventType::cpuCycles );
    m.addEvent( pe::EventType::hwInstructions );
    m.addEvent( pe::EventType::branchInstructions );
    const unsigned N = 16;
    m.initialize( N );
    
    std::cout << "\nvector:" << std::endl;
    for( unsigned i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        vectorInsertUser( u1 );
        vectorInsertUser( u2 );
        vectorInsertUser( u3 );
        vectorInsertUser( u4 );
        vectorRemoveUser( u4 );
        vectorRemoveUser( u3 );
        vectorRemoveUser( u2 );
        vectorRemoveUser( u1 );
        m.stopCapture();
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();

    std::cout << "\narray:" << std::endl;
    for( unsigned i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        arrayInsertUser( u1 );
        arrayInsertUser( u2 );
        arrayInsertUser( u3 );
        arrayInsertUser( u4 );
        arrayRemoveUser( u4 );
        arrayRemoveUser( u3 );
        arrayRemoveUser( u2 );
        arrayRemoveUser( u1 );
        m.stopCapture();
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();

    return 0;
}

