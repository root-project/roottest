// Assert that the use of a weak symbol (that the JIT can compile) is not
// triggering autoloading.
// Also assert that - once the library has been loaded - symbol resolution
// of weak symbols happens from te library. That latter part is a (significant)
// performance optimization, where cling re-uses existing symbols instead of
// re-JITting them. That should be fine according to ODR.
#include "RooCacheManager.h"
#ifndef R__WIN32
#include "dlfcn.h"
#endif

int someFunctionThatMightTriggerAutoloading(RooCacheManager<double> *rcm) {
   // RooCacheManager::cacheSize() is a weak symbol.
   // ROOT doesn't have RooFit loaded here, and it has the definition of the
   // function through the `#include` above). This should *not* trigger
   // autoloading of libRooFit:
   return rcm->cacheSize();
}

int assertWeakSymbol() {
   void* ptr = (void*)&someFunctionThatMightTriggerAutoloading;
   std::cout << (ptr != nullptr) << '\n'; // to force generation of symbol someFunctionThatMightTriggerAutoloading
   if (strstr(gInterpreter->GetSharedLibs(), "RooFit")) {
      std::cerr << "ERROR: libRooFit was loaded, even though we only looked for a weak symbol!\n";
      exit(1);
   }

#ifndef R__WIN32
   gSystem->Load("libRooFit");
   long val = gInterpreter->ProcessLine("(double(*)(double))&RooMath::erf;");
   Dl_info dlInfo;
   int res = dladdr((void*)val, &dlInfo);
   if (!dlInfo.dli_fname || !strstr(dlInfo.dli_fname, "RooFit")) {
      std::cerr << "ERROR: RooMath::erf was not resolved from libRooFit even though it could have been!\n";
      exit(1);
   }
#endif
   return 0;
}
