# The test assumes we have _rdict.pcm files which we do not generate for runtime
# cxxmodules.
if (NOT ROOT_runtime_cxxmodules_FOUND)
  ROOTTEST_ADD_AUTOMACROS(DEPENDS namingMatches.cxx)
endif()
