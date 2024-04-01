#include <vector>
#include <array>
#include <algorithm>
#include <stdexcept>
#include <mutex>
#include "ArraysExt.h"

int spinUpCpu(int i)
{
    return ( i << 12 ) / ( i == 0 ? 1 : 2 );
}

SpinLock usersLock;

void Object::addUser( Object::User & user )
{
    if( user.ptr and user.ptr.get() != this )
    {
        user.ptr->removeUser( user );
    }
    if( not user.ptr )
    {
        ++_users;
        user.ptr = shared_from_this();
    }
}

void Object::removeUser( Object::User & user )
{
    if( user.ptr.get() == this )
    {
        user.ptr.reset();
        if( --_users == 0 )
        {
            userHook();
        }
    }
}

void Object::addUser( unsigned idx )
{
    _users.fetch_or( 1 << idx );

}

void Object::removeUser( unsigned idx )
{
    int32_t oldValue = _users.load();
    int32_t newValue = oldValue & ~( 1 << idx );
    while( not _users.compare_exchange_weak( oldValue, newValue ) )
    {
        newValue = oldValue & ~( 1 << idx );
    }
    if( newValue == 0 )
    {
        userHook();
    }
}

std::vector<const void*> vec;

void vectorAddUser( const void * user )
{
    if( user )
    {
        std::scoped_lock<SpinLock> protectUsers( usersLock );
        auto it = std::find( vec.begin(), vec.end(), user );
        if( it == vec.end() )
        {
            vec.emplace_back( user );
        }
    }
}

void vectorRemoveUser( const void * user )
{
    if( user )
    {
        std::scoped_lock<SpinLock> protectUsers( usersLock );
        auto it = std::find( vec.begin(), vec.end(), user );
        if( it != vec.end() )
        {
            vec.erase( it );
        }
        if( vec.empty() )
        {
            userHook();
        }
    }
}

// std::array<const void*,4> arr;
constexpr unsigned arrSz = 8;
const void * arr[arrSz]{};
int userCount = 0;

void arrayAddUser( const void * user )
{
    if( user == nullptr )
    {
        return;
    }
    std::scoped_lock<SpinLock> protectUsers( usersLock );
    switch( userCount )
    {
        case 7:
            if( arr[6] == user ) return;
            [[fallthrough]];
        case 6:
            if( arr[5] == user ) return;
            [[fallthrough]];
        case 5:
            if( arr[4] == user ) return;
            [[fallthrough]];
        case 4:
            if( arr[3] == user ) return;
            [[fallthrough]];
        case 3:
            if( arr[2] == user ) return;
            [[fallthrough]];
        case 2:
            if( arr[1] == user ) return;
            [[fallthrough]];
        case 1:
            if( arr[0] == user ) return;
            [[fallthrough]];
        case 0:
            arr[ userCount++ ] = user;
            break;
        default:
            throw std::out_of_range("too much");
            break;
    }
}

void arrayRemoveUser( const void * user )
{
    std::scoped_lock<SpinLock> protectUsers( usersLock );
    int pos = -1;
    switch( userCount )
    {
        case 8:
            if( arr[7] == user )
            {
                pos = 7;
                break;
            }
            [[fallthrough]];
        case 7:
            if( arr[6] == user )
            {
                pos = 6;
                break;
            }
            [[fallthrough]];
        case 6:
            if( arr[5] == user )
            {
                pos = 5;
                break;
            }
            [[fallthrough]];
        case 5:
            if( arr[4] == user )
            {
                pos = 4;
                break;
            }
            [[fallthrough]];
        case 4:
            if( arr[3] == user )
            {
                pos = 3;
                break;
            }
            [[fallthrough]];
        case 3:
            if( arr[2] == user )
            {
                pos = 2;
                break;
            }
            [[fallthrough]];
        case 2:
            if( arr[1] == user )
            {
                pos = 1;
                break;
            }
            [[fallthrough]];
        case 1:
            if( arr[0] == user )
            {
                pos = 0;
                break;
            }
            [[fallthrough]];
        default:
            break;
    }
    if( pos >= 0 )
    {
        --userCount;
        for( int i = pos; i < userCount; ++i )
        {
            arr[i] = arr[i+1];
        }
        if( userCount == 0 )
        {
            userHook();
        }
    }
}

void arrayAddUserX( const void * user )
{
    if( user == nullptr )
    {
        return;
    }
    int firstFreePos = -1;
    for( unsigned i = 0; i < arrSz; ++i )
    {
        if( arr[i] != nullptr )
        {
            if( arr[i] == user )
            {
                return;
            }
        }
        else if( firstFreePos < 0 )
        {
            firstFreePos = (int)i;
        }
    }
    if( firstFreePos >= 0 )
    {
        arr[firstFreePos] = user;
    }
}

void arrayRemoveUserX( const void * user )
{
    for( unsigned i = 0; i < arrSz; ++i )
    {
        if( arr[i] == user )
        {
            arr[i] = nullptr;
            return;
        }
    }
}

void arrayAddUserY( const void * user )
{
    if( user == nullptr )
    {
        return;
    }
    
    for( unsigned i = 0; i < arrSz; ++i )
    {
        if( arr[i] == user )
        {
            return;
        }
    }

    if( arr[0] == nullptr )
    {
        arr[0] = user;
        return;
    }
    if( arr[1] == nullptr )
    {
        arr[1] = user;
        return;
    }
    if( arr[2] == nullptr )
    {
        arr[2] = user;
        return;
    }
    if( arr[3] == nullptr )
    {
        arr[3] = user;
        return;
    }
    if( arr[4] == nullptr )
    {
        arr[4] = user;
        return;
    }
    if( arr[5] == nullptr )
    {
        arr[5] = user;
        return;
    }
}

bool arrayEmpty()
{
    if( arr[0] != nullptr )
    {
        return false;
    }
    if( arr[1] != nullptr )
    {
        return false;
    }
    if( arr[2] != nullptr )
    {
        return false;
    }
    if( arr[3] != nullptr )
    {
        return false;
    }
    if( arr[4] != nullptr )
    {
        return false;
    }
    if( arr[5] != nullptr )
    {
        return false;
    }
    return true;
}

void arrayRemoveUserY( const void * user )
{
    if( arr[0] == user )
    {
        arr[0] = nullptr;
        if( arrayEmpty() ) userHook();
        return;
    }
    if( arr[1] == user )
    {
        arr[1] = nullptr;
        if( arrayEmpty() ) userHook();
        return;
    }
    if( arr[2] == user )
    {
        arr[3] = nullptr;
        if( arrayEmpty() ) userHook();
        return;
    }
    if( arr[3] == user )
    {
        arr[3] = nullptr;
        if( arrayEmpty() ) userHook();
        return;
    }
    if( arr[4] == user )
    {
        arr[4] = nullptr;
        if( arrayEmpty() ) userHook();
        return;
    }
    if( arr[5] == user )
    {
        arr[5] = nullptr;
        if( arrayEmpty() ) userHook();
        return;
    }
}
