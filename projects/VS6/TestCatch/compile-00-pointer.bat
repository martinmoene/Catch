@echo off
::
:: Created by Martin on 24/07/2012.
::
:: Compile Catch selftests
::

::cl -nologo -W3 -EHsc -GZ -Zi -GR -I../../../include -FeTestCatch.exe  ./BasicTests.cpp  &&  TestCatch

cl -nologo -W3 -EHsc -GR -I../../../include -FeTestCatch.exe  ../../SelfTest/PointerTests.cpp  &&  TestCatch
