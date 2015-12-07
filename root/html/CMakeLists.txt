ROOTTEST_ADD_TEST(roottest-root-html-runClassInModule
                  MACRO runClassInModule.C
                  WILLFAIL
                  OUTREF ClassInModule.ref)

ROOTTEST_ADD_TEST(roottest-root-html-runFindSource
                  MACRO runFindSource.C
                  WILLFAIL
                  OUTREF FindSource.ref)
                  
if(ROOTTEST_OS_ID MATCHES Scientific OR ROOTTEST_OS_ID MATCHES CentOS)
  set(ROOTTEST_ENV_EXTRA LD_PRELOAD=/usr/lib64/libGL.so)
endif()

ROOTTEST_ADD_TEST(roottest-root-html-runMakeIndex
                  MACRO runMakeIndex.C
                  OUTCNV MakeIndex_convert.sh
                  OUTREF MakeIndex.oref
                  ERRREF MakeIndex.eref)
