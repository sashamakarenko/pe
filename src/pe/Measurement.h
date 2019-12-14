#pragma once

#include <stdint.h>
#include <vector>
#include <iosfwd>

#ifndef _pe_Measurement_h
#define _pe_Measurement_h

namespace pe
{

enum class EventType: unsigned
{
    hwInstructions,
    cpuCycles,
    branchInstructions,
    llCacheReadMisses,
    llCacheWriteMisses,
    branchMisses,
    count,
    memory
};

class Measurement
{
    public:
        
        Measurement();

        ~Measurement();
        
        bool addEvent( EventType evType );

        bool initialize( unsigned maxCaptures );
        
        bool pinToCpuCore( unsigned core );
        
        void startCapture();

        void stopCapture();
        
        void rewind();
        
        std::ostream & showSelfCost( std::ostream & os ) const;

        std::ostream & showNopeCost( std::ostream & os ) const;
        
        std::ostream & showAverageValues( std::ostream & os ) const;

        void prepareResults();

        void printCaptures() const;
        
        unsigned getMaxCaptures() const
        {
            return _maxCaptures;
        }

    private:

        void fakeStopCapture();

        std::ostream & showCapture( std::ostream & os, const std::string & title, const uint64_t * values ) const;
        
        constexpr static int in  = 1;
        constexpr static int out = 0;
        uint64_t * capture( unsigned idx, int inOut ) const;
        
        uint64_t * capturesSubstruct( const uint64_t * left, const uint64_t * right, uint64_t * result ) const;

        uint64_t * capturesAdd( const uint64_t * left, const uint64_t * right, uint64_t * result ) const;

        void computeAverage( uint64_t * result, const uint64_t * cost, unsigned noFirstCapturesToExclude = 0 );

        void processCaptures( const uint64_t * cost = nullptr );
        
        unsigned               _maxCaptures;
        unsigned               _captureCount;
        size_t                 _captureSize;
        std::vector<uint64_t>  _captures;
        uint64_t             * _firstCapture; //
        uint64_t             * _selfCost;     //
        uint64_t             * _nopeCost;     //
        uint64_t             * _avgValues;    // 
        uint64_t               _eventCount;
        std::vector<int>       _fds;
        std::vector<unsigned>  _events;
        bool                   _captureMemory;
};

}

#endif /* _pe_Measurement_h */

