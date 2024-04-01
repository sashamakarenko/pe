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
int spinUpCpu(int);

struct Object: std::enable_shared_from_this<Object>
{
    using Ptr = std::shared_ptr<Object>;

    struct User
    {
        Ptr ptr;
    };

    void addUser( User & user );

    void removeUser( User & user );

    std::atomic_int32_t _users = 0;

    void addUser( unsigned idx );

    void removeUser( unsigned idx );
};
