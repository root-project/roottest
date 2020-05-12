
ROOTTEST_ADD_AUTOMACROS(DEPENDS namingMatches.cxx
                        EXCLUDE execCheckNaming)

if(MSVC)
    set(OUTREF_FILE execCheckNaming_win32.ref)
else()
    set(OUTREF_FILE execCheckNaming.ref)
endif()

ROOTTEST_ADD_TEST(execCheckNaming
                  MACRO execCheckNaming.C
                  OUTREF ${OUTREF_FILE})
