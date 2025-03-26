if(NOT ROOT_test_distrdf_dask_FOUND AND NOT ROOT_test_distrdf_pyspark_FOUND)
    return()
endif()

if(ROOT_test_distrdf_dask_FOUND)
    list(APPEND DISTRDF_BACKENDS_IN_USE "dask")
    list(APPEND DISTRDF_RESOURCE_LOCKS "dask_resource_lock")
    #list(APPEND DISTRDF_FIXTURES "dask_cluster")
endif()

if(ROOT_test_distrdf_pyspark_FOUND)

    # Define environment variables needed in all pyspark tests
    set(PYSPARK_ENV_VARS PYSPARK_PYTHON=${Python3_EXECUTABLE})

    if(MACOSX_VERSION VERSION_GREATER_EQUAL 10.13)
        # MacOS has changed rules about forking processes after 10.13
        # Running pyspark tests with XCode Python3 throws crashes with errors like:
        # `objc[17271]: +[__NSCFConstantString initialize] may have been in progress in another thread when fork() was called.`
        # This issue should have been fixed after Python 3.8 (see https://bugs.python.org/issue33725)
        # Indeed, any other Python 3.8+ executable does not show this crash. It is
        # specifically the XCode Python executable that triggers this.
        # For now, there seems no other way than this workaround,
        # which effectively sets the behaviour of `fork` back to MacOS 10.12
        list(APPEND PYSPARK_ENV_VARS OBJC_DISABLE_INITIALIZE_FORK_SAFETY=YES)
    endif()

    list(APPEND DISTRDF_ENVIRONMENT_VARS ${PYSPARK_ENV_VARS})
    list(APPEND DISTRDF_BACKENDS_IN_USE "spark")
    list(APPEND DISTRDF_RESOURCE_LOCKS "spark_resource_lock")

    find_program(SPARK_CLASS_CMD NAMES "spark-class")
    if (SPARK_CLASS_CMD-NOTFOUND)
        message(FATAL "Cannot find spark-class.")
    else()
        message(WARNING "Found ${SPARK_CLASS_CMD}")
    endif()

    ROOTTEST_ADD_TEST(start_spark_master
                  EXEC ${CMAKE_CURRENT_SOURCE_DIR}/scripts/start_spark_master.sh
                  OPTS ${SPARK_CLASS_CMD}
                  ENVIRONMENT "${PYSPARK_ENV_VARS}"
                  FIXTURES_SETUP spark_master)
    ROOTTEST_ADD_TEST(stop_spark_master
                  EXEC ${CMAKE_CURRENT_SOURCE_DIR}/scripts/kill_pid.sh
                  OPTS ${CMAKE_CURRENT_BINARY_DIR}/start_spark_master.log
                  FIXTURES_CLEANUP spark_master)

    ROOTTEST_ADD_TEST(start_spark_worker
                  EXEC ${CMAKE_CURRENT_SOURCE_DIR}/scripts/start_spark_worker.sh
                  OPTS ${SPARK_CLASS_CMD} ${CMAKE_CURRENT_BINARY_DIR}
                  ENVIRONMENT "${PYSPARK_ENV_VARS}"
                  FIXTURES_REQUIRED spark_master
                  FIXTURES_SETUP spark_worker)
    ROOTTEST_ADD_TEST(stop_spark_worker
                  EXEC ${CMAKE_CURRENT_SOURCE_DIR}/scripts/kill_pid.sh
                  OPTS ${CMAKE_CURRENT_BINARY_DIR}/start_spark_worker.log
                  FIXTURES_REQUIRED spark_master
                  FIXTURES_CLEANUP spark_worker)

    list(APPEND DISTRDF_FIXTURES "spark_worker")
endif()


# Signal which backends are in use to the test environment
# Change semicolon to comma here to avoid breaking ENVIRONMENT argument later on
string(REPLACE ";" "," DISTRDF_BACKENDS_IN_USE "${DISTRDF_BACKENDS_IN_USE}")
list(APPEND DISTRDF_ENVIRONMENT_VARS DISTRDF_BACKENDS_IN_USE=${DISTRDF_BACKENDS_IN_USE})

# We use the PROCESSORS property to tell cmake how many cores we will be
# using in the following test. In this folder, mock cluster connections will be
# created, each using 2 cores with multiprocessing, we give it more room by
# setting the property to 4. The test also locks a resource for the creation of
# the clusters, depending on how many backends are active. The resource lock
# is shared with the "common" folder and the tutorials of the main repository.
message(WARNING ${DISTRDF_FIXTURES})
ROOTTEST_ADD_TEST(test_all
                    MACRO test_all.py
                    ENVIRONMENT "${DISTRDF_ENVIRONMENT_VARS}"
                    TIMEOUT 1200
                    PROPERTIES PROCESSORS 4
                    FIXTURES_REQUIRED ${DISTRDF_FIXTURES})

# This test has to take multiple resource locks. This means that they should
# be passed as a cmake list (semi-colon separated strings) after the
# RESOURCE_LOCK argument to `set_tests_properties`. The issue lies in
# forwarding this argument to ROOTTEST_ADD_TEST and then ROOT_ADD_TEST.
# Argument forwarding and list parsing in cmake is utterly broken.
# The safest and most reliable thing to do is to call the final cmake
# function directly here, so we can be sure that the PROPERTIES argument
# will be properly parsed.
set_tests_properties(roottest-python-distrdf-backends-test_all PROPERTIES RESOURCE_LOCK "${DISTRDF_RESOURCE_LOCKS}")

