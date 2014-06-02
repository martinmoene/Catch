/*
 *  Created by Phil on 8/5/2012.
 *  Copyright 2012 Two Blue Cubes Ltd. All rights reserved.
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef TWOBLUECUBES_CATCH_ASSERTIONRESULT_BUILDER_H_INCLUDED
#define TWOBLUECUBES_CATCH_ASSERTIONRESULT_BUILDER_H_INCLUDED

#include "catch_tostring.h"
#include "catch_assertionresult.h"
#include "catch_result_type.h"
#include "catch_evaluate.hpp"
#include "catch_common.h"

namespace Catch {

struct STATIC_ASSERT_Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison;

// Wraps the (stringised versions of) the lhs, operator and rhs of an expression - as well as
// the result of evaluating it. This is used to build an AssertionResult object
class ExpressionResultBuilder {
public:

    ExpressionResultBuilder( ResultWas::OfType resultType = ResultWas::Unknown );
    ExpressionResultBuilder( ExpressionResultBuilder const& other );
    ExpressionResultBuilder& operator=(ExpressionResultBuilder const& other );

    ExpressionResultBuilder& setResultType( ResultWas::OfType result );
    ExpressionResultBuilder& setResultType( bool result );
    ExpressionResultBuilder& setLhs( std::string const& lhs );
    ExpressionResultBuilder& setRhs( std::string const& rhs );
    ExpressionResultBuilder& setOp( std::string const& op );

    ExpressionResultBuilder& endExpression( ResultDisposition::Flags resultDisposition );

    template<typename T>
    ExpressionResultBuilder& operator << ( T const& value ) {
        m_stream << value;
        return *this;
    }

    std::string reconstructExpression( AssertionInfo const& info ) const;

    AssertionResult buildResult( AssertionInfo const& info ) const;

    template<typename RhsT> STATIC_ASSERT_Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison& operator && ( RhsT const& );
    template<typename RhsT> STATIC_ASSERT_Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison& operator || ( RhsT const& );

private:
    AssertionResultData m_data;
public:
    struct ExprComponents {
        ExprComponents() : shouldNegate( false ) {}
        bool shouldNegate;
        std::string lhs, rhs, op;
    } m_exprComponents;
private:
    std::ostringstream m_stream;
};

namespace Internal {

    // help VC6 partial ordering:
    struct Basic {};
    struct Preferred : Basic {};

    template<Operator Op>
    class Apply
    {
    public:
        Apply( ExpressionResultBuilder & result )
        : m_result( result ) {}

        template<typename T1, typename T2>
        ExpressionResultBuilder& captureExpression( const T1& lhs, const T2& rhs, Basic const &  ) {
            m_result.setResultType( Comparator<Op>::compare( lhs, rhs ) ? ResultWas::Ok : ResultWas::ExpressionFailed );
            m_result.m_exprComponents.lhs = Catch::toString( lhs );
            m_result.m_exprComponents.rhs = Catch::toString( rhs );
            m_result.m_exprComponents.op = Internal::OperatorTraits<Op>::getName();
            return m_result;
        }

        template<typename T>
#ifdef INTERNAL_CATCH_COMPILER_IS_MSVC6
        ExpressionResultBuilder& captureExpression( const T* lhs, const int& rhs, Preferred const & ) {
#else
        ExpressionResultBuilder& captureExpression( const T* lhs, const int& rhs, Basic const & ) {
#endif
            return captureExpression( lhs, reinterpret_cast<const T*>( rhs ), Basic() );
        }

        template<typename T>
#ifdef INTERNAL_CATCH_COMPILER_IS_MSVC6
        ExpressionResultBuilder& captureExpression( const int& lhs, const T* rhs, Preferred const & ) {
#else
        ExpressionResultBuilder& captureExpression( const int& lhs, const T* rhs, Basic const & ) {
#endif
            return captureExpression( reinterpret_cast<const T*>( lhs ), rhs, Basic() );
        }

    private:
        ExpressionResultBuilder & m_result;
    };

} // end namespace Internal
} // end namespace Catch

#endif // TWOBLUECUBES_CATCH_ASSERTIONRESULT_BUILDER_H_INCLUDED
