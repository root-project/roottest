if(ROOT_pyroot_FOUND)
    ROOTTEST_ADD_TEST(compression_settings
                      MACRO test_compression_settings.py)

    ROOTTEST_ADD_TEST(input_validation
                      MACRO input_validation.py)
endif()

ROOTTEST_ADD_TEST(test_TreeChangeComp
                  COPY_TO_BUILDDIR hadd_gen_input_tree.C hadd_check_comp_tree.C
                  PRECMD ${ROOT_root_CMD} -q -b -l "hadd_gen_input_tree.C(\"test_comp_tree_in.root\")"
                  COMMAND ${ROOT_hadd_CMD} -f401 test_comp_tree_out.root test_comp_tree_in.root
                  POSTCMD ${ROOT_root_CMD} -q -b -l "hadd_check_comp_tree.C(\"test_comp_tree_out.root\", \"test_comp_tree_in.root\", 401)"
)
