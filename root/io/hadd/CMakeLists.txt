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

# ------------------------------------
# arg parsing tests
# ------------------------------------

ROOTTEST_ADD_TEST(test_hadd_args_gen_input
                  COPY_TO_BUILDDIR hadd_gen_input_tree.C
                  PRECMD ${CMAKE_COMMAND} -E rm -f hadd_args_in.root
                  COMMAND ${ROOT_root_CMD} -q -b -l "hadd_gen_input_tree.C(\"hadd_args_in.root\")"
)

ROOTTEST_ADD_TEST(test_hadd_args_n0
                  DEPENDS test_hadd_args_gen_input
                  COMMAND ${ROOT_hadd_CMD} -n0 hadd_args_n0_out.root hadd_args_in.root
                  POSTCMD ${CMAKE_COMMAND} -E rm -f hadd_args_n0_out.root
                  OUTREF hadd_args_n0.ref
)

ROOTTEST_ADD_TEST(test_hadd_args_na
                  DEPENDS test_hadd_args_gen_input
                  COMMAND ${ROOT_hadd_CMD} -na hadd_args_na_out.root hadd_args_in.root
                  POSTCMD ${CMAKE_COMMAND} -E rm -f hadd_args_na_out.root
                  PASSREGEX "Error in <hadd>: the argument after '-n' flag was not of the expected type."
)

ROOTTEST_ADD_TEST(test_hadd_args_n_1
                  DEPENDS test_hadd_args_gen_input
                  COMMAND ${ROOT_hadd_CMD} -n=1 hadd_args_n_1_out.root hadd_args_in.root
                  POSTCMD ${CMAKE_COMMAND} -E rm -f hadd_args_n_1_out.root
                  OUTREF hadd_args_n_1.ref
)

ROOTTEST_ADD_TEST(test_hadd_args_jfa
                  DEPENDS test_hadd_args_gen_input
                  COMMAND ${ROOT_hadd_CMD} -jfa hadd_args_jfa_out.root hadd_args_in.root
                  POSTCMD ${CMAKE_COMMAND} -E rm -f hadd_args_jfa_out.root
                  PASSREGEX "Error in <hadd>: the argument after '-j' flag was not of the expected type"
)

ROOTTEST_ADD_TEST(test_hadd_args_j_10a
                  DEPENDS test_hadd_args_gen_input
                  COMMAND ${ROOT_hadd_CMD} -j 10a hadd_args_j_10a_out.root hadd_args_in.root
                  POSTCMD ${CMAKE_COMMAND} -E rm -f hadd_args_j_10a_out.root
                  PASSREGEX "Error in <hadd>: error parsing integer argument '10a'"
)

ROOTTEST_ADD_TEST(test_hadd_args_j
                  DEPENDS test_hadd_args_gen_input
                  COMMAND ${ROOT_hadd_CMD} -j hadd_args_j_out.root hadd_args_in.root
                  POSTCMD ${CMAKE_COMMAND} -E rm -f hadd_args_j_out.root
                  PASSRC 0
)

ROOTTEST_ADD_TEST(test_hadd_args_ff_dbg_j_2
                  DEPENDS test_hadd_args_gen_input
                  COMMAND ${ROOT_hadd_CMD} -ff hadd_args_ff_dbg_j_2_out.root hadd_args_in.root hadd_args_in.root -dbg -j 2
                  POSTCMD ${CMAKE_COMMAND} -E rm -f hadd_args_ff_dbg_j_2_out.root
                  OUTREF hadd_args_ff_dbg_j_2.ref
)

ROOTTEST_ADD_TEST(test_hadd_args_minusminus
                  DEPENDS test_hadd_args_gen_input
                  COMMAND ${ROOT_hadd_CMD} -f0 -- --hadd_args_out.root hadd_args_in.root
                  POSTCMD ${CMAKE_COMMAND} -E rm -f -- --hadd_args_out.root
                  OUTREF hadd_args_minusminus.ref
)

ROOTTEST_ADD_TEST(test_hadd_args_minusminus2
                  DEPENDS test_hadd_args_gen_input
                  COMMAND ${ROOT_hadd_CMD} -f --hadd_args2_out.root -- hadd_args_in.root
                  POSTCMD ${CMAKE_COMMAND} -E rm -f -- --hadd_args2_out.root
                  PASSREGEX "Warning in <hadd>: unknown flag: --hadd_args2_out.root"
                  PASSRC 1
)

ROOTTEST_ADD_TEST(test_hadd_args_multiple_pos
                  DEPENDS test_hadd_args_gen_input
                  COMMAND ${ROOT_hadd_CMD} -ff hadd_args_multiple_pos_out.root -k hadd_args_in.root
                  POSTCMD ${CMAKE_COMMAND} -E rm -f hadd_args_multiple_pos_out.root
                  PASSREGEX "Error in <hadd>: seen a positional argument 'hadd_args_in.root' after some flags."
)

ROOTTEST_ADD_TEST(test_hadd_args_cachesize
                  DEPENDS test_hadd_args_gen_input
                  COMMAND ${ROOT_hadd_CMD} hadd_args_cachesize_out.root hadd_args_in.root -cachesize 100M -f
                  POSTCMD ${CMAKE_COMMAND} -E rm -f hadd_args_cachesize_out.root
                  OUTREF hadd_args_cachesize.ref
)

ROOTTEST_ADD_TEST(test_hadd_args_missing_arg
                  DEPENDS test_hadd_args_gen_input
                  COMMAND ${ROOT_hadd_CMD} -n hadd_args_missing_arg_out.root hadd_args_in.root
                  POSTCMD ${CMAKE_COMMAND} -E rm -f hadd_args_missing_arg_out.root
                  PASSREGEX "Error in <hadd>: the argument after '-n' flag was not of the expected type"
)

ROOTTEST_ADD_TEST(test_hadd_args_missing_arg2
                  DEPENDS test_hadd_args_gen_input
                  COMMAND ${ROOT_hadd_CMD} hadd_args_missing_arg2_out.root hadd_args_in.root -n
                  POSTCMD ${CMAKE_COMMAND} -E rm -f hadd_args_missing_arg2_out.root
                  PASSREGEX "Error in <hadd>: expected argument after '-n' flag."
)
