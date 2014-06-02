/*
 *  Created by Phil on 8/5/2012.
 *  Copyright 2012 Two Blue Cubes Ltd. All rights reserved.
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef TWOBLUECUBES_CATCH_TOSTRING_H_INCLUDED
#define TWOBLUECUBES_CATCH_TOSTRING_H_INCLUDED

#include "catch_common.h"
#include "catch_sfinae.hpp"

#include <sstream>
#include <iomanip>
#include <limits>
#include <vector>
#include <cstddef>

#ifdef __OBJC__
#include "catch_objc_arc.hpp"
#endif

#ifdef INTERNAL_CATCH_COMPILER_IS_MSVC6
#include "catch_meta_vc6.hpp"
#endif

namespace Catch {
namespace Detail {

// SFINAE is currently disabled by default for all compilers.
// If the non SFINAE version of IsStreamInsertable is ambiguous for you
// and your compiler supports SFINAE, try #defining CATCH_CONFIG_SFINAE
#ifdef CATCH_CONFIG_SFINAE

    template<typename T>
    class IsStreamInsertableHelper {
        template<int N> struct TrueIfSizeable : TrueType {};

        template<typename T2>
        static TrueIfSizeable<sizeof((*(std::ostream*)0) << *((T2 const*)0))> dummy(T2*);
        static FalseType dummy(...);

    public:
        typedef SizedIf<sizeof(dummy((T*)0))> type;
    };

    template<typename T>
    struct IsStreamInsertable : IsStreamInsertableHelper<T>::type {};

#else

    struct BorgType {
        template<typename T> BorgType( T const& );
    };

    TrueType& testStreamable( std::ostream& );
    FalseType testStreamable( FalseType );

    FalseType operator<<( std::ostream const&, BorgType const& );

    template<typename T>
    struct IsStreamInsertable {
        static std::ostream &s;
        static T  const&t;
        enum { value = sizeof( testStreamable(s << t) ) == sizeof( TrueType ) };
    };

#endif

    template<bool C>
    struct StringMakerBase {
        template<typename T>
        static std::string convert( T const& ) { return "{?}"; }
    };

    template<>
    struct StringMakerBase<true> {
        template<typename T>
        static std::string convert( T const& _value ) {
            std::ostringstream oss;
            oss << _value;
            return oss.str();
        }
    };

    struct Endianness {
        enum Arch { Big, Little };

        static Arch which() {
            union _{
                int asInt;
                char asChar[sizeof (int)];
            } u;

            u.asInt = 1;
            return ( u.asChar[sizeof(int)-1] == 1 ) ? Big : Little;
        }
    };

    // Writes the raw memory into a string, considering endianness
    template<typename T>
    std::string rawMemoryToString( T value ) {
        union _ {
            T typedValue;
            unsigned char bytes[sizeof(T)];
        } u;

        u.typedValue = value;

        std::ostringstream oss;
        oss << "0x";

        int i = 0, end = sizeof(T), inc = 1;
        if( Endianness::which() == Endianness::Little ) {
            i = end-1;
            end = inc = -1;
        }
        for( ; i != end; i += inc )
            oss << std::hex << std::setw(2) << std::setfill('0') << (unsigned int)u.bytes[i];
        return oss.str();
    }

} // end namespace Detail

#ifdef INTERNAL_CATCH_COMPILER_IS_MSVC6

/*
 * VC6 does not support partial template specialisation, so we select the
 * proper StringMakerVariant via StringMakerSelector<>.
 */

template<typename T>
struct StringMaker :
    Detail::StringMakerBase<Detail::IsStreamInsertable<T>::value> {};

namespace Detail {
    typedef IntToType<1> valueSelector;
    typedef IntToType<2> pointerSelector;
    typedef IntToType<3> vectorSelector;

    template < typename T >
    struct StringMakerSelector {
        enum {
            value = isVector<T>::value ? (int) vectorSelector::value
                  : isPointer<T>::value ? (int) pointerSelector::value
                  : (int) valueSelector::value
        };
    };
} // end namespace Detail

