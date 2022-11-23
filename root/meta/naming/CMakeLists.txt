if(NOT MSVC OR ${LLVM_VERSION} VERSION_LESS 13.0.0 OR llvm13_broken_tests)
  ROOTTEST_ADD_AUTOMACROS(DEPENDS namingMatches.cxx
                          EXCLUDE execCheckNaming)

  ROOTTEST_ADD_TEST(execCheckNaming
                    MACRO execCheckNaming.C
                    OUTREF execCheckNaming${ref_suffix})
endif()
