#---Copy from source to binary some of the files
configure_file(HistArray.root . COPYONLY)

ROOTTEST_ADD_AUTOMACROS(EXCLUDE execMissing runTExMap runDeleteWarning)

ROOTTEST_ADD_TEST(arrayViewBasic
                  MACRO arrayViewBasic.C
                  OUTREF arrayViewBasic.ref)

ROOTTEST_ADD_TEST(execMissing
                  MACRO execMissing.C
                  OUTREF execMissing.oref
                  ERRREF execMissing.eref )

if (32BIT)
ROOTTEST_ADD_TEST(runTExMap
                  MACRO runTExMap.C
                  OUTREF runTExMap32.oref
                  ERRREF runTExMap32.eref )
else()
ROOTTEST_ADD_TEST(runTExMap
                  MACRO runTExMap.C
                  OUTREF runTExMap.oref
                  ERRREF runTExMap.eref )
endif()

# Disabled to avoid false positives.
# This test relies on a use-after-delete (on purpose) to check that the use-after-delete-detection
# logic in TList works properly. Since the kNotDeleted mechanism relies on UB, sometimes it does not work and this
# test segfaults instead.
#ROOTTEST_ADD_TEST(runDeleteWarning
#                  MACRO runDeleteWarning.C
#                  OUTCNVCMD sed -e s:0x[0-9a-fA-F]*:0xRemoved:g
#                  OUTREF DeleteWarning.ref)
