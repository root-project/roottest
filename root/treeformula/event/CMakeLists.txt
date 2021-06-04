ROOT_GENERATE_DICTIONARY(EventDict ./Event.h
  LINKDEF ./EventLinkDef.h)

ROOT_LINKER_LIBRARY(Event TEST ./Event.cxx EventDict.cxx
  LIBRARIES Core Tree Hist MathCore)

ROOT_LINKER_LIBRARY(EventGeneration TEST ./EventEntryPoint.cxx
  LIBRARIES Core RIO Net Tree Hist MathCore Event)

if(TARGET onepcm)
  set(EventDependencies "onepcm")
endif()

ROOTTEST_COMPILE_MACRO(write.cxx
  DEPENDS EventGeneration ${EventDependencies})

ROOTTEST_COMPILE_MACRO(read.cxx
  DEPENDS EventGeneration ${EventDependencies})

 # TODO add DrawTest
