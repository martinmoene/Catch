/*
 *  PointerTests.cpp
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

TEST_CASE( "./succeeding/Basic/char_pointer", "pointer--NULL comparison" )
{
    char * p = NULL;
    const char * cp = NULL;

    REQUIRE( p == NULL );
    REQUIRE( cp == NULL );
}

struct X {};

TEST_CASE( "./succeeding/Basic/struct_pointer", "pointer--NULL comparison" )
{
    X * p = NULL;
    char * cp = NULL;

    REQUIRE( p == NULL );
    REQUIRE( cp == NULL );
}
