
if((compression_default STREQUAL "lz4") AND (NOT builtin_zlib))
            ROOTTEST_ADD_TEST(testForeignDraw
                  MACRO Run.C
                  PRECMD ${ROOT_root_CMD} -b -q -l ${CMAKE_CURRENT_SOURCE_DIR}/def.C+
                  OUTREF testForeignDrawLZ4.ref
                  DEPENDS test-dataset)
elseif((compression_default STREQUAL "lz4") AND (builtin_zlib))
	    ROOTTEST_ADD_TEST(testForeignDraw
                  MACRO Run.C
                  PRECMD ${ROOT_root_CMD} -b -q -l ${CMAKE_CURRENT_SOURCE_DIR}/def.C+
		  OUTREF testForeignDrawZLIBCF.ref
                  DEPENDS test-dataset)
else()
            ROOTTEST_ADD_TEST(testForeignDraw
                  MACRO Run.C
                  PRECMD ${ROOT_root_CMD} -b -q -l ${CMAKE_CURRENT_SOURCE_DIR}/def.C+
                  OUTREF testForeignDrawZLIB.ref
                  DEPENDS test-dataset)
endif()
