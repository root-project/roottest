import math

import pytest

import ROOT

from DistRDF.Backends import Dask


class TestSharedLibDask:
    """
    Check that the required header files are properly included in Dask
    environment.
    """
    
    def _check_rdf_histos(self, rdf):
        # This filters out all numbers less than 5
        rdf_filtered = rdf.Filter("check_number_less_than_5(rdfentry_)")
        histo = rdf_filtered.Histo1D(("name", "title", 10, 0, 10), "rdfentry_")

        # The expected results after filtering
        # The actual set of numbers required after filtering
        required_numbers = range(5)
        required_size = len(required_numbers)
        required_mean = sum(required_numbers) / float(required_size)
        required_stdDev = math.sqrt(
            sum((x - required_mean)**2 for x in required_numbers) /
            required_size)

        # Compare the sizes of equivalent set of numbers
        assert histo.GetEntries() == required_size
        # Compare the means of equivalent set of numbers
        assert histo.GetMean() == required_mean
        # Compare the standard deviations of equivalent set of numbers
        assert histo.GetStdDev() == required_stdDev
        from DistRDF.Backends.Base import BaseBackend
        BaseBackend.headers = set()   
        
    def _check_rdf_histos_second(self, rdf):
        # This filters out all numbers less than 5
        rdf_filtered = rdf.Filter("check_number_less_than_4(rdfentry_)")
        histo = rdf_filtered.Histo1D(("name", "title", 10, 0, 10), "rdfentry_")

        # The expected results after filtering
        # The actual set of numbers required after filtering
        required_numbers = range(4)
        required_size = len(required_numbers)
        required_mean = sum(required_numbers) / float(required_size)
        required_stdDev = math.sqrt(
            sum((x - required_mean)**2 for x in required_numbers) /
            required_size)

        # Compare the sizes of equivalent set of numbers
        assert histo.GetEntries() == required_size
        # Compare the means of equivalent set of numbers
        assert histo.GetMean() == required_mean
        # Compare the standard deviations of equivalent set of numbers
        assert histo.GetStdDev() == required_stdDev
        from DistRDF.Backends.Base import BaseBackend
        BaseBackend.headers = set()   
    
    def _distribute_header_check_filter_and_histo(self, connection, backend):
        """
        Check that the filter operation is able to use C++ functions that
        were included using header files.
        """
        if backend == "dask":
            RDataFrame = ROOT.RDF.Experimental.Distributed.Dask.RDataFrame
            rdf = RDataFrame(10, daskclient=connection)       
        elif backend == "spark":
            RDataFrame = ROOT.RDF.Experimental.Distributed.Spark.RDataFrame
            rdf = RDataFrame(10, sparkcontext=connection)
            
        ROOT.RDF.Experimental.Distributed.DistributeHeaders("../test_headers/header1.hxx")     
        self._check_rdf_histos(rdf)

    def _extend_ROOT_include_path(self, connection, backend):
        """
        Check that the include path of ROOT is extended with the directories
        specified in `DistRDF.include_headers()` so references between headers
        are correctly solved.
        """
        header_folder = "../test_headers/headers_folder"
        
        # Create an RDataFrame with 100 integers from 0 to 99
        if backend == "dask":
            RDataFrame = ROOT.RDF.Experimental.Distributed.Dask.RDataFrame
            rdf = RDataFrame(100, daskclient=connection)
        elif backend == "spark":
            RDataFrame = ROOT.RDF.Experimental.Distributed.Spark.RDataFrame
            rdf = RDataFrame(100, sparkcontext=connection)
                    
        ROOT.RDF.Experimental.Distributed.DistributeHeaders(header_folder)
        # Get list of include paths seen by ROOT
        ROOT_include_path = ROOT.gInterpreter.GetIncludePath().split(" ")

        # Create new include folder token
        new_folder_include = "-I\"{}\"".format(header_folder)

        # Check that new folder is in ROOT include paths
        assert new_folder_include in ROOT_include_path

        # Filter numbers less than 10 and create an histogram
        rdf_less_than_10 = rdf.Filter("check_number_less_than_10(rdfentry_)")
        histo1 = rdf_less_than_10.Histo1D(
            ("name", "title", 10, 0, 100), "rdfentry_")

        # Check that histogram has 10 entries and mean 4.5
        assert histo1.GetEntries() == 10
        assert histo1.GetMean() == pytest.approx(4.5)
        
        # Reset headers for future tests
        # TODO: this underlines either a misusage of the class or a bug
        from DistRDF.Backends.Base import BaseBackend
        BaseBackend.headers = set()


    def _distribute_shared_lib_check_filter_and_histo(self, connection, backend):
        """
        Check that the filter operation is able to use C++ functions that
        were included using header files.
        """
        if backend == "dask":
            RDataFrame = ROOT.RDF.Experimental.Distributed.Dask.RDataFrame
            rdf = RDataFrame(10, daskclient=connection)
        elif backend == "spark":
            RDataFrame = ROOT.RDF.Experimental.Distributed.Spark.RDataFrame
            rdf = RDataFrame(100, sparkcontext=connection)        
        
        ROOT.RDF.Experimental.Distributed.DistributeSharedLib("../test_shared_libs/headersecond_hxx.so")
        self._check_rdf_histos_second(rdf)
        
    def _distribute_shared_lib_folder_check_filter_and_histo(self, connection, backend):
        """
        Check that the filter operation is able to use C++ functions that
        were included using header files.
        """
        if backend == "dask":
            RDataFrame = ROOT.RDF.Experimental.Distributed.Dask.RDataFrame
            rdf = RDataFrame(10, daskclient=connection)
        elif backend == "spark":
            RDataFrame = ROOT.RDF.Experimental.Distributed.Spark.RDataFrame
            rdf = RDataFrame(10, sparkcontext=connection)
        
        ROOT.RDF.Experimental.Distributed.DistributeSharedLib("../test_shared_libs/")
        self._check_rdf_histos(rdf)
        

    def _distribute_multiple_shared_lib_folder_check_filter_and_histo(self, connection, backend):
        if backend == "dask":
            RDataFrame = ROOT.RDF.Experimental.Distributed.Dask.RDataFrame
            rdf = RDataFrame(10, daskclient=connection)
        elif backend == "spark":
            RDataFrame = ROOT.RDF.Experimental.Distributed.Spark.RDataFrame
            rdf = RDataFrame(10, sparkcontext=connection)
        
        ROOT.RDF.Experimental.Distributed.DistributeSharedLib("../test_shared_libs/")
        self._check_rdf_histos(rdf)
        self._check_rdf_histos_second(rdf)
        
    def _distribute_multiple_shared_lib_check_filter_and_histo(self, connection, backend):
        if backend == "dask":
            RDataFrame = ROOT.RDF.Experimental.Distributed.Dask.RDataFrame
            rdf = RDataFrame(10, daskclient=connection)
        elif backend == "spark":
            RDataFrame = ROOT.RDF.Experimental.Distributed.Spark.RDataFrame
            rdf = RDataFrame(10, sparkcontext=connection)
        
        ROOT.RDF.Experimental.Distributed.DistributeSharedLib(["../test_shared_libs/header1_hxx.so","../test_shared_libs/headersecond_hxx.so"])
        self._check_rdf_histos(rdf)
        self._check_rdf_histos_second(rdf)
    
    def _distribute_single_declare(self, connection, backend):
    
        if backend == "dask":
            RDataFrame = ROOT.RDF.Experimental.Distributed.Dask.RDataFrame
            rdf = RDataFrame(10, daskclient=connection)
        elif backend == "spark":
            RDataFrame = ROOT.RDF.Experimental.Distributed.Spark.RDataFrame
            rdf = RDataFrame(100, sparkcontext=connection)
            
        from DistRDF.Backends.Base import BaseBackend
        BaseBackend.declaration_str = ""
                
        ROOT.RDF.Experimental.Distributed.DeclareCppCode("""
            bool check_number_less_than_5(int num){
            return num < 5;
            }  
            """)
        
        self._check_rdf_histos(rdf)

    # TODO the test here would pass but it uncovers that the multiple declares are called more times than needed
    # if they are called only once, the test would fail 
    
    # def _distribute_multiple_declares(self, connection, backend):
        
    #     if backend == "dask":
    #         RDataFrame = ROOT.RDF.Experimental.Distributed.Dask.RDataFrame
    #         rdf = RDataFrame(10, daskclient=connection)
    #     elif backend == "spark":
    #         RDataFrame = ROOT.RDF.Experimental.Distributed.Spark.RDataFrame
    #         rdf = RDataFrame(100, sparkcontext=connection)
        
    #     from DistRDF.Backends.Base import BaseBackend
    #     BaseBackend.declaration_string = "" 
        
    #     ROOT.RDF.Experimental.Distributed.DeclareCppCode("""
    #         bool check_number_less_than_5(int num){
    #         return num < 5;
    #         }  
    #         """) 
        
    #     ROOT.RDF.Experimental.Distributed.DeclareCppCode("""
    #         bool check_number_less_than_4(int num){
    #         return num < 4;
    #         }  
    #         """)
        
    #     self._check_rdf_histos(rdf)
    #     self._check_rdf_histos_second(rdf)
    
    def _distribute_single_file(self, connection, backend):
        # another, hacky way would be to print localdir of workers and ls what's inside
        # here we distribute file that is a header 
        # #TODO we should really test the files that are not headers
        if backend == "dask":
            RDataFrame = ROOT.RDF.Experimental.Distributed.Dask.RDataFrame
            rdf = RDataFrame(10, daskclient=connection)
        elif backend == "spark":
            RDataFrame = ROOT.RDF.Experimental.Distributed.Spark.RDataFrame
            rdf = RDataFrame(100, sparkcontext=connection)
                    
        ROOT.RDF.Experimental.Distributed.DistributeFiles("../test_headers/header1.hxx")
        self._check_rdf_histos(rdf)
    
    # def _distribute_multiple_files(self, connection, backend):
    #     # another, hacky way would be to print localdir of workers and ls what's inside
    #     # here we distribute file that is a header
    #     # #TODO issue if we try with two headers - this is not the correct way to run the files test, here for the reference
    #     if backend == "dask":
    #         RDataFrame = ROOT.RDF.Experimental.Distributed.Dask.RDataFrame
    #         rdf = RDataFrame(10, daskclient=connection)
    #     elif backend == "spark":
    #         RDataFrame = ROOT.RDF.Experimental.Distributed.Spark.RDataFrame
    #         rdf = RDataFrame(100, sparkcontext=connection)
                    
    #     ROOT.RDF.Experimental.Distributed.DistributeFiles(["../test_headers/header1.hxx", "../test_headers/headersecond.hxx"])
    #     self._check_rdf_histos(rdf)
    #     self._check_rdf_histos_second(rdf)
    
    def test_check_new_interfaces(self, payload):
        """
        Tests for the distribution of headers to the workers and their
        corresponding inclusion.
        """
        connection, backend = payload
        
        self._distribute_header_check_filter_and_histo(connection, backend)
        self._extend_ROOT_include_path(connection, backend)
        # TODO how do we proceed with the shared libs tests? They pass locally. 
        # self._distribute_shared_lib_check_filter_and_histo(connection, backend)
        # self._distribute_shared_lib_folder_check_filter_and_histo(connection, backend)
        # self._distribute_multiple_shared_lib_check_filter_and_histo(connection, backend)
        # self._distribute_multiple_shared_lib_folder_check_filter_and_histo(connection, backend)
        self._distribute_single_declare(connection, backend)
        #self._distribute_multiple_declares(connection, backend)
        self._distribute_single_file(connection, backend)
        #self._distribute_multiple_files(connection, backend)
        

if __name__ == "__main__":
    pytest.main(args=[__file__])
