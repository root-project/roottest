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

#
# function EVENT_WRAP(name mode)
#
# Generates strings for use in macro interpretations
# which are done inside ROOTTEST_ADD_TEST.
#
macro(EVENT_WRAP name mode)
  set("${name}-file" "${name}.root")
  set("${name}-wrap" "${CMAKE_CURRENT_SOURCE_DIR}/dt_wrap.C\(\"${name}.root\",${mode}\)")
endmacro()

EVENT_WRAP(new-split0 0)
ROOTTEST_ADD_TEST(new-split0
  PRECMD  ./EventGeneration 50 0 0 1 600
  COMMAND ${ROOT_root_CMD} -q -b -l "${new-split0-wrap}"
  POSTCMD mv Event.root "${new-split0-file}"
  COPY_TO_BUILDDIR dt_wrap.C dt_RunDrawTest.C dt_DrawTest.C
  RUN_SERIAL)
