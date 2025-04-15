ROOTTEST_GENERATE_DICTIONARY(
  event_v2_dict
  ${CMAKE_CURRENT_SOURCE_DIR}/Event_v2.hxx
  LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/Event_v2_LinkDef.h
  NO_ROOTMAP NO_CXXMODULE
  FIXTURES_SETUP generated_event_v2_dictionary
)

ROOTTEST_GENERATE_EXECUTABLE(
  write_autoptr
  LIBRARIES Core RIO Tree
  FIXTURES_REQUIRED generated_event_v2_dictionary
  FIXTURES_SETUP write_autoptr_excutable)

target_sources(
  write_autoptr
  PRIVATE write_autoptr.cxx event_v2_dict.cxx
)

ROOTTEST_ADD_TEST(write_autoptr
                  EXEC ./write_autoptr
                  FIXTURES_REQUIRED write_autoptr_excutable
                  FIXTURES_SETUP written_autoptr)

ROOTTEST_GENERATE_DICTIONARY(
  event_v3_dict
  ${CMAKE_CURRENT_SOURCE_DIR}/Event_v3.hxx
  LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/Event_v3_LinkDef.h
  NO_ROOTMAP NO_CXXMODULE
  FIXTURES_SETUP generated_event_v3_dictionary
)

ROOTTEST_GENERATE_EXECUTABLE(
  evolve_autoptr
  LIBRARIES Core RIO Tree
  FIXTURES_REQUIRED generated_event_v3_dictionary
  FIXTURES_SETUP evolve_autoptr_executable)

target_sources(
  evolve_autoptr
  PRIVATE evolve_autoptr.cxx event_v3_dict.cxx
)

ROOTTEST_ADD_TEST(evolve_autoptr
                  EXEC ./evolve_autoptr
                  FIXTURES_REQUIRED evolve_autoptr_executable written_autoptr)
