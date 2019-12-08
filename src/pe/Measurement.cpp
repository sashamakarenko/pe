#include <pe/Measurement.h>
#include <string>
#include <linux/perf_event.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/ioctl.h>
#include <memory.h>
#include <iostream>
#include <sched.h>

namespace pe
{

const std::string EventNames[] =
{
    "hw.instrs",
    "cpu.cycles",
    "br.instrs",
    "cch.ll.rmiss",
    "cch.ll.wmiss",
    "br.misses"
};
    
const uint32_t EventNativeType[] =
{
    PERF_TYPE_HARDWARE,
    PERF_TYPE_HARDWARE,
    PERF_TYPE_HARDWARE,
    PERF_TYPE_HW_CACHE,
    PERF_TYPE_HW_CACHE,
    PERF_TYPE_HARDWARE
};

// you will need:
// sudo echo -1 > /proc/sys/kernel/perf_event_paranoid

#define CACHEMISS_R ( ( PERF_COUNT_HW_CACHE_OP_READ  << 8 ) | ( PERF_COUNT_HW_CACHE_RESULT_MISS << 16 ) ) 
#define CACHEMISS_W ( ( PERF_COUNT_HW_CACHE_OP_WRITE << 8 ) | ( PERF_COUNT_HW_CACHE_RESULT_MISS << 16 ) ) 

const uint64_t EventNativeConfig[] =
{
    PERF_COUNT_HW_INSTRUCTIONS,
    PERF_COUNT_HW_CPU_CYCLES,
    PERF_COUNT_HW_BRANCH_INSTRUCTIONS,
    PERF_COUNT_HW_CACHE_LL | CACHEMISS_R,
    PERF_COUNT_HW_CACHE_LL | CACHEMISS_W,
    PERF_COUNT_HW_BRANCH_MISSES
};

Measurement::Measurement():
    _maxCaptures { 0 },
    _captureCount{ 0 },
    _captureSize { 0 },
    _captures    {},
    _firstCapture{ nullptr },
    _selfCost    { nullptr },
    _nopeCost    { nullptr },
    _eventCount  { 0 },
    _fds         {},
    _events      {}
{
}

Measurement::~Measurement()
{
}

inline uint64_t * Measurement::capture( unsigned idx, int inOut ) const
{
    uint64_t * ptr = _firstCapture - ( (int)idx * 2 - inOut  ) * _eventCount;
    return ptr;
}

uint64_t * Measurement::capturesSubstruct( const uint64_t * left, const uint64_t * right, uint64_t * result ) const
{
    for( unsigned i = 0; i < _eventCount; ++i )
    {
        result[i] = left[i] > right[i] ? left[i] - right[i] : 0;
    }
    return result;
}

uint64_t * Measurement::capturesAdd( const uint64_t * left, const uint64_t * right, uint64_t * result ) const
{
    for( unsigned i = 0; i < _eventCount; ++i )
    {
        result[i] = left[i] + right[i];
    }
    return result;
}

void Measurement::computeAverage( uint64_t * result, const uint64_t * cost, unsigned noFirstCapturesToExclude )
{
    if( noFirstCapturesToExclude >= _captureCount )
    {
        return;
    }
    
    for( unsigned i = noFirstCapturesToExclude; i < _captureCount; ++i )
    {
        capturesAdd( result, capture(i,in), result );
    }
    
    uint64_t count = _captureCount - noFirstCapturesToExclude;
    for( unsigned j = 0; j < _eventCount; ++j )
    {
        result[j] /= count;
    }
}

void Measurement::processCaptures( const uint64_t * cost )
{
    for( unsigned i = 0; i < _captureCount; ++i )
    {
        capturesSubstruct( capture(i,out), capture(i,in), capture(i,in) );
        if( cost )
        {
            capturesSubstruct( capture(i,in), cost, capture(i,in) );
        }
    }
}

bool Measurement::addEvent( EventType evType )
{
    if( evType >= EventType::count )
    {
        std::cerr << "too high event type " << (unsigned)evType << std::endl;
        return false;
    }
    perf_event_attr pe;
    memset( &pe, 0, sizeof(pe) );
    pe.type           = EventNativeType[ (unsigned)evType ];
    pe.size           = sizeof(pe);
    pe.config         = EventNativeConfig[ (unsigned)evType ];
    pe.exclude_kernel = pe.type == PERF_TYPE_SOFTWARE ? 0 : 1;
    pe.exclude_hv     = 1;
    pe.read_format    = PERF_FORMAT_GROUP;
    pe.disabled       = 1;
    
    int groupFd = _fds.size() ? _fds[0] : -1;
    int fd = syscall( __NR_perf_event_open, &pe, 0, -1, groupFd, 0 );
    if( fd == -1 )
    {
        std::cerr << "could not add perf event " << EventNames[ (unsigned)evType ] << std::endl;
        return false;
    }
    _fds.push_back( fd );
    _events.push_back( evType );
    _eventCount = _events.size();
    _captureSize = sizeof(uint64_t) * ( 1 + _eventCount );
    return true;
}

bool Measurement::initialize( unsigned maxCaptures )
{
    if( _eventCount == 0 )
    {
        std::cerr << "You probably forgot to add events or to 'sudo sysctl -w kernel.perf_event_paranoid=1'" << std::endl;
        return false;
    }
    _maxCaptures = maxCaptures;
    _captures.assign( 1 + ( _maxCaptures + 1 ) * 2 * _eventCount + _eventCount, 0 );
    _firstCapture = (&_captures[0]) + 1 + ( _maxCaptures - 1 ) * 2 * _eventCount;
    _avgValues    = capture(0,in) + _eventCount;
    _selfCost     = _avgValues + _eventCount;
    _nopeCost     = _selfCost  + _eventCount;

    for( int fd : _fds )
    {
        ioctl( fd, PERF_EVENT_IOC_RESET , 0 );
        ioctl( fd, PERF_EVENT_IOC_ENABLE, 0 );
    }
    
    unsigned count = 100 < _maxCaptures ? 100 : _maxCaptures;
    
    for( int i = 0; i < count; ++i )
    {
        startCapture();
        stopCapture();
    }
    processCaptures( nullptr );
    computeAverage( _nopeCost, nullptr, 2 );
    _captureCount = 0;

    for( int i = 0; i < count; ++i )
    {
        startCapture();
        fakeStopCapture();
        fakeStopCapture();
        stopCapture();
    }
    processCaptures( _nopeCost );
    computeAverage( _selfCost, nullptr, 2 );
    _captureCount = 0;
    
    return true;
}

bool Measurement::pinToCpuCore( unsigned core )
{
    cpu_set_t cpuSet;
    CPU_ZERO( &cpuSet );
    CPU_SET( core, &cpuSet );
    if( sched_setaffinity( 0, sizeof(cpuSet), &cpuSet ) )
    {
        perror( "sched_setaffinity error" );
        return false;
    }
    return true;
}

void Measurement::rewind()
{
    _captureCount = 0;
}

void Measurement::startCapture()
{
    if( _eventCount == 0 || _captureCount >= _maxCaptures )
    {
        return;
    }
    read( _fds[0], capture(_captureCount,in)-1, _captureSize );
}

void Measurement::stopCapture()
{
    if( _eventCount == 0 || _captureCount >= _maxCaptures )
    {
        return;
    }
    read( _fds[0], capture(_captureCount,out)-1, _captureSize );
    ++_captureCount;
}

void Measurement::fakeStopCapture()
{
    if( _eventCount == 0 || _captureCount >= _maxCaptures )
    {
        return;
    }
    read( _fds[0], capture(_captureCount,out)-1, _captureSize );
    // ++_captureCount;
}

        
std::ostream & Measurement::showCapture( std::ostream & os, const std::string & title, const uint64_t * values ) const
{
    os << "\n" << title << "\n-------------\n";
    for( unsigned i = 0 ; i < _eventCount; ++i )
    {
        os << EventNames[ (int)_events[i] ] << ":\t" << values[i] << "\n";
    }
    return os;
}

std::ostream & Measurement::showSelfCost( std::ostream & os ) const
{
    return showCapture( os, "self cost", _selfCost );
}

std::ostream & Measurement::showNopeCost( std::ostream & os ) const
{
    return showCapture( os, "nope cost", _nopeCost );
}

std::ostream & Measurement::showAverageValues( std::ostream & os ) const
{
    return showCapture( os, "average values", _avgValues );
}

void Measurement::prepareResults()
{
    processCaptures( _nopeCost );
    capturesSubstruct( _avgValues, _avgValues, _avgValues );
    computeAverage( _avgValues, nullptr, 2 );
}

void Measurement::printCaptures() const
{
    printf( "%6s ", "event:" );
    for( unsigned i = 0 ; i < _eventCount; ++i )
    {
        printf( "%16s", EventNames[ (int)_events[i] ].c_str() );
    }
    printf( "\n" );
    for( unsigned i = 0; i < _captureCount; ++i )
    {
        printf( "%6d ", i+1 );
        const uint64_t * values = capture(i,in);
        for( unsigned j = 0 ; j < _eventCount; ++j )
        {
            printf( "%16ld", values[j] );
        }
        printf( "\n" );
    }
}

}
