
template< typename T, T N >
constexpr T dec_zeros()
{
    return ( dec_zeros<T,T(N-1)>() * T(10) ) + T( '0' );
}

template<>
constexpr unsigned short dec_zeros<unsigned short,0U>()
{
    return 0U;
}

template<>
constexpr short dec_zeros<short,0>()
{
    return 0;
}

template<>
constexpr unsigned dec_zeros<unsigned,0U>()
{
    return 0U;
}

template<>
constexpr int dec_zeros<int,0>()
{
    return 0;
}

template<>
constexpr int64_t dec_zeros<int64_t,0L>()
{
    return 0L;
}

template<>
constexpr uint64_t dec_zeros<uint64_t,0UL>()
{
    return 0UL;
}

// in FIX an int ends with SOH or .
constexpr bool isNotDecDigit( char c )
{
    return c < '0';
}

constexpr static const uint64_t uintPow10[] =
{
    1U,
    10U,
    100U,
    1000U,
    1000'0U,
    1000'00U,
    1000'000U,
    1000'000'0U,
    1000'000'00U,
    1000'000'000U,
    1000'000'000'0U,
    1000'000'000'00U,
    1000'000'000'000U,
    1000'000'000'000'0U,
    1000'000'000'000'00U,
    1000'000'000'000'000U,
};

constexpr static const double div10Pow[] =
{
    1,
    0.1,
    0.01,
    0.001,
    0.0001,
    0.00001,
    0.000001,
    0.0000001,
    0.00000001,
    0.000000001,
    0.0000000001,
    0.00000000001
};

template< typename T = unsigned >
inline T parseUInt2( const char * ptr, unsigned & len )
{
    using UC = unsigned char;
    T res = 0;
    UC dif1 = UC(*ptr++) - UC('0');
    if( dif1 <= UC(9) )
    {
        res += T(dif1);
        UC dif2 = UC(*ptr++) - UC('0');
        if( dif2 <= UC(9) )
        {
            res = res * 10 + T(dif2);
            dif1 = UC(*ptr++) - UC('0');
            if( dif1 <= UC(9) )
            {
                res = res * 10 + T(dif1);
                dif2 = UC(*ptr++) - UC('0');
                if( dif2 <= UC(9) )
                {
                    res = res * 10 + T(dif2);
                    dif1 = UC(*ptr++) - UC('0');
                    if( dif1 <= UC(9) )
                    {
                        res = res * 10 + T(dif1);
                        dif2 = UC(*ptr++) - UC('0');
                        if( dif2 <= UC(9) )
                        {
                            res = res * 10 + T(dif2);
                            dif1 = UC(*ptr++) - UC('0');
                            if( dif1 <= UC(9) )
                            {
                                res = res * 10 + T(dif1);
                                dif2 = UC(*ptr++) - UC('0');
                                if( dif2 <= UC(9) )
                                {
                                    res = res * 10 + T(dif2);
                                    dif1 = UC(*ptr++) - UC('0');
                                    if( dif1 <= UC(9) )
                                    {
                                        res = res * 10 + T(dif1);
                                    }
                                    ++len;
                                }
                                ++len;
                            }
                            ++len;
                        }
                        ++len;
                    }
                    ++len;
                }
                ++len;
            }
            ++len;
        }
        ++len;
    }
    return res;
}