ROOTTEST_ADD_TEST(test_distrdf_backend_mytestflag
                    MACRO check_backend.py
                    ENVIRONMENT "${DISTRDF_ENVIRONMENT_VARS}"
                    TIMEOUT 1200
                    PROPERTIES RESOURCE_LOCK spark_resource_lock
                    FIXTURES_REQUIRED ${DISTRDF_FIXTURES})

ROOTTEST_ADD_TEST(test_distrdf_cloned_actions_mytestflag
                    MACRO check_cloned_actions.py
                    ENVIRONMENT "${DISTRDF_ENVIRONMENT_VARS}"
                    TIMEOUT 1200
                    PROPERTIES RESOURCE_LOCK spark_resource_lock
                    FIXTURES_REQUIRED ${DISTRDF_FIXTURES})

ROOTTEST_ADD_TEST(test_distrdf_distribute_cppcode_mytestflag
                    MACRO check_distribute_cppcode.py
                    ENVIRONMENT "${DISTRDF_ENVIRONMENT_VARS}"
                    TIMEOUT 1200
                    PROPERTIES RESOURCE_LOCK spark_resource_lock
                    FIXTURES_REQUIRED ${DISTRDF_FIXTURES})

ROOTTEST_ADD_TEST(test_distrdf_distribute_headers_sharedlibs_files_mytestflag
                    MACRO check_distribute_headers_sharedlibs_files.py
                    ENVIRONMENT "${DISTRDF_ENVIRONMENT_VARS}"
                    TIMEOUT 1200
                    PROPERTIES RESOURCE_LOCK spark_resource_lock
                    FIXTURES_REQUIRED ${DISTRDF_FIXTURES})


ROOTTEST_ADD_TEST(test_distrdf_explicit_api_mytestflag
                    MACRO check_explicit_api.py
                    ENVIRONMENT "${DISTRDF_ENVIRONMENT_VARS}"
                    TIMEOUT 1200
                    PROPERTIES RESOURCE_LOCK spark_resource_lock
                    FIXTURES_REQUIRED ${DISTRDF_FIXTURES})

ROOTTEST_ADD_TEST(test_distrdf_friend_trees_alignment_mytestflag
                    MACRO check_friend_trees_alignment.py
                    ENVIRONMENT "${DISTRDF_ENVIRONMENT_VARS}"
                    TIMEOUT 1200
                    PROPERTIES RESOURCE_LOCK spark_resource_lock
                    FIXTURES_REQUIRED ${DISTRDF_FIXTURES})

ROOTTEST_ADD_TEST(test_distrdf_friend_trees_mytestflag
                    MACRO check_friend_trees.py
                    ENVIRONMENT "${DISTRDF_ENVIRONMENT_VARS}"
                    TIMEOUT 1200
                    PROPERTIES RESOURCE_LOCK spark_resource_lock
                    FIXTURES_REQUIRED ${DISTRDF_FIXTURES})


ROOTTEST_ADD_TEST(test_distrdf_histo_write_mytestflag
                MACRO check_histo_write.py
                    ENVIRONMENT "${DISTRDF_ENVIRONMENT_VARS}"
                    TIMEOUT 1200
                    PROPERTIES RESOURCE_LOCK spark_resource_lock
                    FIXTURES_REQUIRED ${DISTRDF_FIXTURES})


ROOTTEST_ADD_TEST(test_distrdf_inv_mass_mytestflag
                    MACRO check_inv_mass.py
                        ENVIRONMENT "${DISTRDF_ENVIRONMENT_VARS}"
                        TIMEOUT 1200
                        PROPERTIES RESOURCE_LOCK spark_resource_lock
                        FIXTURES_REQUIRED ${DISTRDF_FIXTURES})


ROOTTEST_ADD_TEST(test_distrdf_live_visualize_mytestflag
                MACRO check_live_visualize.py
                    ENVIRONMENT "${DISTRDF_ENVIRONMENT_VARS}"
                            TIMEOUT 1200
                            PROPERTIES RESOURCE_LOCK spark_resource_lock
                            FIXTURES_REQUIRED ${DISTRDF_FIXTURES})

ROOTTEST_ADD_TEST(test_distrdf_missing_values_mytestflag
                            MACRO check_missing_values.py
                                ENVIRONMENT "${DISTRDF_ENVIRONMENT_VARS}"
                                        TIMEOUT 1200
                                        PROPERTIES RESOURCE_LOCK spark_resource_lock
                                        FIXTURES_REQUIRED ${DISTRDF_FIXTURES})

ROOTTEST_ADD_TEST(test_distrdf_reducer_merge_mytestflag
                MACRO check_reducer_merge.py
                    ENVIRONMENT "${DISTRDF_ENVIRONMENT_VARS}"
                            TIMEOUT 1200
                            PROPERTIES RESOURCE_LOCK spark_resource_lock
                            FIXTURES_REQUIRED ${DISTRDF_FIXTURES})

ROOTTEST_ADD_TEST(test_distrdf_rungraphs_mytestflag
                            MACRO check_rungraphs.py
                            ENVIRONMENT "${DISTRDF_ENVIRONMENT_VARS}"
                                        TIMEOUT 1200
                                        PROPERTIES RESOURCE_LOCK spark_resource_lock
                                        FIXTURES_REQUIRED ${DISTRDF_FIXTURES})

ROOTTEST_ADD_TEST(test_distrdf_variations_mytestflag
                    MACRO check_variations.py
                    ENVIRONMENT "${DISTRDF_ENVIRONMENT_VARS}"
                                TIMEOUT 1200
                                PROPERTIES RESOURCE_LOCK spark_resource_lock
                                FIXTURES_REQUIRED ${DISTRDF_FIXTURES})