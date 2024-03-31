#include <pe/Measurement.h>
#include <iostream>
#include <utests/TrivialHelper.h>
#include "ArraysExt.h"

unsigned hookCallCount = 0;
void userHook()
{
    ++hookCallCount;
}



const void * u1 = "u1";
const void * u2 = "u2";
const void * u3 = "u3";
const void * u4 = "u4";

Object::User ou1, ou2, ou3, ou4;

int main( int argc, char** argv )
{
    pe::Measurement m;
    m.pinToCpuCore( 3 );
    m.addEvent( pe::EventType::cpuCycles );
    m.addEvent( pe::EventType::hwInstructions );
    m.addEvent( pe::EventType::branchInstructions );
    const unsigned N = 16;
    m.initialize( N );
    
    hookCallCount = 0;
    std::cout << "\nvector:" << std::endl;
    for( unsigned i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        vectorAddUser( u1 );
        vectorAddUser( u2 );
        vectorAddUser( u3 );
        vectorAddUser( u4 );
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
    CHECK( vector hooks, hookCallCount, == m.getMaxCaptures() )

    hookCallCount = 0;
    std::cout << "\narray:" << std::endl;
    for( unsigned i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        arrayAddUser( u1 );
        arrayAddUser( u2 );
        arrayAddUser( u3 );
        arrayAddUser( u4 );
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
    CHECK( array hooks, hookCallCount, == m.getMaxCaptures() )

    hookCallCount = 0;
    std::cout << "\nref count:" << std::endl;
    Object obj;
    for( unsigned i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        ou1 = obj.addUser();
        ou2 = obj.addUser();
        ou3 = obj.addUser();
        ou4 = obj.addUser();
        ou4.reset();
        ou3.reset();
        ou2.reset();
        ou1.reset();
        m.stopCapture();
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();
    CHECK( array hooks, hookCallCount, == m.getMaxCaptures() )

    return 0;
}

