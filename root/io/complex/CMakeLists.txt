ROOTTEST_ADD_TESTDIRS()

ROOTTEST_GENERATE_REFLEX_DICTIONARY( classWithComplex classWithComplex.h  SELECTION classWithComplex_selection.xml)

if(MSVC)
    set(MIGHT_FAIL WILLFAIL)
endif()

ROOTTEST_ADD_TEST(writeClassWithComplex
                  MACRO  execwriteClassWithComplex.C
                  OUTREF execwriteClassWithComplex.ref
                  ${MIGHT_FAIL}
                  DEPENDS ${GENERATE_REFLEX_TEST})

ROOTTEST_ADD_TEST(readClassWithComplex
                  MACRO  execreadClassWithComplex.C
                  OUTREF execreadClassWithComplex.ref
                  ${MIGHT_FAIL}
                  DEPENDS writeClassWithComplex)


ROOTTEST_ADD_TEST(writecomplex
                  MACRO  execwritecomplex.C
                  OUTREF execwritecomplex.ref
                  ${MIGHT_FAIL} )

ROOTTEST_ADD_TEST(readcomplex
                  COPY_TO_BUILDDIR complexOfilekubuntuROOT5.root  complexOfilekubuntuROOT6.root complexOfileslc6ROOT5.xml complexOfilekubuntuROOT5.xml   complexOfilekubuntuROOT6.xml   complexOfileslc6ROOT5.root
                  MACRO  execreadcomplex.C
                  OUTREF execreadcomplex.ref
                  DEPENDS writecomplex
                  ${MIGHT_FAIL} )
