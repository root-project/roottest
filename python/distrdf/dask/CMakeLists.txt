# Test only if:
# * `dask` and `distributed` are installed on that label for that python version
if (ROOT_test_distrdf_dask_FOUND)

ROOTTEST_ADD_TEST(dask_test_backend
                  MACRO test_backend.py
                  RUN_SERIAL)

ROOTTEST_ADD_TEST(dask_test_definepersample 
                  MACRO test_definepersample.py
                  RUN_SERIAL)

ROOTTEST_ADD_TEST(dask_test_friend_trees
                  MACRO test_friend_trees.py
                  RUN_SERIAL)

ROOTTEST_ADD_TEST(dask_test_histo_write 
                  MACRO test_histo_write.py
                  RUN_SERIAL)

ROOTTEST_ADD_TEST(dask_test_include_headers 
                  MACRO test_include_headers.py
                  RUN_SERIAL)

ROOTTEST_ADD_TEST(dask_test_inv_mass 
                  MACRO test_inv_mass.py
                  RUN_SERIAL)

ROOTTEST_ADD_TEST(dask_test_rungraphs
                  MACRO test_rungraphs.py
                  RUN_SERIAL)

ROOTTEST_ADD_TEST(dask_test_reducer_merge 
                  MACRO test_reducer_merge.py
                  RUN_SERIAL)

endif()
