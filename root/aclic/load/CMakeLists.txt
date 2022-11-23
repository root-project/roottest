ROOTTEST_ADD_TEST(autoload
                  COPY_TO_BUILDDIR theRootmap.rootmap
                  MACRO execautoload.C
                  OUTREF execautoload.ref
                  DEPENDS autoloadtest.C)

if(NOT MSVC OR ${LLVM_VERSION} VERSION_LESS 13.0.0 OR llvm13_broken_tests)
    ROOTTEST_ADD_TEST(reload
                      COPY_TO_BUILDDIR Reload.root
                      MACRO assertReload.C
                      OUTREF assertReload.ref)
endif()

ROOTTEST_ADD_TEST(linktest
                  MACRO execlinktest.C
                  OUTREF execlinktest.ref)

if(NOT MSVC OR win_broken_tests)
    ROOTTEST_ADD_TEST(autoloadNamespace
                      MACRO execautoloadNamespace.C
                      OUTREF execautoloadNamespace.ref
                      DEPENDS MyClass.cxx autoload)
endif()
