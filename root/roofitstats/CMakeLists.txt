if(ROOT_roofit_FOUND)
set(ROOFITLIBRARIES RooFitCore RooFit HistFactory)

ROOTTEST_ADD_TEST(read-scientificnotation
                  COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/read_scientific_notation.py
                  OUTCNVCMD grep -v -e "Wouter"
                  OUTREF read_scientific_notation.ref)

ROOTTEST_GENERATE_EXECUTABLE(testDestructRooGaussian
                  testDestructRooGaussian testDestructRooGaussian.cxx LIBRARIES ${ROOFITLIBRARIES})
ROOTTEST_ADD_TEST(testDestructRooGaussian
                  EXEC ./testDestructRooGaussian
                  DEPENDS ${GENERATE_EXECUTABLE_TEST})

ROOTTEST_GENERATE_EXECUTABLE(testRooSetMemPools testRooSetMemPools testRooSetMemPools.cxx LIBRARIES ${ROOFITLIBRARIES})
ROOTTEST_ADD_TEST(testRooSetMemPools
                  EXEC ./testRooSetMemPools
                  DEPENDS ${GENERATE_EXECUTABLE_TEST})
endif()
