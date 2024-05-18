#include <pe/Measurement.h>
#include <iostream>
#include <utests/TrivialHelper.h>
#include "AllocatorsExt.h"

int main( int argc, char** argv )
{
    pe::Measurement m;
    m.pinToCpuCore( 3 );
    m.addEvent( pe::EventType::cpuCycles );
    m.addEvent( pe::EventType::hwInstructions );
    m.addEvent( pe::EventType::branchInstructions );
    m.addEvent( pe::EventType::busCycles );
    const unsigned N = 16;
    m.initialize( N );

    HIGHLIGHT_CYAN( list push_back std::allocator )
    for( unsigned i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        stdListInt.push_back(i);
        m.stopCapture();
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();

    HIGHLIGHT_CYAN( list remove std::allocator )
    for( unsigned i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        stdListInt.pop_back();
        m.stopCapture();
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();

    HIGHLIGHT_CYAN( list insert std::allocator )
    stdListInt.clear();
    stdListInt.push_back(1);
    stdListInt.push_back(2);
    for( unsigned i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        stdListInt.insert( ++stdListInt.begin(), i );
        m.stopCapture();
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();

    HIGHLIGHT_GREEN( list push_back my allocator )
    for( unsigned i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        myListInt.push_back(i);
        m.stopCapture();
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();

    HIGHLIGHT_GREEN( list remove my allocator )
    for( unsigned i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        myListInt.pop_back();
        m.stopCapture();
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();

    HIGHLIGHT_GREEN( list insert my allocator )
    myListInt.clear();
    myListInt.push_back(1);
    myListInt.push_back(2);
    for( unsigned i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        myListInt.insert( ++myListInt.begin(), i );
        m.stopCapture();
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();

    HIGHLIGHT_CYAN( set add std::allocator )
    for( unsigned i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        stdSetInt.insert(i);
        m.stopCapture();
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();

    HIGHLIGHT_CYAN( set remove std::allocator )
    for( unsigned i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        stdSetInt.erase( stdSetInt.begin() );
        m.stopCapture();
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();

    HIGHLIGHT_CYAN( set insert std::allocator )
    stdSetInt.clear();
    stdSetInt.insert(1);
    stdSetInt.insert(2);
    for( unsigned i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        stdSetInt.insert( ++stdSetInt.begin(), i );
        m.stopCapture();
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();

    HIGHLIGHT_GREEN( set add my allocator )
    for( unsigned i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        mySetInt.insert(i);
        m.stopCapture();
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();

    HIGHLIGHT_GREEN( set remove my allocator )
    for( unsigned i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        mySetInt.erase( mySetInt.begin() );
        m.stopCapture();
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();

    HIGHLIGHT_GREEN( set insert my allocator )
    decltype(mySetInt) tmpSet( std::move(mySetInt) );
    std::swap( tmpSet, mySetInt );
    mySetInt.insert(1);
    mySetInt.insert(2);
    for( unsigned i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        mySetInt.insert( ++mySetInt.begin(), i );
        m.stopCapture();
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();


    HIGHLIGHT_CYAN( map add std allocator )
    for( unsigned i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        auto [ it, success ] = stdMapLongList.try_emplace(long(i));
        it->second.push_back(i);
        m.stopCapture();
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();

    HIGHLIGHT_GREEN( map add my allocator )
    for( unsigned i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        auto [ it, success ] = myMapLongList.try_emplace(long(i));
        it->second.push_back(i);
        m.stopCapture();
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();

    return 0;
}

