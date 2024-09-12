import math
import pytest
import os 

from distributed import get_worker
from pathlib import Path

import ROOT

class TestInterfaceHeadersLibrariesFiles:
    """
    Check that the interface functions to distribute headers, shared libraries and files work properly. 
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
        # Reset headers for future tests
        # TODO: this underlines either a misusage of the class or a bug
        # this way we avoid SPARK warnings like:
        # WARN SparkContext: 
        # The path ../test_headers/header1.hxx has been added already. Overwriting of added paths is not supported in the current version.
        from DistRDF.Backends.Base import BaseBackend
        BaseBackend.headers = set()
        

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
        # this way we avoid SPARK warnings like:
        # WARN SparkContext: 
        # The path ../test_headers/header1.hxx has been added already. Overwriting of added paths is not supported in the current version.
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
        
        ROOT.RDF.Experimental.Distributed.DistributeSharedLibs("../test_shared_libs/headersecond_hxx.so")
        self._check_rdf_histos_second(rdf)
        from DistRDF.Backends.Base import BaseBackend
        BaseBackend.shared_libraries = set()
        BaseBackend.pcms = set()
        
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
        
        ROOT.RDF.Experimental.Distributed.DistributeSharedLibs("../test_shared_libs/")
        self._check_rdf_histos(rdf)
        from DistRDF.Backends.Base import BaseBackend
        BaseBackend.shared_libraries = set()
        BaseBackend.pcms = set()

    def _distribute_multiple_shared_lib_folder_check_filter_and_histo(self, connection, backend):
        if backend == "dask":
            RDataFrame = ROOT.RDF.Experimental.Distributed.Dask.RDataFrame
            rdf = RDataFrame(10, daskclient=connection)
        elif backend == "spark":
            RDataFrame = ROOT.RDF.Experimental.Distributed.Spark.RDataFrame
            rdf = RDataFrame(10, sparkcontext=connection)
        
        ROOT.RDF.Experimental.Distributed.DistributeSharedLibs("../test_shared_libs/")
        self._check_rdf_histos(rdf)
        self._check_rdf_histos_second(rdf)
        from DistRDF.Backends.Base import BaseBackend
        BaseBackend.shared_libraries = set()
        BaseBackend.pcms = set()
        
        
    def _distribute_multiple_shared_lib_check_filter_and_histo(self, connection, backend):
        if backend == "dask":
            RDataFrame = ROOT.RDF.Experimental.Distributed.Dask.RDataFrame
            rdf = RDataFrame(10, daskclient=connection)
        elif backend == "spark":
            RDataFrame = ROOT.RDF.Experimental.Distributed.Spark.RDataFrame
            rdf = RDataFrame(10, sparkcontext=connection)
        
        ROOT.RDF.Experimental.Distributed.DistributeSharedLibs(["../test_shared_libs/header1_hxx.so","../test_shared_libs/headersecond_hxx.so"])
        self._check_rdf_histos(rdf)
        self._check_rdf_histos_second(rdf)
        from DistRDF.Backends.Base import BaseBackend
        BaseBackend.shared_libraries = set()
        BaseBackend.pcms = set()
    
    
    def _distribute_single_file(self, connection, backend):
        # #TODO do we want to test Spark as well? 
        # For spark we are using spark "addFile" function directly 
        if backend == "dask":
            RDataFrame = ROOT.RDF.Experimental.Distributed.Dask.RDataFrame
            rdf = RDataFrame(10, daskclient=connection)
        elif backend == "spark":
            RDataFrame = ROOT.RDF.Experimental.Distributed.Spark.RDataFrame
            rdf = RDataFrame(100, sparkcontext=connection)
        
        ROOT.RDF.Experimental.Distributed.DistributeFiles("../test_files/file.txt")

        if backend == "dask":
            def Foo():
                try:
                    localdir = get_worker().local_directory
                    
                    if os.path.exists(Path(localdir)/"file.txt"):
                        os.environ['ENV'] = localdir
                except ValueError:
                    pass
                ROOT.gInterpreter.Declare(
                """
                #ifndef CODE_ENV
                #define CODE_ENV
                bool isEnv(){
                    return  gSystem->Getenv("ENV");
                }
                #endif
                """
                )
                
            ROOT.RDF.Experimental.Distributed.initialize(Foo)
            df_flag = rdf.Define("flags", "isEnv()")
            countFlags = df_flag.Sum("flags").GetValue()
            assert countFlags == 10.0    
        # Reset files for future tests
        # TODO: this underlines either a misusage of the class or a bug
        from DistRDF.Backends.Base import BaseBackend
        BaseBackend.files = set()
            
    def _distribute_multiple_files(self, connection, backend):
        # #TODO do we want to test Spark as well? 
        # For spark we are using spark "addFile" function directly 
    
        if backend == "dask":
            RDataFrame = ROOT.RDF.Experimental.Distributed.Dask.RDataFrame
            rdf = RDataFrame(10, daskclient=connection)
        elif backend == "spark":
            RDataFrame = ROOT.RDF.Experimental.Distributed.Spark.RDataFrame
            rdf = RDataFrame(100, sparkcontext=connection)
        
        ROOT.RDF.Experimental.Distributed.DistributeFiles(["../test_files/file.txt", "../test_files/file_1.txt"])

        if backend == "dask":
            def Foo():
                try:
                    localdir = get_worker().local_directory
                    if os.path.exists(Path(localdir)/"file.txt") and os.path.exists(Path(localdir)/"file_1.txt"):
                        os.environ['ENV'] = localdir
                except ValueError:
                    pass
                ROOT.gInterpreter.Declare(
                """
                #ifndef CODE_ENV
                #define CODE_ENV
                bool isEnv(){
                    return  gSystem->Getenv("ENV");
                }
                #endif
                """
                )
                
            ROOT.RDF.Experimental.Distributed.initialize(Foo)
            df_flag = rdf.Define("flags", "isEnv()")
            countFlags = df_flag.Sum("flags").GetValue()
            assert countFlags == 10.0
            
        # Reset files for future tests
        # TODO: this underlines either a misusage of the class or a bug
        from DistRDF.Backends.Base import BaseBackend
        BaseBackend.files = set()
        
    
    def test_check_new_interfaces(self, payload):
        """
        Tests for the distribution of headers to the workers and their
        corresponding inclusion and tests for the distribution of files. 
        """
        connection, backend = payload
        self._distribute_header_check_filter_and_histo(connection, backend)
        self._extend_ROOT_include_path(connection, backend)
        self._distribute_shared_lib_check_filter_and_histo(connection, backend)
        self._distribute_shared_lib_folder_check_filter_and_histo(connection, backend)
        self._distribute_multiple_shared_lib_check_filter_and_histo(connection, backend)
        self._distribute_multiple_shared_lib_folder_check_filter_and_histo(connection, backend)
        self._distribute_single_file(connection, backend)
        self._distribute_multiple_files(connection, backend)

if __name__ == "__main__":
    pytest.main(args=[__file__])
