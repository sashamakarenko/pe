#include <list>
#include <set>
#include <map>
#include <iostream>

// single item allocators (not good for vector and deque)

template< size_t IS, size_t PS = 1024 >
struct Pool
{

    static constexpr size_t ItemSize = IS;
    static constexpr size_t PoolSize = PS;

    Pool()
    {
        for( size_t i = 0; i < PoolSize; ++i )
        {
            _head = new Elem( _head );
        }
    }

    Pool( const Pool & ) = delete;

    Pool( Pool && other )
    {
        _head = other._head;
        other._head = nullptr;
    }

    ~Pool()
    {
        while( _head )
        {
            Elem * head = _head;
            _head = _head->next;
            delete head;
        }
    }

    char * allocate()
    {
        if( _head == nullptr )
        {
            _head = new Elem( nullptr );
        }
        char * ptr = _head->data;
        _head = _head->next;
        return ptr;
    }

    void deallocate( char * ptr )
    {
        Elem * recycled = reinterpret_cast<Elem*>( ptr - sizeof(Elem*) );
        recycled->next = _head;
        _head = recycled;
    }

    struct Elem
    {
        Elem( Elem * n ): next{n} {}
        Elem * next; // preceding pointer will properly align data of any type
        char   data[ ItemSize ];
    };

    Elem * _head = nullptr;
};

template< typename T, size_t N = 1024 >
struct MyAlloc: std::allocator<T>
{
    MyAlloc()
    {
    }

    MyAlloc( const MyAlloc & ) = delete;

    MyAlloc( MyAlloc && other )
    : _pool( std::move( other._pool ) )
    {
    }

    ~MyAlloc()
    {
    }

    template< typename U > struct rebind
    {
        using other = MyAlloc<U,N>;
    };

    using pointer = T *;
    
    static constexpr size_t PoolSize = N;

    using PoolType = Pool<sizeof(T),PoolSize>;

    pointer allocate( size_t n )
    {
        return reinterpret_cast<pointer>( _pool.allocate() );
    }

    void deallocate( pointer ptr, size_t n )
    {
        _pool.deallocate( reinterpret_cast<char*>(ptr) );
    }

    PoolType _pool;
};

template< typename T, size_t N = 1024 >
struct SharedAlloc: std::allocator<T>
{
    template< typename U > struct rebind
    {
        using other = SharedAlloc<U,N>;
    };

    using pointer = T *;

    static constexpr size_t PoolSize = N;

    using PoolType = Pool<sizeof(T),PoolSize>;
    
    pointer allocate( size_t n )
    {
        return reinterpret_cast<pointer>( _pool.allocate() );
    }

    void deallocate( pointer ptr, size_t n )
    {
        _pool.deallocate( reinterpret_cast<char*>(ptr) );
    }

    static PoolType _pool;

};

using ListInt       = std::list<int>;
using MyListInt     = std::list<int,MyAlloc<int,128>>;
using SharedListInt = std::list<int,SharedAlloc<int,1024>>;
using SetInt        = std::set<int>;
using MySetInt      = std::set<int,std::less<int>,MyAlloc<int,128>>;
using MapLongList   = std::map<long,ListInt>;
using MyMapLongList = std::map<long,SharedListInt,std::less<long>,MyAlloc<std::pair<const long,SharedListInt>,1024>>;

extern ListInt       stdListInt;
extern MyListInt     myListInt;
extern SetInt        stdSetInt;
extern MySetInt      mySetInt;
extern MapLongList   stdMapLongList;
extern MyMapLongList myMapLongList;
