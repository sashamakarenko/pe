# pe - perf event wrapper
linux perf event c++ wrapper

Measure how much your code costs in terms of hardware instructions, cachemisses, branchmisses and memory allocations.

## 1. Compile pe

You will need sashamakarenko/makefile project cloned next to the pe working copy.

```
$> git clone https://github.com/sashamakarenko/makefile.git makefile
$> git clone https://github.com/sashamakarenko/pe.git pe
$> cd pe
$> make
```

## 2. Create code snippet
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

##  3. Compile

```
$> make
```

## 4. Run measurement

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
```

Relax the CPU
```
$> sudo cpufreq-set -g powersave
```

## 5. Results

The metrics correspond to **10 calls** in a row.
Hopefully the column names are self-explanatory.
The column `nanos/call` correspond to a single call time on a 5GHz

### Cost of time measurement functions

|event|nanos/call|cpu.cycles|hw.instrs|br.instrs|cch.ll.rmiss|br.misses|bus.cycles|cch.l1d.rmiss|cch.l1i.rmiss|
|-----|----:|----:|----:|----:|----:|----:|----:|----:|----:|
|**gettimeofday**|13.2|661|821|140|0|0|3|2|0|
|**clock_gettime**|14.3|714|911|170|0|3|3|3|0|
|**rdtsc**|7.0|352|61|0|0|0|2|0|0|

### Functions and methods

See `src/tests/TestLibCalls.cpp` for details. 
In order to avoid compiler call evictions, for the functions returning int we actually measure:

```cpp
externVolatileInt += function();
externVolatileInt += function();
... // repeated 10 times
externVolatileInt += function();
```

#### Without LTO

|event|nanos/call|cpu.cycles|hw.instrs|br.instrs|cch.ll.rmiss|br.misses|bus.cycles|cch.l1d.rmiss|cch.l1i.rmiss|
|-----|----:|----:|----:|----:|----:|----:|----:|----:|----:|
|void()|1.4|70|20|20|0|0|0|0|0|
|void(int)|0.7|36|30|20|0|0|0|0|0|
|int(int)|0.9|43|81|20|0|0|0|0|0|
|inline int base.get()|0.7|37|43|0|0|0|0|0|0|
|int base.get()|0.9|45|83|20|0|0|0|0|0|
|int base.*getIntPtr()|1.2|61|123|30|0|0|0|0|0|
|virtual int base.get()|0.9|43|93|20|0|0|0|0|0|
|virtual int derived.get()|0.9|43|93|20|0|0|0|0|0|
|virtual int virtDerived.get()|1.3|67|133|20|0|0|0|0|0|
|inline base.getIndirect()|1.0|52|46|0|0|0|0|0|0|
|inline derived.getIndirect()|1.0|48|46|0|0|0|0|0|0|

#### With LTO

|event|nanos/call|cpu.cycles|hw.instrs|br.instrs|cch.ll.rmiss|br.misses|bus.cycles|cch.l1d.rmiss|cch.l1i.rmiss|
|-----|----:|----:|----:|----:|----:|----:|----:|----:|----:|
|void()|0.0|0|0|0|0|0|0|0|0|
|void(int)|0.0|0|0|0|0|0|0|0|0|
|int(int)|1.0|48|30|0|0|0|0|0|0|
|inline int base.get()|0.9|47|40|0|0|0|0|0|0|
|int base.get()|0.9|44|40|0|0|0|0|0|0|
|int base.*getIntPtr()|1.3|64|120|30|0|0|0|0|0|
|virtual int base.get()|1.0|48|71|20|0|0|0|0|0|
|virtual int derived.get()|0.9|45|71|20|0|0|0|0|0|
|virtual int virtDerived.get()|1.5|77|120|20|0|0|0|0|0|
|inline base.getIndirect()|1.0|48|40|0|0|0|0|0|0|
|inline derived.getIndirect()|0.9|43|40|0|0|0|0|0|0|

[![HitCount](https://hits.dwyl.com/sashamakarenko/pe.svg?style=flat)](http://hits.dwyl.com/sashamakarenko/pe)