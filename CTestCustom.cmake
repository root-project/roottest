#-------------------------------------------------------------------------------
#
# CTestCustom.cmake
#
# This file enables customization of CTest.
#
#-------------------------------------------------------------------------------

# Specify tests that will be ignored.

list(APPEND CTEST_CUSTOM_TESTS_IGNORE
            roottest-cling-parsing-semicolon)

if(CTEST_BUILD_NAME MATCHES slc6|centos7)
  list(APPEND CTEST_CUSTOM_TESTS_IGNORE
              roottest-root-meta-LoadAllLibs
              roottest-root-meta-LoadAllLibsAZ
              roottest-root-meta-LoadAllLibsZA
              roottest-root-html-runMakeIndex
              roottest-root-multicore-fork)
endif()
