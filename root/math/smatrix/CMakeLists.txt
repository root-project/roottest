ROOTTEST_COMPILE_MACRO(testInversion.cxx)
ROOTTEST_ADD_TEST(testInversion
                MACRO ${CMAKE_CURRENT_SOURCE_DIR}/testInversion.cxx+
                DEPENDS ${COMPILE_MACRO_TEST})

ROOTTEST_COMPILE_MACRO(testKalman.cxx)
ROOTTEST_ADD_TEST(testKalman
                MACRO ${CMAKE_CURRENT_SOURCE_DIR}/testKalman.cxx+
                DEPENDS ${COMPILE_MACRO_TEST})

ROOTTEST_COMPILE_MACRO(testOperations.cxx)
ROOTTEST_ADD_TEST(testOperations
                MACRO ${CMAKE_CURRENT_SOURCE_DIR}/testOperations.cxx+
                DEPENDS ${COMPILE_MACRO_TEST})

ROOTTEST_COMPILE_MACRO(testSMatrix.cxx)
ROOTTEST_ADD_TEST(testSMatrix
                MACRO ${CMAKE_CURRENT_SOURCE_DIR}/testSMatrix.cxx+
                DEPENDS ${COMPILE_MACRO_TEST})
