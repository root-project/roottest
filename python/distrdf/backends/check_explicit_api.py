import pytest

import ROOT

import DistRDF


class TestExplicitAPI:
    """
    Tests for compatibility with using full names to classes and functions of
    distributed RDataFrame.
    """

    def test_histo_variations(self, payload):
        connection, backend = payload
        if backend == "dask":
            RDataFrame = ROOT.RDF.Experimental.Distributed.Dask.RDataFrame
            df = RDataFrame(10, npartitions=2, daskclient=connection)
        elif backend == "spark":
            RDataFrame = ROOT.RDF.Experimental.Distributed.Spark.RDataFrame
            df = RDataFrame(10, npartitions=2, sparkcontext=connection)
        df = df.Define("x", "1")
        df1 = df.Vary("x", "ROOT::RVecI{-2,2}", ["down", "up"])
        h = df1.Histo1D(("name", "title", 10, -10, 10), "x")
        histos = DistRDF.VariationsFor(h)

        expectednames = ["nominal", "x:up", "x:down"]
        expectedmeans = [1, 2, -2]
        for varname, mean in zip(expectednames, expectedmeans):
            histo = histos[varname]
            assert isinstance(histo, ROOT.TH1D)
            assert histo.GetEntries() == 10
            assert histo.GetMean() == mean

    def test_rungraphs_3histos(self, payload):
        """
        Submit three different Dask RDF graphs concurrently
        """
        # Create a test file for processing
        treename = "tree"
        filename = "../data/ttree/distrdf_roottest_check_rungraphs.root"
        nentries = 10000
        connection, backend = payload
        if backend == "dask":
            RDataFrame = ROOT.RDF.Experimental.Distributed.Dask.RDataFrame
        elif backend == "spark":
            RDataFrame = ROOT.RDF.Experimental.Distributed.Spark.RDataFrame

        histoproxies = [
            RDataFrame(treename, filename, npartitions=2,
                       executor=connection).Histo1D((col, col, 1, 40, 45), col)
            for col in ["b1", "b2", "b3"]
        ]

        # Before triggering the computation graphs values are None
        for proxy in histoproxies:
            assert proxy.proxied_node.value is None

        DistRDF.RunGraphs(histoproxies)

        # After RunGraphs all histograms are correctly assigned to the
        # node objects
        for proxy in histoproxies:
            histo = proxy.proxied_node.value
            assert isinstance(histo, ROOT.TH1D)
            assert histo.GetEntries() == nentries
            assert histo.GetMean() == 42

    def test_redefine_one_column(self, payload):
        """Test that values of one column can be properly redefined."""
        # A simple dataframe with ten sequential numbers from 0 to 9
        connection, backend = payload
        if backend == "dask":
            RDataFrame = ROOT.RDF.Experimental.Distributed.Dask.RDataFrame
            df = RDataFrame(10, daskclient=connection)
        elif backend == "spark":
            RDataFrame = ROOT.RDF.Experimental.Distributed.Spark.RDataFrame
            df = RDataFrame(10, sparkcontext=connection)
        df_before = df.Define("x", "1")
        df_after = df_before.Redefine("x", "2")

        # Initial sum should be equal to 10
        sum_before = df_before.Sum("x")
        # Sum after the redefinition should be equal to 20
        sum_after = df_after.Sum("x")

        assert sum_before.GetValue() == 10.0
        assert sum_after.GetValue() == 20.0


if __name__ == "__main__":
    pytest.main(args=[__file__])
