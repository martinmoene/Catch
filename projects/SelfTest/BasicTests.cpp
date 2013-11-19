/*
 *  BasicTests.cpp
 *  Catch - Test
 *
 *  Created by Martin  on 25/07/2012.
 *  Copyright 2010 Two Blue Cubes Ltd. All rights reserved.
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */

#include "catch.hpp"

#include <iostream>
#include <string>

TEST_CASE( "./succeeding/Basic/integral", "integral comparison" )
{
    REQUIRE( 1 == 1 );
    REQUIRE( 1 != 2 );
    REQUIRE( 1 <= 1 );
    REQUIRE( 1 <  2 );
    REQUIRE( 1 >= 1 );
    REQUIRE( 2 >  1 );

    int a = 1;
    int b = 2;

    REQUIRE( a != b );
    REQUIRE( b != a );

    REQUIRE( a <= b );
    REQUIRE( b >= a );
    REQUIRE( a <  b );
    REQUIRE( b >  a );
}

TEST_CASE( "./failing/Basic/integral", "integral comparison" )
{
    CHECK( 1 != 1 );
    CHECK( 1 == 2 );
    CHECK( 2 <= 1 );
    CHECK( 2 <  1 );
    CHECK( 1 >= 2 );
    CHECK( 1 >  1 );

    int a = 1;
    int b = 2;

    CHECK( a == b );
    CHECK( b == a );

    CHECK( b <= a );
    CHECK( a >= b );
    CHECK( b <  a );
    CHECK( a >  b );
}

TEST_CASE( "./succeeding/Basic/mixed_integral", "mixed-type integral comparison" )
{
    REQUIRE( (int)1 == (long)1 );
    REQUIRE( (int)1 != (long)2 );
    REQUIRE( (int)1 <= (long)2 );
    REQUIRE( (int)1 <  (long)2 );
    REQUIRE( (int)2 >= (long)1 );
    REQUIRE( (int)2 >  (long)1 );

    REQUIRE( (long)1 == (int)1 );
    REQUIRE( (long)1 != (int)2 );
    REQUIRE( (long)1 <= (int)2 );
    REQUIRE( (long)1 <  (int)2 );
    REQUIRE( (long)2 >= (int)1 );
    REQUIRE( (long)2 >  (int)1 );

    REQUIRE( (unsigned int)1 == (unsigned long)1 );
    REQUIRE( (unsigned int)1 != (unsigned long)2 );
    REQUIRE( (unsigned int)1 <= (unsigned long)2 );
    REQUIRE( (unsigned int)1 <  (unsigned long)2 );
    REQUIRE( (unsigned int)2 >= (unsigned long)1 );
    REQUIRE( (unsigned int)2 >  (unsigned long)1 );

    REQUIRE( (unsigned long)1 == (unsigned int)1 );
    REQUIRE( (unsigned long)1 != (unsigned int)2 );
    REQUIRE( (unsigned long)1 <= (unsigned int)2 );
    REQUIRE( (unsigned long)1 <  (unsigned int)2 );
    REQUIRE( (unsigned long)2 >= (unsigned int)1 );
    REQUIRE( (unsigned long)2 >  (unsigned int)1 );

    REQUIRE( (unsigned int)1 == (long)1 );
    REQUIRE( (unsigned int)1 != (long)2 );
    REQUIRE( (unsigned int)1 <= (long)2 );
    REQUIRE( (unsigned int)1 <  (long)2 );
    REQUIRE( (unsigned int)2 >= (long)1 );
    REQUIRE( (unsigned int)2 >  (long)1 );

    REQUIRE( (unsigned long)1 == (int)1 );
    REQUIRE( (unsigned long)1 != (int)2 );
    REQUIRE( (unsigned long)1 <= (int)2 );
    REQUIRE( (unsigned long)1 <  (int)2 );
    REQUIRE( (unsigned long)2 >= (int)1 );
    REQUIRE( (unsigned long)2 >  (int)1 );

    REQUIRE( (int)1 == (unsigned long)1 );
    REQUIRE( (int)1 != (unsigned long)2 );
    REQUIRE( (int)1 <= (unsigned long)2 );
    REQUIRE( (int)1 <  (unsigned long)2 );
    REQUIRE( (int)2 >= (unsigned long)1 );
    REQUIRE( (int)2 >  (unsigned long)1 );

    REQUIRE( (long)1 == (unsigned int)1 );
    REQUIRE( (long)1 != (unsigned int)2 );
    REQUIRE( (long)1 <= (unsigned int)2 );
    REQUIRE( (long)1 <  (unsigned int)2 );
    REQUIRE( (long)2 >= (unsigned int)1 );
    REQUIRE( (long)2 >  (unsigned int)1 );
}

//TEST_CASE( "./failing/Basic/mixed_integral", "mixed-type integral comparison" )
//{
//}

TEST_CASE( "./succeeding/Basic/string", "std::string comparison" )
{
    REQUIRE( std::string("hello") == std::string("hello") );
    REQUIRE( std::string("hello") != std::string("world") );

    std::string h = "hello" ;
    std::string w = "world" ;

    REQUIRE( h == h );
    REQUIRE( w == w );

    REQUIRE( h != w );
    REQUIRE( w != h );
}

TEST_CASE( "./failing/Basic/string", "std::string comparison" )
{
    CHECK( std::string("hello") != std::string("hello") );
    CHECK( std::string("hello") == std::string("world") );

    std::string h = "hello" ;
    std::string w = "world" ;

    CHECK( h == w );
    CHECK( w == h );
}

TEST_CASE( "./succeeding/Basic/mixedstring", "C and std::string comparison" )
{
    REQUIRE( std::string("hello") == "hello" );
    REQUIRE( std::string("hello") != "world" );

    REQUIRE( "hello" == std::string("hello") );
    REQUIRE( "hello" != std::string("world") );

    char const * const ch = "hello" ;
    char const * const cw = "world" ;
    std::string h = "hello" ;
    std::string w = "world" ;

    REQUIRE( h == ch );
    REQUIRE( w == cw );

    REQUIRE( ch == h );
    REQUIRE( cw == w );

    REQUIRE( h != cw );
    REQUIRE( w != ch );

    REQUIRE( ch != w );
    REQUIRE( cw != h );
}

TEST_CASE( "./failing/Basic/mixedstring", "C and std::string comparison" )
{
    CHECK( std::string("hello") != "hello" );
    CHECK( std::string("hello") == "world" );

    CHECK( "hello" != std::string("hello") );
    CHECK( "hello" == std::string("world") );

    char const * const ch = "hello" ;
    char const * const cw = "world" ;
    std::string h = "hello" ;
    std::string w = "world" ;

    CHECK( h != ch );
    CHECK( w != cw );

    CHECK( ch != h );
    CHECK( cw != w );

    CHECK( h == cw );
    CHECK( w == ch );

    CHECK( ch == w );
    CHECK( cw == h );
}

TEST_CASE( "./succeeding/Basic/pointer", "pointer--NULL comparison" )
{
    char const * const p = NULL;
    char const * const cp = NULL;

    REQUIRE( p == NULL );
    REQUIRE( cp == NULL );
}
