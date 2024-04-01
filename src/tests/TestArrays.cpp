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
    int j = 0;
    for( int i = 0; i < 100000; ++i )
    {
        j += spinUpCpu(j+i);
    }
    pe::Measurement m;
    m.pinToCpuCore( 3 );
    m.addEvent( pe::EventType::cpuCycles );
    m.addEvent( pe::EventType::hwInstructions );
    m.addEvent( pe::EventType::branchInstructions );
    m.addEvent( pe::EventType::busCycles );
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
    Object::Ptr obj = std::make_shared<Object>();
    for( unsigned i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        obj->addUser( ou1 );
        obj->addUser( ou2 );
        obj->addUser( ou3 );
        obj->addUser( ou4 );
        obj->removeUser( ou4 );
        obj->removeUser( ou3 );
        obj->removeUser( ou2 );
        obj->removeUser( ou1 );
        m.stopCapture();
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();
    CHECK( ref count, hookCallCount, == m.getMaxCaptures() )

    hookCallCount = 0;
    std::cout << "\nbitmask count:" << std::endl;
    for( unsigned i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        obj->addUser( 1 );
        obj->addUser( 2 );
        obj->addUser( 3 );
        obj->addUser( 4 );
        obj->removeUser( 4 );
        obj->removeUser( 3 );
        obj->removeUser( 2 );
        obj->removeUser( 1 );
        m.stopCapture();
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();
    CHECK( bitmask count, hookCallCount, == m.getMaxCaptures() )

    return 0;
}

