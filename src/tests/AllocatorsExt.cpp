#include "AllocatorsExt.h"

template< typename T, size_t N > typename SharedAlloc<T,N>::PoolType SharedAlloc<T,N>::_pool;

// not strictly necessary since myMapLongList below will do it
template struct SharedAlloc< typename std::allocator_traits<SharedListInt::allocator_type>::rebind_traits<SharedListInt::value_type>::value_type, 
                             SharedListInt::allocator_type::PoolSize >;

ListInt       stdListInt;
MyListInt     myListInt;
SetInt        stdSetInt;
MySetInt      mySetInt;
MapLongList   stdMapLongList;
MyMapLongList myMapLongList;
