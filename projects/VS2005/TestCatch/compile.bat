@echo off
::
:: Created by Martin on 20/07/2012.
::
:: Compile Catch selftests
::

::
:: Note: in several stages of its development, Catch-VC6 gives a memory access
:: violation just (code 0xC0000005) before the program ends (all output is there).
:: When a selection of tests is specified, the problem does not show.
:: The access violation is seen when linking to the default runtime library
:: LIBC.LIB, option /ML and with LIBCMT.LIB, option /MT.
:: Linking to MSVCRT.LIB, option /MD, made the (detection of the) access
:: violation go away.
::

cl -nologo -W3 -EHsc -GR -MD -I../../../include -FeTestCatch.exe  ../../SelfTest/TestMain.cpp ../../SelfTest/ApproxTests.cpp ../../SelfTest/BasicTests.cpp ../../SelfTest/BDDTests.cpp ../../SelfTest/ClassTests.cpp ../../SelfTest/CmdLineTests.cpp ../../SelfTest/ConditionTests.cpp ../../SelfTest/ExceptionTests.cpp ../../SelfTest/GeneratorTests.cpp ../../SelfTest/MessageTests.cpp ../../SelfTest/MiscTests.cpp ../../SelfTest/PointerTests.cpp ../../SelfTest/SectionTrackerTests.cpp ../../SelfTest/TrickyTests.cpp  &&  TestCatch
