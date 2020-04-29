ROOTTEST_ADD_TEST(autoload
                  COPY_TO_BUILDDIR theRootmap.rootmap
                  MACRO execautoload.C
                  OUTREF execautoload.ref
                  DEPENDS autoloadtest.C)

ROOTTEST_ADD_TEST(reload
                  COPY_TO_BUILDDIR Reload.root
                  MACRO assertReload.C
                  OUTREF assertReload.ref)

ROOTTEST_ADD_TEST(linktest
                  MACRO execlinktest.C
                  OUTREF execlinktest.ref)

if(MSVC)
    set(MIGHT_FAIL WILLFAIL)
endif()

ROOTTEST_ADD_TEST(autoloadNamespace
                  MACRO execautoloadNamespace.C
                  OUTREF execautoloadNamespace.ref
                  ${MIGHT_FAIL}
                  DEPENDS MyClass.cxx autoload)
