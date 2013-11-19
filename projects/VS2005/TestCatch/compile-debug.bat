@echo off
::
:: Created by Martin on 25/07/2012.
::
:: Compile Catch selftests
::

cl -nologo -W3 -EHsc -GR -GZ -RTC1 -MTd -I../../../include -I../../../include/internal -FeTestCatch.exe  ../../SelfTest/TestMain.cpp ../../SelfTest/ApproxTests.cpp ../../SelfTest/BasicTests.cpp ../../SelfTest/BDDTests.cpp ../../SelfTest/ClassTests.cpp ../../SelfTest/ConditionTests.cpp ../../SelfTest/ExceptionTests.cpp ../../SelfTest/GeneratorTests.cpp ../../SelfTest/MessageTests.cpp ../../SelfTest/MiscTests.cpp ../../SelfTest/PointerTests.cpp ../../SelfTest/TrickyTests.cpp  &&  TestCatch
