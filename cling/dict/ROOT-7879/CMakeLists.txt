# ROOT-7879
if(MSVC)
    ROOTTEST_ADD_TEST(skipLinkDefDict
                      COMMAND ${ROOT_rootcling_CMD} -f libSkippingDict.cxx -rmf libSkipping.rootmap -rml libSkipping.dll  ${CMAKE_CURRENT_SOURCE_DIR}/skippingHeader.h ${CMAKE_CURRENT_SOURCE_DIR}/skipLinkDef.h
                      OUTREF skipLinkDefDict.ref)
    ROOTTEST_ADD_TEST(skipLinkDef
                      COMMAND findstr clingAutoload libSkippingDict.cxx
                      DEPENDS skipLinkDefDict
                      OUTREF skippingDict_win32.ref)
else()
    ROOTTEST_ADD_TEST(skipLinkDefDict
                      COMMAND ${ROOT_rootcling_CMD} -f libSkippingDict.cxx -rmf libSkipping.rootmap -rml libSkipping.so  ${CMAKE_CURRENT_SOURCE_DIR}/skippingHeader.h ${CMAKE_CURRENT_SOURCE_DIR}/skipLinkDef.h
                      OUTREF skipLinkDefDict.ref)
    ROOTTEST_ADD_TEST(skipLinkDef
                      COMMAND grep clingAutoload libSkippingDict.cxx
                      DEPENDS skipLinkDefDict
                      OUTREF skippingDict.ref)
endif()

ROOTTEST_ADD_TESTDIRS()
