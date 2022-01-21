import os
import unittest

from dask.distributed import Client, LocalCluster
import numpy

import ROOT
from DistRDF.Backends import Dask

class ReducerMergeTest(unittest.TestCase):
    """Check the working of merge operations in the reducer function."""

    @classmethod
    def setUpClass(cls):
        """
        Set up test environment for this class. Currently this includes:

        - Initialize a Dask client for the tests in this class. This uses a
          `LocalCluster` object that spawns 2 single-threaded Python processes.
        """
        cls.client = Client(LocalCluster(n_workers=2, threads_per_worker=1, processes=True))

    @classmethod
    def tearDownClass(cls):
        """Reset test environment."""
        cls.client.shutdown()
        cls.client.close()

    def assertHistoOrProfile(self, obj_1, obj_2):
        """Asserts equality between two 'ROOT.TH1' or 'ROOT.TH2' objects."""
        # Compare the sizes of equivalent objects
        self.assertEqual(obj_1.GetEntries(), obj_2.GetEntries())

        # Compare the means of equivalent objects
        self.assertEqual(obj_1.GetMean(), obj_2.GetMean())

        # Compare the standard deviations of equivalent objects
        self.assertEqual(obj_1.GetStdDev(), obj_2.GetStdDev())

    def define_two_columns(self, rdf):
        """
        Helper method that Defines and returns two columns with definitions
        "x = rdfentry_" and "y = rdfentry_ * rdfentry_".

        """
        return rdf.Define("x", "rdfentry_").Define("y", "rdfentry_*rdfentry_")

    def define_three_columns(self, rdf):
        """
        Helper method that Defines and returns three columns with definitions
        "x = rdfentry_", "y = rdfentry_ * rdfentry_" and
        "z = rdfentry_ * rdfentry_ * rdfentry_".

        """
        return rdf.Define("x", "rdfentry_")\
                  .Define("y", "rdfentry_*rdfentry_")\
                  .Define("z", "rdfentry_*rdfentry_*rdfentry_")

    def test_histo1d_merge(self):
        """Check the working of Histo1D merge operation in the reducer."""
        # Operations with DistRDF
        rdf_py = Dask.RDataFrame(10, daskclient=self.client)
        histo_py = rdf_py.Histo1D("rdfentry_")

        # Operations with PyROOT
        rdf_cpp = ROOT.ROOT.RDataFrame(10)
        histo_cpp = rdf_cpp.Histo1D("rdfentry_")

        # Compare the 2 histograms
        self.assertHistoOrProfile(histo_py, histo_cpp)

    def test_histo2d_merge(self):
        """Check the working of Histo2D merge operation in the reducer."""
        modelTH2D = ("", "", 64, -4, 4, 64, -4, 4)

        # Operations with DistRDF
        rdf_py = Dask.RDataFrame(10, daskclient=self.client)
        columns_py = self.define_two_columns(rdf_py)
        histo_py = columns_py.Histo2D(modelTH2D, "x", "y")

        # Operations with PyROOT
        rdf_cpp = ROOT.ROOT.RDataFrame(10)
        columns_cpp = self.define_two_columns(rdf_cpp)
        histo_cpp = columns_cpp.Histo2D(modelTH2D, "x", "y")

        # Compare the 2 histograms
        self.assertHistoOrProfile(histo_py, histo_cpp)

    def test_histo3d_merge(self):
        """Check the working of Histo3D merge operation in the reducer."""
        modelTH3D = ("", "", 64, -4, 4, 64, -4, 4, 64, -4, 4)
        # Operations with DistRDF
        rdf_py = Dask.RDataFrame(10, daskclient=self.client)
        columns_py = self.define_three_columns(rdf_py)
        histo_py = columns_py.Histo3D(modelTH3D, "x", "y", "z")

        # Operations with PyROOT
        rdf_cpp = ROOT.ROOT.RDataFrame(10)
        columns_cpp = self.define_three_columns(rdf_cpp)
        histo_cpp = columns_cpp.Histo3D(modelTH3D, "x", "y", "z")

        # Compare the 2 histograms
        self.assertHistoOrProfile(histo_py, histo_cpp)

    def test_profile1d_merge(self):
        """Check the working of Profile1D merge operation in the reducer."""
        # Operations with DistRDF
        rdf_py = Dask.RDataFrame(10, daskclient=self.client)
        columns_py = self.define_two_columns(rdf_py)
        profile_py = columns_py.Profile1D(("", "", 64, -4, 4), "x", "y")

        # Operations with PyROOT
        rdf_cpp = ROOT.ROOT.RDataFrame(10)
        columns_cpp = self.define_two_columns(rdf_cpp)
        profile_cpp = columns_cpp.Profile1D(("", "", 64, -4, 4), "x", "y")

        # Compare the 2 profiles
        self.assertHistoOrProfile(profile_py, profile_cpp)

    def test_profile2d_merge(self):
        """Check the working of Profile2D merge operation in the reducer."""
        model = ("", "", 64, -4, 4, 64, -4, 4)

        # Operations with DistRDF
        rdf_py = Dask.RDataFrame(10, daskclient=self.client)
        columns_py = self.define_three_columns(rdf_py)
        profile_py = columns_py.Profile2D(model, "x", "y", "z")

        # Operations with PyROOT
        rdf_cpp = ROOT.ROOT.RDataFrame(10)
        columns_cpp = self.define_three_columns(rdf_cpp)
        profile_cpp = columns_cpp.Profile2D(model, "x", "y", "z")

        # Compare the 2 profiles
        self.assertHistoOrProfile(profile_py, profile_cpp)

    @unittest.skipIf(ROOT.gROOT.GetVersion() < '6.16',
                     "Graph featured included in ROOT-6.16 for the first time")
    def test_tgraph_merge(self):
        """Check the working of TGraph merge operation in the reducer."""
        # Operations with DistRDF
        rdf_py = Dask.RDataFrame(10, daskclient=self.client)
        columns_py = self.define_two_columns(rdf_py)
        graph_py = columns_py.Graph("x", "y")

        # Operations with PyROOT
        rdf_cpp = ROOT.ROOT.RDataFrame(10)
        columns_cpp = self.define_two_columns(rdf_cpp)
        graph_cpp = columns_cpp.Graph("x", "y")

        # Sort the graphs to make sure corresponding points are same
        graph_py.Sort()
        graph_cpp.Sort()

        # Compare the X co-ordinates of the graphs
        self.assertListEqual(list(graph_py.GetX()), list(graph_cpp.GetX()))

        # Compare the Y co-ordinates of the graphs
        self.assertListEqual(list(graph_py.GetY()), list(graph_cpp.GetY()))

    def test_distributed_count(self):
        """Test support for `Count` operation in distributed backend"""
        rdf_py = Dask.RDataFrame(100, daskclient=self.client)
        count = rdf_py.Count()

        self.assertEqual(count.GetValue(), 100)

    def test_distributed_sum(self):
        """Test support for `Sum` operation in distributed backend"""
        rdf_py = Dask.RDataFrame(10, daskclient=self.client)
        rdf_def = rdf_py.Define("x", "rdfentry_")
        rdf_sum = rdf_def.Sum("x")

        self.assertAlmostEqual(rdf_sum.GetValue(), 45.0)

    def test_distributed_asnumpy(self):
        """Test support for `AsNumpy` pythonization in distributed backend"""

        # Let's create a simple dataframe with ten rows and two columns
        df = Dask.RDataFrame(10, daskclient=self.client).Define("x", "(int)rdfentry_")\
            .Define("y", "1.f/(1.f+rdfentry_)")

        # Build a dictionary of numpy arrays.
        npy = df.AsNumpy()
        self.assertIsInstance(npy, dict)

        # Retrieve the two numpy arrays with the column names of the original
        # RDataFrame as dictionary keys.
        npy_x = npy["x"]
        npy_y = npy["y"]
        self.assertIsInstance(npy_x, numpy.ndarray)
        self.assertIsInstance(npy_y, numpy.ndarray)

        # Check the two arrays are of the same length as the original columns.
        self.assertEqual(len(npy_x), 10)
        self.assertEqual(len(npy_y), 10)

        # Check the types correspond to the ones of the original columns.
        int_32_dtype = numpy.dtype("int32")
        float_32_dtype = numpy.dtype("float32")
        self.assertEqual(npy_x.dtype, int_32_dtype)
        self.assertEqual(npy_y.dtype, float_32_dtype)

    def test_distributed_asnumpy_columns(self):
        """
        Test that distributed AsNumpy correctly accepts the 'columns' keyword
        argument.
        """

        # Let's create a simple dataframe with ten rows and two columns
        df = Dask.RDataFrame(10, daskclient=self.client)\
                  .Define("x", "(int)rdfentry_")\
                  .Define("y", "1.f/(1.f+rdfentry_)")

        # Build a dictionary of numpy arrays.
        npy = df.AsNumpy(columns=["x"])

        # Check the dictionary only has the desired column
        self.assertListEqual(list(npy.keys()), ["x"])

        # Check correctness of the output array
        npy_x = npy["x"]
        self.assertIsInstance(npy_x, numpy.ndarray)
        self.assertEqual(len(npy_x), 10)
        int_32_dtype = numpy.dtype("int32")
        self.assertEqual(npy_x.dtype, int_32_dtype)

    def test_distributed_snapshot(self):
        """Test support for `Snapshot` in distributed backend"""
        # A simple dataframe with ten sequential numbers from 0 to 9
        df = Dask.RDataFrame(10, daskclient=self.client).Define("x", "rdfentry_")

        # Count rows in the dataframe
        nrows = df.Count()

        # Snapshot to two files, build a ROOT.TChain with them and retrieve a
        # Dask.RDataFrame
        snapdf = df.Snapshot("snapTree", "snapFile.root")

        # Count the rows in the snapshotted dataframe
        snapcount = snapdf.Count()

        self.assertEqual(nrows.GetValue(), 10)
        self.assertEqual(snapcount.GetValue(), 10)

        # Retrieve list of file from the snapshotted dataframe
        input_files = snapdf.proxied_node.inputfiles
        # Create list of supposed filenames for the intermediary files
        tmp_files = ["snapFile_0.root", "snapFile_1.root"]
        # Check that the two lists are the same
        self.assertListEqual(input_files, tmp_files)
        # Check that the intermediary .root files were created with the right
        # names, then remove them because they are not necessary
        for filename in tmp_files:
            self.assertTrue(os.path.exists(filename))
            os.remove(filename)

    def test_distributed_snapshot_columnlist(self):
        """
        Test that distributed Snapshot correctly passes also the third input
        argument "columnList".
        """
        # A simple dataframe with ten sequential numbers from 0 to 9
        df = Dask.RDataFrame(10, daskclient=self.client)\
                  .Define("a", "rdfentry_")\
                  .Define("b", "rdfentry_")\
                  .Define("c", "rdfentry_")\
                  .Define("d", "rdfentry_")

        expectedcolumns = ["a", "b"]
        df.Snapshot("snapTree_columnlist", "snapFile_columnlist.root", expectedcolumns)

        # Create a traditional RDF from the snapshotted files to retrieve the
        # list of columns
        tmp_files = ["snapFile_columnlist_0.root", "snapFile_columnlist_1.root"]
        rdf = ROOT.RDataFrame("snapTree_columnlist", tmp_files)
        snapcolumns = [str(column) for column in rdf.GetColumnNames()]

        self.assertListEqual(snapcolumns, expectedcolumns)

        for filename in tmp_files:
            os.remove(filename)

    def test_redefine_one_column(self):
        """Test that values of one column can be properly redefined."""
        # A simple dataframe with ten sequential numbers from 0 to 9
        df = Dask.RDataFrame(10, daskclient=self.client)
        df_before = df.Define("x", "1")
        df_after = df_before.Redefine("x", "2")

        # Initial sum should be equal to 10
        sum_before = df_before.Sum("x")
        # Sum after the redefinition should be equal to 20
        sum_after = df_after.Sum("x")

        self.assertAlmostEqual(sum_before.GetValue(), 10.0)
        self.assertAlmostEqual(sum_after.GetValue(), 20.0)


if __name__ == "__main__":
    unittest.main(argv=[__file__])
