if(TARGET onepcm)
  set(EventDependencies "onepcm")
endif()

# Populate a string with the path to the folder where the generation of the
# Event library together with all the input files happened
get_filename_component(PARENT_DIR ${CMAKE_CURRENT_BINARY_DIR} DIRECTORY)
set(EVENT_DIR ${PARENT_DIR}/event)

#
# function FASTCLONINGEVENTTREE_GENERATE_TEST
#
# Generate ctrans.C tests using file generated with FASTCLONINGEVENTTREE_GENERATE_FILE
#
function(FASTCLONINGEVENTTREE_GENERATE_TEST file_name)
      ROOTTEST_ADD_TEST("ctrans-${file_name}"
            COMMAND ${ROOT_root_CMD} -q -b -l "${CMAKE_CURRENT_SOURCE_DIR}/ctrans.C\(\"${EVENT_DIR}/${file_name}.root\"\)"
            DEPENDS EventGeneration
            FIXTURES_REQUIRED EventGeneration-${file_name})
endfunction()

FASTCLONINGEVENTTREE_GENERATE_TEST(split-zero)
FASTCLONINGEVENTTREE_GENERATE_TEST(split-one)
FASTCLONINGEVENTTREE_GENERATE_TEST(split-two)
FASTCLONINGEVENTTREE_GENERATE_TEST(split-nine)
FASTCLONINGEVENTTREE_GENERATE_TEST(stream-old)

ROOTTEST_ADD_TEST(makeref-split-nine
            COMMAND ${ROOT_root_CMD} -q -b -l "${CMAKE_CURRENT_SOURCE_DIR}/dt_MakeRef.C\(\"${EVENT_DIR}/split-nine.root\"\)" > log
            DEPENDS EventGeneration
            FIXTURES_REQUIRED EventGeneration-split-nine)
