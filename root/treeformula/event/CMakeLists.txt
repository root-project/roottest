#-------------------------------------------------------------------------------
#
# Placeholder file to translate the tests to the new CTest system. Meanwhile we
# define a CTest test that calls 'make' in ${CMAKE_CURRENT_SOURCE_DIR}
#
#-------------------------------------------------------------------------------

ROOT_GENERATE_DICTIONARY(EventDict ./Event.h
  LINKDEF ./EventLinkDef.h)

ROOT_LINKER_LIBRARY(Event TEST ./Event.cxx EventDict.cxx
  LIBRARIES Core Tree Hist MathCore)

ROOTTEST_GENERATE_EXECUTABLE(EventGeneration ./EventMain.cxx
  LIBRARIES Core RIO Net Tree Hist MathCore Event)

if(TARGET onepcm)
  set(EventDependencies "onepcm")
endif()

ROOTTEST_ADD_TEST(read-write
  COMMAND ./EventGeneration 10 1 1 1 &&
          ./EventGeneration 10 1 1 20
  DEPENDS EventGeneration)

 # TODO add DrawTest
