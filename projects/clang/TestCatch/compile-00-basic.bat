@echo off
::
:: Created by Martin on 254/07/2012.
::
:: Compile Catch selftests
::

clang -Wall -I../../../include -o TestCatch.exe  ../../SelfTest/BasicTests.cpp  &&  TestCatch
