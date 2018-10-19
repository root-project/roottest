if(ROOT_roofit_FOUND)
set(ROOFITLIBRARIES RooFitCore RooFit HistFactory)

ROOTTEST_ADD_TEST(read-scientificnotation
                  COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/read_scientific_notation.py
                  OUTCNVCMD grep -v -e "Wouter"
                  OUTREF read_scientific_notation.ref)

ROOTTEST_GENERATE_EXECUTABLE(test_destruct_RooGaussian testDestructRooGaussian testDestructRooGaussian.cxx LIBRARIES ${ROOFITLIBRARIES})

ROOTTEST_ADD_TEST(test_destruct_RooGaussian
                  EXEC ./test_destruct_RooGaussian )
endif()
