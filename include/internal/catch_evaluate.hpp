/*
 *  Created by Phil on 04/03/2011.
 *  Copyright 2011 Two Blue Cubes Ltd. All rights reserved.
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef TWOBLUECUBES_CATCH_EVALUATE_HPP_INCLUDED
#define TWOBLUECUBES_CATCH_EVALUATE_HPP_INCLUDED

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4389) // '==' : signed/unsigned mismatch
#endif

namespace Catch {

namespace Detail {
    // required for VC6:
    class Approx;
}

namespace Internal {

    enum Operator {
        IsEqualTo,
        IsNotEqualTo,
        IsLessThan,
        IsGreaterThan,
        IsLessThanOrEqualTo,
        IsGreaterThanOrEqualTo
    };

    template<Operator Op> struct OperatorTraits             { static const char* getName(){ return "*error*"; } };
    template<> struct OperatorTraits<IsEqualTo>             { static const char* getName(){ return "=="; } };
    template<> struct OperatorTraits<IsNotEqualTo>          { static const char* getName(){ return "!="; } };
    template<> struct OperatorTraits<IsLessThan>            { static const char* getName(){ return "<"; } };
    template<> struct OperatorTraits<IsGreaterThan>         { static const char* getName(){ return ">"; } };
    template<> struct OperatorTraits<IsLessThanOrEqualTo>   { static const char* getName(){ return "<="; } };
    template<> struct OperatorTraits<IsGreaterThanOrEqualTo>{ static const char* getName(){ return ">="; } };
    template<typename T>
    inline T& opCast(T const& t) { return const_cast<T&>(t); }

// nullptr_t support based on pull request #154 from Konstantin Baumann
#ifdef CATCH_CONFIG_CPP11_NULLPTR
    inline std::nullptr_t opCast(std::nullptr_t) { return nullptr; }
#endif // CATCH_CONFIG_CPP11_NULLPTR


    // So the compare overloads can be operator agnostic we convey the operator as a template
    // enum, which is used to specialise an Evaluator for doing the comparison.
    template<Operator Op>
    struct Evaluator
    {
       template<typename T1, typename T2>
       static bool evaluate( T1 const& lhs, T2 const& rhs);
    };

    template<>
    struct Evaluator<IsEqualTo>
    {
       template<typename T1, typename T2>
        static bool evaluate( T1 const& lhs, T2 const& rhs) {
            return opCast( lhs ) ==  opCast( rhs );
        }
    };
    template<>
    struct Evaluator<IsNotEqualTo>
    {
       template<typename T1, typename T2>
        static bool evaluate( T1 const& lhs, T2 const& rhs) {
            return opCast( lhs ) != opCast( rhs );
        }
    };
    template<>
    struct Evaluator<IsLessThan>
    {
       template<typename T1, typename T2>
        static bool evaluate( T1 const& lhs, T2 const& rhs) {
            return opCast( lhs ) < opCast( rhs );
        }
    };
    template<>
    struct Evaluator<IsGreaterThan>
    {
       template<typename T1, typename T2>
        static bool evaluate( T1 const& lhs, T2 const& rhs) {
            return opCast( lhs ) > opCast( rhs );
        }
    };
    template<>
    struct Evaluator<IsGreaterThanOrEqualTo>
    {
       template<typename T1, typename T2>
        static bool evaluate( T1 const& lhs, T2 const& rhs) {
            return opCast( lhs ) >= opCast( rhs );
        }
    };
    template<>
    struct Evaluator<IsLessThanOrEqualTo>
    {
        template<typename T1, typename T2>
        static bool evaluate( T1 const& lhs, T2 const& rhs) {
            return opCast( lhs ) <= opCast( rhs );
        }
    };

    template<Operator Op>
    struct Comparator
    {
        template<typename T1, typename T2>
        static bool compare( T1 const& lhs, T2 const& rhs ) {
            return Evaluator<Op>::evaluate( lhs, rhs );
        }

        // unsigned X to int
        static bool compare( unsigned int lhs, int rhs ) {
            return Evaluator<Op>::evaluate( lhs, static_cast<unsigned int>( rhs ) );
        }
        static bool compare( unsigned long lhs, int rhs ) {
            return Evaluator<Op>::evaluate( lhs, static_cast<unsigned int>( rhs ) );
        }
        static bool compare( unsigned char lhs, int rhs ) {
            return Evaluator<Op>::evaluate( lhs, static_cast<unsigned int>( rhs ) );
        }

        // unsigned X to long
        static bool compare( unsigned int lhs, long rhs ) {
            return Evaluator<Op>::evaluate( lhs, static_cast<unsigned long>( rhs ) );
        }
        static bool compare( unsigned long lhs, long rhs ) {
            return Evaluator<Op>::evaluate( lhs, static_cast<unsigned long>( rhs ) );
        }
        static bool compare( unsigned char lhs, long rhs ) {
            return Evaluator<Op>::evaluate( lhs, static_cast<unsigned long>( rhs ) );
        }

        // int to unsigned X
        static bool compare( int lhs, unsigned int rhs ) {
            return Evaluator<Op>::evaluate( static_cast<unsigned int>( lhs ), rhs );
        }
        static bool compare( int lhs, unsigned long rhs ) {
            return Evaluator<Op>::evaluate( static_cast<unsigned int>( lhs ), rhs );
        }
        static bool compare( int lhs, unsigned char rhs ) {
            return Evaluator<Op>::evaluate( static_cast<unsigned int>( lhs ), rhs );
        }

        // long to unsigned X
        static bool compare( long lhs, unsigned int rhs ) {
            return Evaluator<Op>::evaluate( static_cast<unsigned long>( lhs ), rhs );
        }
        static bool compare( long lhs, unsigned long rhs ) {
            return Evaluator<Op>::evaluate( static_cast<unsigned long>( lhs ), rhs );
        }
        static bool compare( long lhs, unsigned char rhs ) {
            return Evaluator<Op>::evaluate( static_cast<unsigned long>( lhs ), rhs );
        }

        // pointer to long (when comparing against NULL)
        template<typename T>
        static bool compare( long lhs, T const* rhs ) {
            return Evaluator<Op>::evaluate( reinterpret_cast<T const*>( lhs ), rhs );
        }

        template<typename T>
        static bool compare( T const* lhs, long rhs ) {
            return Evaluator<Op>::evaluate( lhs, reinterpret_cast<T const*>( rhs ) );
        }

        template<typename T>
        static bool compare( long lhs, T* rhs ) {
            return Evaluator<Op>::evaluate( reinterpret_cast<T*>( lhs ), rhs );
        }

        template<typename T>
        static bool compare( T* lhs, long rhs ) {
            return Evaluator<Op>::evaluate( lhs, reinterpret_cast<T*>( rhs ) );
        }

        // pointer to int (when comparing against NULL)
        template<typename T>
        static bool compare( int lhs, T const* rhs ) {
            return Evaluator<Op>::evaluate( reinterpret_cast<T const*>( lhs ), rhs );
        }

        template<typename T>
        static bool compare( T const* lhs, int rhs ) {
            return Evaluator<Op>::evaluate( lhs, reinterpret_cast<T const*>( rhs ) );
        }

        template<typename T>
        static bool compare( int lhs, T* rhs ) {
            return Evaluator<Op>::evaluate( reinterpret_cast<T*>( lhs ), rhs );
        }

        template<typename T>
        static bool compare( T* lhs, int rhs ) {
            return Evaluator<Op>::evaluate( lhs, reinterpret_cast<T*>( rhs ) );
        }
    };

#ifdef CATCH_CONFIG_CPP11_NULLPTR
    // pointer to nullptr_t (when comparing against nullptr)
    template<Operator Op, typename T> bool compare( std::nullptr_t, T* rhs ) {
        return Evaluator<Op>::evaluate( NULL, rhs );
    }
    template<Operator Op, typename T> bool compare( T* lhs, std::nullptr_t ) {
        return Evaluator<Op>::evaluate( lhs, NULL );
    }
#endif // CATCH_CONFIG_CPP11_NULLPTR

} // end of namespace Internal
} // end of namespace Catch

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif // TWOBLUECUBES_CATCH_EVALUATE_HPP_INCLUDED
