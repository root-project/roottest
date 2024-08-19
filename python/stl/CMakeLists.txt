# Disabled for ROOT 6.26, 6.28, and 6.30, because it sporadically failed before
# the cppyy upgrade in ROOT 6.32.
#
# if(ROOT_pyroot_FOUND)
#   if(NOT MSVC OR win_broken_tests)
#     ROOTTEST_ADD_TEST(stl
#                       MACRO PyROOT_stltests.py
#                       COPY_TO_BUILDDIR StlTypes.C
#                       PRECMD ${ROOT_root_CMD} -b -q -l -e .L\ StlTypes.C+
#                       ENVIRONMENT LEGACY_PYROOT=${legacy_pyroot})
#   endif()
# endif()

