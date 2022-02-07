import os
import sys
import unittest
import warnings
from array import array

import pyspark
import ROOT
from DistRDF.Backends import Spark


class SparkHistoWriteTest(unittest.TestCase):
    """
    Integration tests to check writing histograms to a `TFile` distributedly.
    """

    @classmethod
    def setUpClass(cls):
        """
        Set up test environment for this class. Currently this includes:

        - Class-wide histogram-related parameters.
        - Synchronize PYSPARK_PYTHON variable to the current Python executable.
          Needed to avoid mismatch between python versions on driver and on the
          fake executor on the same machine.
        - Ignore `ResourceWarning: unclosed socket` warning triggered by Spark.
          this is ignored by default in any application, but Python's unittest
          library overrides the default warning filters thus exposing this
          warning
        - Initialize a SparkContext for the tests in this class
        """
        cls.nentries = 10000  # Number of fills
        cls.gaus_mean = 10  # Mean of the gaussian distribution
        cls.gaus_stdev = 1  # Standard deviation of the gaussian distribution
        cls.delta_equal = 0.01  # Delta to check for float equality

        if sys.version_info.major >= 3:
            warnings.simplefilter("ignore", ResourceWarning)

        sparkconf = pyspark.SparkConf().setMaster("local[2]")
        cls.sc = pyspark.SparkContext(conf=sparkconf)

    @classmethod
    def tearDownClass(cls):
        """Reset test environment."""
        if sys.version_info.major >= 3:
            warnings.simplefilter("default", ResourceWarning)

        cls.sc.stop()

    def create_tree_with_data(self):
        """Creates a .root file with some data"""
        f = ROOT.TFile("tree_gaus.root", "recreate")
        T = ROOT.TTree("Events", "Gaus(10,1)")

        x = array("f", [0])
        T.Branch("x", x, "x/F")

        r = ROOT.TRandom()
        # The parent will have a gaussian distribution with mean 10 and
        # standard deviation 1
        for i in range(self.nentries):
            x[0] = r.Gaus(self.gaus_mean, self.gaus_stdev)
            T.Fill()

        f.Write()
        f.Close()

    def test_write_histo(self):
        """
        Tests that an histogram is correctly written to a .root file created
        before the execution of the event loop.
        """
        self.create_tree_with_data()

        # Create a new file where the histogram will be written
        outfile = ROOT.TFile("out_file.root", "recreate")

        # Create a DistRDF RDataFrame with the parent and the friend trees
        df = Spark.RDataFrame("Events", "tree_gaus.root", sparkcontext=self.sc)

        # Create histogram
        histo = df.Histo1D(("x", "x", 100, 0, 20), "x")

        # Write histogram to out_file.root and close the file
        histo.Write()
        outfile.Close()

        # Reopen file to check that histogram was correctly stored
        reopen_file = ROOT.TFile("out_file.root", "read")
        reopen_histo = reopen_file.Get("x")

        # Check histogram statistics
        self.assertEqual(reopen_histo.GetEntries(), self.nentries)
        self.assertAlmostEqual(reopen_histo.GetMean(), self.gaus_mean,
                               delta=self.delta_equal)
        self.assertAlmostEqual(reopen_histo.GetStdDev(), self.gaus_stdev,
                               delta=self.delta_equal)

        # Remove unnecessary .root files
        os.remove("tree_gaus.root")
        os.remove("out_file.root")


if __name__ == "__main__":
    unittest.main(argv=[__file__])
