if(NOT ROOT_pyroot_FOUND)
    return()
endif()

ROOTTEST_ADD_TEST(compression_settings
                  MACRO test_compression_settings.py)
ROOTTEST_ADD_TEST(input_validation
                  MACRO input_validation.py)
ROOTTEST_ADD_TEST(test_MergeChangeComp
                  COPY_TO_BUILDDIR merge_gen_input_tuples.C merge_changeComp_check_output.C
                  PRECMD ${ROOT_root_CMD} -q -b -l "merge_gen_input_tuples.C(\"test_comp_in1.root\", \"test_comp_in2.root\")"
                  COMMAND ${ROOT_hadd_CMD} -f404 test_comp_out.root test_comp_in1.root test_comp_in2.root
                  POSTCMD ${ROOT_root_CMD} -q -b -l "merge_changeComp_check_output.C(404, \"test_comp_out.root\", \"test_comp_in1.root\", \"test_comp_in2.root\")"
)

ROOTTEST_ADD_TEST(test_TreeChangeComp
                  COPY_TO_BUILDDIR hadd_gen_input_tree.C hadd_check_comp_tree.C
                  PRECMD ${ROOT_root_CMD} -q -b -l "hadd_gen_input_tree.C(\"test_comp_tree_in.root\")"
                  COMMAND ${ROOT_hadd_CMD} -f401 test_comp_tree_out.root test_comp_tree_in.root
                  POSTCMD ${ROOT_root_CMD} -q -b -l "hadd_check_comp_tree.C(\"test_comp_tree_out.root\", \"test_comp_tree_in.root\", 401)"
)

ROOTTEST_ADD_TEST(test_MergeNestedSameName
                  COPY_TO_BUILDDIR hadd_check_nested_same_name.C
                  COMMAND ${ROOT_root_CMD} -q -b -l "hadd_check_nested_same_name.C"
                  OUTREF hadd_check_nested_same_name.ref
)

ROOTTEST_ADD_TEST(test_TreeChangeComp
                  COPY_TO_BUILDDIR hadd_gen_input_tree.C hadd_check_comp_tree.C
                  PRECMD ${ROOT_root_CMD} -q -b -l "hadd_gen_input_tree.C(\"test_comp_tree_in.root\")"
                  COMMAND ${ROOT_hadd_CMD} -f401 test_comp_tree_out.root test_comp_tree_in.root
                  POSTCMD ${ROOT_root_CMD} -q -b -l "hadd_check_comp_tree.C(\"test_comp_tree_out.root\", \"test_comp_tree_in.root\", 401)"
)

ROOTTEST_ADD_TEST(test_hadd_args_n0
                  PRECMD ${ROOT_root_CMD} -q -b -l "hadd_gen_input_tree.C(\"hadd_args_n0.root\")"
                  COMMAND ${ROOT_hadd_CMD} -n0 hadd_args_n0_out.root hadd_args_n0.root
                  POSTCMD rm -f hadd_args_n0*.root
                  OUTREF hadd_args_n0.ref
)

ROOTTEST_ADD_TEST(test_hadd_args_na
                  PRECMD ${ROOT_root_CMD} -q -b -l "hadd_gen_input_tree.C(\"hadd_args_na.root\")"
                  COMMAND ${ROOT_hadd_CMD} -na hadd_args_na_out.root hadd_args_na.root
                  POSTCMD rm -f hadd_args_na*.root
                  PASSREGEX "Error in <hadd>: the argument after '-n' flag was not of the expected type."
)

ROOTTEST_ADD_TEST(test_hadd_args_n_1
                  PRECMD ${ROOT_root_CMD} -q -b -l "hadd_gen_input_tree.C(\"hadd_args_n_1.root\")"
                  COMMAND ${ROOT_hadd_CMD} -n=1 hadd_args_n_1_out.root hadd_args_n_1.root
                  POSTCMD rm -f hadd_args_n_1*.root
                  OUTREF hadd_args_n_1.ref
)

