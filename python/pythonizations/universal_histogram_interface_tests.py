# Author: Lukas Breitwieser, CERN, 11/2024

"""Universal histogram interface (UHI) tests."""

import os, sys, pytest
sys.path.append(os.path.dirname(os.path.dirname(__file__)))

from common import *
ROOT = pytest.importorskip("ROOT")

def test_root_th1_indexing() -> None:
    h = ROOT.TH1F("h1", "h1", 12, -2.5, 2.5)

    # standard bracket operator
    h[2] = 2
    assert h[2] == 2.0

    # underflow
    h[ROOT.underflow] = 3
    assert h[ROOT.underflow] == 3.0

    # overflow
    h[ROOT.overflow] = 4
    assert h[ROOT.overflow] == 4.0

    # loc 
    h.SetBinContent(h.FindBin(1.4), 5.0)
    assert h[ROOT.loc(1.4)] == 5.0
    h[ROOT.loc(-1.4)] = 6.0
    assert h.GetBinContent(h.FindBin(-1.4)) == 6.0

    # loc with addition 
    h.SetBinContent(h.FindBin(1.4) + 1, 7.0)
    assert h[ROOT.loc(1.4) + 1] == 7.0
    h[ROOT.loc(-1.4) + 1] = 8.0
    assert h.GetBinContent(h.FindBin(-1.4) + 1) == 8.0

    # loc with subraction 
    h.SetBinContent(h.FindBin(1.4) - 1, 9.0)
    assert h[ROOT.loc(1.4) - 1] == 9.0
    h[ROOT.loc(-1.4) - 1] = 10.0
    assert h.GetBinContent(h.FindBin(-1.4) - 1) == 10.0

    # wrong type read access
    with pytest.raises(TypeError):
        h[1.0]

    # wrong type write access
    with pytest.raises(TypeError):
        h[1.0] = 0.0
    
def test_root_th1_slicing() -> None:
    h = ROOT.TH1F("h1", "h1", 12, -2.5, 2.5)
    h1 = h[:]
    assert h == h1

    with pytest.raises(NotImplementedError):
        h1 = h[0:4]

def test_root_th1_slicing_with_step() -> None:
    h = ROOT.TH1F("h1", "h1", 12, -2.5, 2.5)
    h1 = h[:]
    assert h == h1

    # slicing with a step is not supported in UHI v0.5.1
    with pytest.raises(TypeError):
        h1 = h[::2]

def test_root_th1_rebin_inplace() -> None:
    h = ROOT.TH1F("h1", "h1", 12, -2.5, 2.5)
    h.FillRandom("gaus", 10000)
    hclone = h.Clone()
    h1 = h[::ROOT.rebin(2)]
    href = hclone.Rebin(2)
    # we assume that calling ROOT.rebin, without specifying a new name modifies
    # h instead of creating a new histogram
    assert h is h1 
    assert h1.GetNcells() == href.GetNcells()
    assert all(
        h1.GetBinContent(i) == href.GetBinContent(i)
        for i in range(h1.GetNcells()))

def test_root_th1_rebin_copy() -> None:
    h = ROOT.TH1F("h1", "h1", 12, -2.5, 2.5)
    h.FillRandom("gaus", 10000)

    h1 = h[::ROOT.rebin(2, "h1_rebinned_2")]
    assert h is not h1
    href = h.Rebin(2)
    assert h1.GetNcells() == href.GetNcells()
    assert all(
        h1.GetBinContent(i) == href.GetBinContent(i)
        for i in range(h1.GetNcells()))

def test_root_th1_sum() -> None:
    h = ROOT.TH1F("h1", "h1", 12, -2.5, 2.5)
    print(h.GetNcells())
    ref = 0
    for i in range(1, h.GetNcells() - 1):
        ref = ref + i
        h.SetBinContent(i, i) 

    sum = h[::ROOT.sum()]
    assert type(sum) is float
    assert sum == ref 

## actual test run
if __name__ == '__main__':
    result = run_pytest(__file__)
    sys.exit(result)
