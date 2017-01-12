ROOTTEST_ADD_TEST(testSigBus
                  MACRO     testSigBus.C
                  PASSREGEX "bus error")

ROOTTEST_ADD_TEST(testSigSegV
                  MACRO     testSigSegV.C
                  PASSREGEX "segmentation violation")

ROOTTEST_ADD_TEST(testSigIll
                  MACRO     testSigIll.C
                  PASSREGEX "illegal instruction")

ROOTTEST_ADD_TEST(testSigFPE
                  MACRO     testSigFPE.C
                  PASSREGEX "floating exception")

ROOTTEST_ADD_TESTDIRS()
