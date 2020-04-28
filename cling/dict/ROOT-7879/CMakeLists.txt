# ROOT-7879
ROOTTEST_ADD_TEST(skipLinkDefDict
                  COMMAND ${ROOT_rootcling_CMD} -f libSkippingDict.cxx -rmf libSkipping.rootmap -rml libSkipping${CMAKE_SHARED_LIBRARY_SUFFIX}  ${CMAKE_CURRENT_SOURCE_DIR}/skippingHeader.h ${CMAKE_CURRENT_SOURCE_DIR}/skipLinkDef.h
                  OUTREF skipLinkDefDict.ref)

if(MSVC)
    ROOTTEST_ADD_TEST(skipLinkDef
                      COMMAND grep clingAutoload libSkippingDict.cxx
                      DEPENDS skipLinkDefDict
                      OUTREF skippingDict.ref)
else()
    ROOTTEST_ADD_TEST(skipLinkDef
                      COMMAND grep clingAutoload libSkippingDict.cxx
                      DEPENDS skipLinkDefDict
                      OUTREF skippingDict_win32.ref)
endif()

ROOTTEST_ADD_TESTDIRS()