template<int S>
struct StringMakerVariant {
    template<typename T>
    static std::string convert( T const & value ) {
        return StringMaker<T>::convert( value );
    }
};

template<>
struct StringMakerVariant<Detail::pointerSelector::value> {
    template<typename T>
    static std::string convert( T const * const p ) {
        if( !p )
            return "(NULL)"; // return INTERNAL_CATCH_STRINGIFY( NULL );
        else
            return Detail::rawMemoryToString( p );
    }
};

namespace Detail {
    template<typename InputIterator>
    std::string rangeToString( InputIterator first, InputIterator last );
}

template<>
struct StringMakerVariant<Detail::vectorSelector::value> {
template<typename T, typename Allocator>
    static std::string convert( std::vector<T,Allocator> const& v ) {
        return Detail::rangeToString( v.begin(), v.end() );
    }
};

namespace Detail {
    template<typename T>
    inline std::string makeString( T const & value ) {
       return StringMakerVariant<StringMakerSelector<T>::value >::convert( value );
    }
} // end namespace Detail

template<typename T>
inline std::string toString( T const & value ) {
   return Detail::makeString( value );
}

#else // INTERNAL_CATCH_COMPILER_IS_MSVC6

template<typename T>
std::string toString( T const& value );

template<typename T>
struct StringMaker :
    Detail::StringMakerBase<Detail::IsStreamInsertable<T>::value> {};

template<typename T>
struct StringMaker<T*> {
    template<typename U>
    static std::string convert( U* p ) {
        if( !p )
            return INTERNAL_CATCH_STRINGIFY( NULL );
        else
            return Detail::rawMemoryToString( p );
    }
};

template<typename R, typename C>
struct StringMaker<R C::*> {
    static std::string convert( R C::* p ) {
        if( !p )
            return INTERNAL_CATCH_STRINGIFY( NULL );
        else
            return Detail::rawMemoryToString( p );
    }
};

namespace Detail {
    template<typename InputIterator>
    std::string rangeToString( InputIterator first, InputIterator last );
}

template<typename T, typename Allocator>
struct StringMaker<std::vector<T, Allocator> > {
    static std::string convert( std::vector<T,Allocator> const& v ) {
        return Detail::rangeToString( v.begin(), v.end() );
    }
};

namespace Detail {
    template<typename T>
    std::string makeString( T const& value ) {
        return StringMaker<T>::convert( value );
    }
} // end namespace Detail

/// \brief converts any type to a string
///
/// The default template forwards on to ostringstream - except when an
/// ostringstream overload does not exist - in which case it attempts to detect
/// that and writes {?}.
/// Overload (not specialise) this template for custom typs that you don't want
/// to provide an ostream overload for.
template<typename T>
std::string toString( T const& value ) {
    return StringMaker<T>::convert( value );
}

#endif // INTERNAL_CATCH_COMPILER_IS_MSVC6

// Built in overloads

std::string toString( std::string const& value );
std::string toString( std::wstring const& value );
std::string toString( const char* const value );
std::string toString( char* const value );
std::string toString( int value );
std::string toString( unsigned long value );
std::string toString( unsigned int value );
std::string toString( const double value );
std::string toString( bool value );
std::string toString( char value );
std::string toString( signed char value );
std::string toString( unsigned char value );

#ifdef CATCH_CONFIG_CPP11_NULLPTR
std::string toString( std::nullptr_t );
#endif

#ifdef __OBJC__
    std::string toString( NSString const * const& nsstring );
    std::string toString( NSString * CATCH_ARC_STRONG const& nsstring );
    std::string toString( NSObject* const& nsObject );
#endif

    namespace Detail {
    template<typename InputIterator>
    std::string rangeToString( InputIterator first, InputIterator last ) {
        std::ostringstream oss;
        oss << "{ ";
        if( first != last ) {
            oss << toString( *first );
            for( ++first ; first != last ; ++first ) {
                oss << ", " << toString( *first );
            }
        }
        oss << " }";
        return oss.str();
    }
}

} // end namespace Catch

#endif // TWOBLUECUBES_CATCH_TOSTRING_H_INCLUDED
