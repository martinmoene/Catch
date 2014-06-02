/*
 *  Created by Phil on 11/5/2012.
 *  Copyright 2012 Two Blue Cubes Ltd. All rights reserved.
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef TWOBLUECUBES_CATCH_EXPRESSION_LHS_HPP_INCLUDED
#define TWOBLUECUBES_CATCH_EXPRESSION_LHS_HPP_INCLUDED

#include "catch_expressionresult_builder.h"
#include "catch_evaluate.hpp"
#include "catch_tostring.h"

#ifdef INTERNAL_CATCH_COMPILER_IS_MSVC6
#pragma warning( push )
#pragma warning( disable: 4800 )
#endif

namespace Catch {

// Wraps the LHS of an expression and captures the operator and RHS (if any) -
// wrapping them all in an ExpressionResultBuilder object
template<typename T>
class ExpressionLhs {
    ExpressionLhs& operator = ( ExpressionLhs const& );
#  ifdef CATCH_CPP11_OR_GREATER
    ExpressionLhs& operator = ( ExpressionLhs && ) = delete;
#  endif

public:
    ExpressionLhs( T lhs ) : m_lhs( lhs ) {}
#  ifdef CATCH_CPP11_OR_GREATER
    ExpressionLhs( ExpressionLhs const& ) = default;
    ExpressionLhs( ExpressionLhs && )     = default;
#  endif

    template<typename RhsT>
    ExpressionResultBuilder& operator == ( RhsT const& rhs ) {
        return Internal::Apply<Internal::IsEqualTo>(m_result).captureExpression( m_lhs, rhs, Internal::Preferred() );
    }

    template<typename RhsT>
    ExpressionResultBuilder& operator != ( RhsT const& rhs ) {
        return Internal::Apply<Internal::IsNotEqualTo>(m_result).captureExpression( m_lhs, rhs, Internal::Preferred() );
    }

    template<typename RhsT>
    ExpressionResultBuilder& operator < ( RhsT const& rhs ) {
        return Internal::Apply<Internal::IsLessThan>(m_result).captureExpression( m_lhs, rhs, Internal::Preferred() );
    }

    template<typename RhsT>
    ExpressionResultBuilder& operator > ( RhsT const& rhs ) {
        return Internal::Apply<Internal::IsGreaterThan>(m_result).captureExpression( m_lhs, rhs, Internal::Preferred() );
    }

    template<typename RhsT>
    ExpressionResultBuilder& operator <= ( RhsT const& rhs ) {
        return Internal::Apply<Internal::IsLessThanOrEqualTo>(m_result).captureExpression( m_lhs, rhs, Internal::Preferred() );
    }

    template<typename RhsT>
    ExpressionResultBuilder& operator >= ( RhsT const& rhs ) {
        return Internal::Apply<Internal::IsGreaterThanOrEqualTo>(m_result).captureExpression( m_lhs, rhs, Internal::Preferred() );
    }

    ExpressionResultBuilder& operator == ( bool rhs ) {
        return Internal::Apply<Internal::IsEqualTo>(m_result).captureExpression( m_lhs, rhs, Internal::Preferred() );
    }

    ExpressionResultBuilder& operator != ( bool rhs ) {
        return Internal::Apply<Internal::IsNotEqualTo>(m_result).captureExpression( m_lhs, rhs, Internal::Preferred() );
    }

    ExpressionResultBuilder& endExpression( ResultDisposition::Flags resultDisposition ) {
        bool value = m_lhs ? true : false;
        return m_result
            .setLhs( Catch::toString( value ) )
            .setResultType( value )
            .endExpression( resultDisposition );
    }

    // Only simple binary expressions are allowed on the LHS.
    // If more complex compositions are required then place the sub expression in parentheses
    template<typename RhsT> STATIC_ASSERT_Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison& operator + ( RhsT const& );
    template<typename RhsT> STATIC_ASSERT_Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison& operator - ( RhsT const& );
    template<typename RhsT> STATIC_ASSERT_Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison& operator / ( RhsT const& );
    template<typename RhsT> STATIC_ASSERT_Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison& operator * ( RhsT const& );
    template<typename RhsT> STATIC_ASSERT_Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison& operator && ( RhsT const& );
    template<typename RhsT> STATIC_ASSERT_Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison& operator || ( RhsT const& );

private:
    template<Internal::Operator Op, typename RhsT>
    ExpressionResultBuilder& captureExpression( RhsT const& rhs ) {
        return m_result
            .setResultType( Internal::Comparator<Op>::compare( m_lhs, rhs ) )
            .setLhs( Catch::toString( m_lhs ) )
            .setRhs( Catch::toString( rhs ) )
            .setOp( Internal::OperatorTraits<Op>::getName() );
    }

private:
    ExpressionResultBuilder m_result;
    T m_lhs;
};

} // end namespace Catch

#ifdef INTERNAL_CATCH_COMPILER_IS_MSVC6
#pragma warning( pop )
#endif

#endif // TWOBLUECUBES_CATCH_EXPRESSION_LHS_HPP_INCLUDED
