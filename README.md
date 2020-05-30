# pe - perf event wrapper
linux perf event c++ wrapper

Measure how much your code costs in terms of hardware instructions, cachemisses, branchemisses and memory allocations.

## 1. Compile pe ##

You will need sashamakarenko/makefile project cloned next to the pe working copy.

```
$> git clone https://github.com/sashamakarenko/makefile.git makefile
$> git clone https://github.com/sashamakarenko/pe.git pe
$> cd pe
$> make
```

## 2. Create code snippet ##
Let us invoke gettimeofday 20 times on the CPU core 3 and get statistics from this.
Instrument your code or create pe/src/tests/TestGettimeofday.cpp like this:

```cpp
#include <pe/Measurement.h>

int main( int argc, char** argv )
{
    pe::Measurement m;
    m.pinToCpuCore( 3 );
    m.addEvent( pe::EventType::cpuCycles );
    m.addEvent( pe::EventType::hwInstructions );
    m.addEvent( pe::EventType::branchInstructions );
    m.addEvent( pe::EventType::llCacheReadMisses );
    m.addEvent( pe::EventType::branchMisses );
    m.addEvent( pe::EventType::memory );
    m.initialize( 20 );
    
    std::cout << "\ngettimeofday:" << std::endl;
    for( int i = 0; i < m.getMaxCaptures(); ++i )
    {
        m.startCapture();
        // this is what we measure
        gettimeofday( &tv, nullptr );
        m.stopCapture();
    }
    m.prepareResults();
    m.printCaptures();
    m.showAverageValues( std::cout );
    m.rewind();
    return 0;
}
```

##  3. Compile ##

```
$> make
```

## 4. Run measurement ##

Enable perf events:
```
$> sudo echo -1 > /proc/sys/kernel/perf_event_paranoid
```

Bring the frequency up
```
$> sudo cpufreq-set -c 3 -g performance
```

Run the code
```
$> make check
gettimeofday:
event:       cpu.cycles       hw.instrs       br.instrs    cch.ll.rmiss       br.misses      bus.cycles   cch.l1d.rmiss   cch.l1i.rmiss
     1             1755              83              15               0               4               9              19              43
     2              104              82              14               0               1               0               0               0
     3               76              82              14               0               0               0               0               0
     4               78              82              14               0               0               0               0               0
     5               77              82              14               0               0               0               0               0
     6               77              82              14               0               0               1               0               0
     7               78              82              14               0               0               1               0               0
     8               79              82              14               0               0               0               0               0
   avg:              77              82              14               0               0               0               0               0

```

Relax the CPU
```
$> sudo cpufreq-set -g powersave
```

## 5. Results ##

Hopefully the colun names are self explainable.
One can convert CPU cycles to time (if frequency is stable) by deviding number of cycles by CPU frequency.
In my case a gettimeofday casts in average 77/5GHz = 15 nanos.
