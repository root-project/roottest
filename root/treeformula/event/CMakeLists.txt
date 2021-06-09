ROOT_GENERATE_DICTIONARY(EventDict ./Event.h
  LINKDEF ./EventLinkDef.h)

ROOT_LINKER_LIBRARY(Event TEST ./Event.cxx EventDict.cxx
  LIBRARIES Core Tree Hist MathCore)

ROOTTEST_GENERATE_EXECUTABLE(EventGeneration ./EventMain.cxx
  LIBRARIES Core RIO Net Tree Hist MathCore Event)

if(TARGET onepcm)
  set(EventDependencies "onepcm")
endif()

ROOTTEST_ADD_TEST(write
  COMMAND ./EventGeneration 10 1 1 1
  DEPENDS EventGeneration ${EventDependencies}
  RUN_SERIAL)

ROOTTEST_ADD_TEST(read
  COMMAND ./EventGeneration 10 1 1 20
  DEPENDS EventGeneration ${EventDependencies}
  RUN_SERIAL)

# TODO add DrawTest