ROOTTEST_ADD_TEST(test_hadd_args_jfa
                  PRECMD ${ROOT_root_CMD} -q -b -l "hadd_gen_input_tree.C(\"hadd_args_jfa.root\")"
                  COMMAND ${ROOT_hadd_CMD} -jfa hadd_args_jfa_out.root hadd_args_jfa.root
                  POSTCMD rm -f hadd_args_jfa*.root
                  OUTREF hadd_args_jfa.ref
)

ROOTTEST_ADD_TEST(test_hadd_args_ff_dbg_j_2
                  PRECMD ${ROOT_root_CMD} -q -b -l "hadd_gen_input_tree.C(\"hadd_args_ff_dbg_j_2.root\")"
                  COMMAND ${ROOT_hadd_CMD} -ff hadd_args_ff_dbg_j_2_out.root hadd_args_ff_dbg_j_2.root hadd_args_ff_dbg_j_2.root -dbg -j 2
                  POSTCMD rm -f hadd_args_ff_dbg_j_2*.root
                  OUTREF hadd_args_ff_dbg_j_2.ref
)

ROOTTEST_ADD_TEST(test_hadd_args_minusminus
                  PRECMD ${ROOT_root_CMD} -q -b -l "hadd_gen_input_tree.C(\"--hadd_args.root\")"
                  COMMAND ${ROOT_hadd_CMD} -f0 -- --hadd_args_out.root --hadd_args.root
                  POSTCMD rm -f -- --hadd_args*.root
                  OUTREF hadd_args_minusminus.ref
)

ROOTTEST_ADD_TEST(test_hadd_args_minusminus2
                  PRECMD ${ROOT_root_CMD} -q -b -l "hadd_gen_input_tree.C(\"--hadd_args2.root\")"
                  COMMAND ${ROOT_hadd_CMD} -f --hadd_args2_out.root -- --hadd_args2.root
                  POSTCMD rm -f -- --hadd_args2*.root
                  PASSREGEX "Warning in <hadd>: unknown flag: --hadd_args2_out.root"
                  PASSRC 1
)

ROOTTEST_ADD_TEST(test_hadd_args_multiple_pos
                  PRECMD ${ROOT_root_CMD} -q -b -l "hadd_gen_input_tree.C(\"hadd_args_multiple_pos.root\")"
                  COMMAND ${ROOT_hadd_CMD} -ff hadd_args_multiple_pos_out.root -k hadd_args_multiple_pos.root
                  POSTCMD rm -f hadd_args_multiple_pos*.root
                  PASSREGEX "Error in <hadd>: seen a positional argument 'hadd_args_multiple_pos.root' after some flags."
)

ROOTTEST_ADD_TEST(test_hadd_args_cachesize
                  PRECMD ${ROOT_root_CMD} -q -b -l "hadd_gen_input_tree.C(\"hadd_args_cachesize.root\")"
                  COMMAND ${ROOT_hadd_CMD} hadd_args_cachesize_out.root hadd_args_cachesize.root -cachesize 100M -f
                  POSTCMD rm -f hadd_args_cachesize*.root
                  OUTREF hadd_args_cachesize.ref
)

ROOTTEST_ADD_TEST(test_hadd_args_missing_arg
                  PRECMD ${ROOT_root_CMD} -q -b -l "hadd_gen_input_tree.C(\"hadd_args_missing_arg.root\")"
                  COMMAND ${ROOT_hadd_CMD} -n hadd_args_missing_arg_out.root hadd_args_missing_arg.root
                  POSTCMD rm -f hadd_args_missing_arg*.root
                  PASSREGEX "Error in <hadd>: the argument after '-n' flag was not of the expected type"
)

ROOTTEST_ADD_TEST(test_hadd_args_missing_arg2
                  PRECMD ${ROOT_root_CMD} -q -b -l "hadd_gen_input_tree.C(\"hadd_args_missing_arg2.root\")"
                  COMMAND ${ROOT_hadd_CMD} hadd_args_missing_arg2_out.root hadd_args_missing_arg2.root -n
                  POSTCMD rm -f hadd_args_missing_arg2*.root
                  PASSREGEX "Error in <hadd>: expected argument after '-n' flag."
)

