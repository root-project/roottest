#include "TPython.h"
#include <iostream>

void PyClassTest4() {
// python classes live in "MyPyClass.py" and "MyModule.py", which must have been
// loaded in a separate line or macro, or this one won't compile

// test access to C++ classes derived from python classes
   TPython::Exec( "ROOT.TPython.Result().voidPtrVal = ROOT.Derived1( 42 )" );
   Derived1* a = static_cast<Derived1 *>(TPython::Result().voidPtrVal);
   std::cout << "Derived1 (42):  " << a->fVal << std::endl;

   TPython::Exec( "ROOT.TPython.Result().voidPtrVal = ROOT.Derived2( 5.0 )" );
   Derived2* b = static_cast<Derived2 *>(TPython::Result().voidPtrVal);
   std::cout << "Derived2 (5.0): " << b->fVal << std::endl;
}