template< typename T = unsigned >
inline T parseUInt8( const char * ptr, unsigned & len )
{
    if( isNotDecDigit( ptr[0] ) )
    {
        return 0;
    }
    if( isNotDecDigit( ptr[1] ) )
    {
        len += 1;
        return T( ptr[0] ) - dec_zeros<T,T(1)>();
    }
    if( isNotDecDigit( ptr[2] ) )
    {
        len += 2;
        return T( ptr[0] ) * 10 + T( ptr[1] ) - dec_zeros<T,T(2)>();
    }
    if( isNotDecDigit( ptr[3] ) )
    {
        len += 3;
        return T( ptr[0] ) * 100 + T( ptr[1] ) * 10 + T( ptr[2] ) - dec_zeros<T,T(3)>();
    }
    if( isNotDecDigit( ptr[4] ) )
    {
        len += 4;
        return T( ptr[0] ) * 1000 +
               T( ptr[1] ) * 100 +
               T( ptr[2] ) * 10 +
               T( ptr[3] ) - dec_zeros<T,(4)>();
    }
    if( isNotDecDigit( ptr[5] ) )
    {
        len += 5;
        return T( ptr[0] ) * 10000 +
               T( ptr[1] ) * 1000 +
               T( ptr[2] ) * 100 +
               T( ptr[3] ) * 10 +
               T( ptr[4] ) - dec_zeros<T,T(5)>();
    }
    if( isNotDecDigit( ptr[6] ) )
    {
        len += 6;
        return T( ptr[0] ) * 100000 +
               T( ptr[1] ) * 10000 +
               T( ptr[2] ) * 1000 +
               T( ptr[3] ) * 100 +
               T( ptr[4] ) * 10 +
               T( ptr[5] ) - dec_zeros<T,T(6)>();
    }
    if( isNotDecDigit( ptr[7] ) )
    {
        len += 7;
        return T( ptr[0] ) * 1000000 +
               T( ptr[1] ) * 100000 +
               T( ptr[2] ) * 10000 +
               T( ptr[3] ) * 1000 +
               T( ptr[4] ) * 100 +
               T( ptr[5] ) * 10 +
               T( ptr[6] ) - dec_zeros<T,T(7)>();
    }
    T tmp =    T( ptr[0] ) * 10000000 +
               T( ptr[1] ) * 1000000 +
               T( ptr[2] ) * 100000 +
               T( ptr[3] ) * 10000 +
               T( ptr[4] ) * 1000 +
               T( ptr[5] ) * 100 +
               T( ptr[6] ) * 10 +
               T( ptr[7] ) - dec_zeros<T,T(8)>();
    if( isNotDecDigit( ptr[8] ) )
    {
        len += 8;
        return tmp;
    }

    unsigned tmplen = 0;
    T next = parseUInt8<T>( ptr + 8, tmplen );
    len += tmplen + 8;
    return tmp * (T)uintPow10[ tmplen ] + next;
}


template< typename T = unsigned >
inline T parseUInt( const char * ptr, unsigned & len )
{
    if( isNotDecDigit( ptr[0] ) )
    {
        return 0;
    }
    if( isNotDecDigit( ptr[1] ) )
    {
        len += 1;
        return T( ptr[0] ) - dec_zeros<T,T(1)>();
    }
    if( isNotDecDigit( ptr[2] ) )
    {
        len += 2;
        return T( ptr[0] ) * 10 + T( ptr[1] ) - dec_zeros<T,T(2)>();
    }
    if( isNotDecDigit( ptr[3] ) )
    {
        len += 3;
        return ( T( ptr[0] ) * 10 + T( ptr[1] ) ) * 10 + T( ptr[2] ) - dec_zeros<T,T(3)>();
    }
    if( isNotDecDigit( ptr[4] ) )
    {
        len += 4;
        return ( (
               T( ptr[0] )   * 10 +
               T( ptr[1] ) ) * 10 +
               T( ptr[2] ) ) * 10 +
               T( ptr[3] ) - dec_zeros<T,T(4)>();
    }
    if( isNotDecDigit( ptr[5] ) )
    {
        len += 5;
        return ( ( (
               T( ptr[0] )   * 10 +
               T( ptr[1] ) ) * 10 +
               T( ptr[2] ) ) * 10 +
               T( ptr[3] ) ) * 10 +
               T( ptr[4] ) - dec_zeros<T,T(5)>();
    }
    if( isNotDecDigit( ptr[6] ) )
    {
        len += 6;
        return ( ( ( (
               T( ptr[0] )   * 10 +
               T( ptr[1] ) ) * 10 +
               T( ptr[2] ) ) * 10 +
               T( ptr[3] ) ) * 10 +
               T( ptr[4] ) ) * 10 +
               T( ptr[5] ) - dec_zeros<T,T(6)>();
    }
    if( isNotDecDigit( ptr[7] ) )
    {
        len += 7;
        return ( ( ( ( (
               T( ptr[0] )   * 10 +
               T( ptr[1] ) ) * 10 +
               T( ptr[2] ) ) * 10 +
               T( ptr[3] ) ) * 10 +
               T( ptr[4] ) ) * 10 +
               T( ptr[5] ) ) * 10 +
               T( ptr[6] ) - dec_zeros<T,T(7)>();
    }
    T tmp =    ( ( ( ( ( (
               T( ptr[0] )   * 10 +
               T( ptr[1] ) ) * 10 +
               T( ptr[2] ) ) * 10 +
               T( ptr[3] ) ) * 10 +
               T( ptr[4] ) ) * 10 +
               T( ptr[5] ) ) * 10 +
               T( ptr[6] ) ) * 10 +
               T( ptr[7] ) - dec_zeros<T,T(8)>();
    if( isNotDecDigit( ptr[8] ) )
    {
        len += 8;
        return tmp;
    }

    unsigned tmplen = 0;
    T next = parseUInt<T>( ptr + 8, tmplen );
    len += tmplen + 8;
    return tmp * (T)uintPow10[ tmplen ] + next;
}


