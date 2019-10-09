if(EXISTS ${FileToBeChecked})
# For C++ modules builds, module.modulemap is generated during configuration time.
  ROOTTEST_ADD_TEST(examples
                 COPY_TO_BUILDDIR stl_makeproject_test.h
                 MACRO create_makeproject_examples.C
                 POSTCMD ${CMAKE_COMMAND} -E rename ${CMAKE_CURRENT_BINARY_DIR}/module.modulemap ${CMAKE_CURRENT_BINARY_DIR}/disabled.module.modulemap
                 OUTREF create_makeproject_examples.ref
                 DEPENDS ${GENERATE_DICTIONARY_TEST} stl_makeproject_test-build)
else()
    if(EXISTS ${DisabledFileToBeChecked})
    # For C++ modules incremental builds.
        ROOTTEST_ADD_TEST(examples
                COPY_TO_BUILDDIR stl_makeproject_test.h
                PRECMD ${CMAKE_COMMAND} -E rename ${CMAKE_CURRENT_BINARY_DIR}/disabled.module.modulemap ${CMAKE_CURRENT_BINARY_DIR}/module.modulemap
                MACRO create_makeproject_examples.C
                POSTCMD ${CMAKE_COMMAND} -E rename ${CMAKE_CURRENT_BINARY_DIR}/module.modulemap ${CMAKE_CURRENT_BINARY_DIR}/disabled.module.modulemap
                OUTREF create_makeproject_examples.ref
                DEPENDS ${GENERATE_DICTIONARY_TEST} stl_makeproject_test-build)
    endif()
endif()