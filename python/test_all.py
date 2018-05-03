#! /usr/bin/env python
"""
Test runner interface
---------------------

This is the pytest (see http://pytest.org) based test runner taken from
PyPy. This way, PyROOT and PyPy/cppyy can share the same set of tests.

For more information, use test_all.py -h.
"""

import os, sys

import pytest
import pytest_cov

if __name__ == '__main__':
    if len(sys.argv) == 1 and os.path.dirname(sys.argv[0]) in '.':
        print >> sys.stderr, __doc__
        sys.exit(2)
    sys.exit(pytest.main(plugins=[pytest_cov]))