template< typename T = unsigned >
inline T parseUInt4( const char * ptr, unsigned & len )
{
    uint32_t mem = *reinterpret_cast<const uint32_t*>(ptr);
    if( isNotDecDigit( mem & 0xff ) )
    {
        return 0;
    }
    if( isNotDecDigit( ( mem >> 8 ) & 0xff ) )
    {
        len += 1;
        return T( mem & 0xff ) - dec_zeros<T,T(1)>();
    }
    if( isNotDecDigit( ( mem >> 16 ) & 0xff  ) )
    {
        len += 2;
        return T( mem & 0xff ) * 10 + T( (mem>>8) & 0xff ) - dec_zeros<T,T(2)>();
    }
    if( isNotDecDigit( ( mem >> 24 ) & 0xff  ) )
    {
        len += 3;
        return T(  mem      & 0xff ) * 100 +
               T( (mem>>8 ) & 0xff ) * 10 +
               T( (mem>>16) & 0xff ) - dec_zeros<T,T(3)>();
    }
    T tmp =    T( ptr[0] ) * 1000 +
               T( ptr[1] ) * 100 +
               T( ptr[2] ) * 10 +
               T( ptr[3] ) - dec_zeros<T,(4)>();
    if( isNotDecDigit( ptr[4] ) )
    {
        len += 4;
        return tmp;
    }

    unsigned tmplen = 0;
    T next = parseUInt4<T>( ptr + 4, tmplen );
    len += tmplen + 4;
    return tmp * (T)uintPow10[ tmplen ] + next;
}

inline double parseDouble( const char * ptr )
{
    unsigned intlen = 0;
    unsigned integer = parseUInt2( ptr, intlen );
    double mantissa = 0.0;
    if( ptr[intlen] == '.' )
    {
        unsigned mantissaLength = 0;
        mantissa = parseUInt2( ptr + intlen + 1, mantissaLength );
        mantissa *= div10Pow[ mantissaLength ];
    }
    return (double)integer + mantissa;
}

template< typename T = unsigned >
inline T naiveParseUInt1( const char * p, unsigned & len )
{
    T x = 0;
    while ( *p >= '0' )
    {
        x = x*10 + T(*p - '0');
        ++p;
        ++len;
    }
    return x;
}

template< typename T = unsigned >
inline T naiveParseUInt2( const char * p, unsigned & len )
{
    using UC = unsigned char;
    T x = 0;
    UC dif1 = UC(*p++) - UC('0');
    while( dif1 < UC(10) )
    {
        x = x * 10 + T(dif1);
        UC dif2 = UC(*p++) - UC('0');
        ++len;
        if( dif2 < UC(10) )
        {
            x = x * 10 + T(dif2);
            dif1 = UC(*p++) - UC('0');
            ++len;
        }
        else
        {
            break;
        }
    }
    return x;
}
