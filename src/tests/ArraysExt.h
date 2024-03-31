#include <memory>
#include <atomic>
#include <thread>

struct SpinLock
{
    SpinLock( bool relaxed = true ) noexcept
    : _relaxed{ relaxed }
    , _flag{ ATOMIC_FLAG_INIT }
    {
    }

    void lock() noexcept
    {
        while( _flag.test_and_set() )
        {
            if( _relaxed )
            {
                std::this_thread::yield();
            }
        }
    }

    void unlock() noexcept
    {
        _flag.clear();
    }

    const bool       _relaxed;
    std::atomic_flag _flag;
};

void userHook();
void vectorAddUser( const void * user );
void vectorRemoveUser( const void * user );
void arrayAddUser( const void * user );
void arrayRemoveUser( const void * user );

struct Object
{
    struct Recycler
    {
        void operator()( Object * ptr );
    };

    using User = std::unique_ptr<Object,Recycler>;
    
    User addUser();

    std::atomic_uint32_t _users = 0;
};
