@echo off
::
:: Created by Martin on 26/07/2012.
::
:: Compile Catch selftests
::

cl -nologo -W3 -EHsc -GR -I../../../include -I../../../include/internal -FeTestCatch.exe  ../../SelfTest/TrickyTests.cpp ../../SelfTest/ApproxTests.cpp ../../SelfTest/ClassTests.cpp ../../SelfTest/ConditionTests.cpp ../../SelfTest/ExceptionTests.cpp ../../SelfTest/GeneratorTests.cpp ../../SelfTest/MessageTests.cpp ../../SelfTest/MiscTests.cpp  &&  TestCatch

:: ../../SelfTest/TestMain.cpp
