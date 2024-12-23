int countSubstring(const std::string& str, const std::string& sub)
{
   if (sub.length() == 0) return 0;
   int count = 0;
   for (size_t offset = str.find(sub); offset != std::string::npos;
      offset = str.find(sub, offset + sub.length())) { ++count; }
   return count;
}

int countIncludePaths()
{
   for (auto lib : {"libMathCore", "libunordered_mapDict", "libmapDict", "libHist"})
      gSystem->Load(lib);

   std::string includePath(gSystem->GetIncludePath());

   // Exclude from the test the builds with R as external package
   if (std::string::npos != includePath.find("RInside/include")) return 0;

   int maxPaths = 10; // At most

   // detect if -Dclingtest=ON, which adds extra paths:
   // interpreter/cling/include, interpreter/llvm-project/clang/include, interpreter/llvm-project/llvm/tools/clang//include, interpreter/llvm-project/llvm/include, interpreter/llvm-project/llvm/include
   if (std::string::npos != includePath.find("interpreter/cling/include"))
      maxPaths += 5;
   auto nPaths = countSubstring(includePath, "-I");
   if (nPaths > maxPaths) {
      std::cerr << "The number of include paths is too high (" << nPaths << ">" << maxPaths << ") "
                << ". The number of \"-I\"s has been counted in the include path of ROOT (gSystem->GetIncludePath()=" << includePath << ")." << std::endl;
      return nPaths;
   }
   return 0;
}
