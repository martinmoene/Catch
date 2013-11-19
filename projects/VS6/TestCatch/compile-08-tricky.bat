@echo off
::
:: Created by Martin on 25/07/2012.
::
:: Compile Catch selftests
::

cl -nologo -W3 -EHsc -GR -I../../../include -FeTestCatch.exe  ../../SelfTest/TrickyTests.cpp  &&  TestCatch
