#include "tExecutor.h"
#include "ROOT/TExecutor.hxx"

int PoolTest() {

   //////////////1-11:Tests Sequential///////////////
   ROOT::Internal::TExecutor pool(ROOT::Internal::ExecutionPolicy::kSequential);
   auto res = TExecutorPoolTest(pool);
   if (res!=0)
      return res;

#ifdef R__USE_IMT
   //////////////12-22 Tests MT///////////////
   ROOT::Internal::TExecutor pool1(ROOT::Internal::ExecutionPolicy::kMultithread);
      auto res1 = TExecutorPoolTest(pool1);
      if (res1!=0)
         return res1+11;
#endif

   //////////////23-33 Tests MP///////////////
   ROOT::Internal::TExecutor pool2(ROOT::Internal::ExecutionPolicy::kMultiprocess);
      auto res2 = TExecutorPoolTest(pool2);
      if (res2!=0)
         return res2+22;

   //////////////34-44 Tests Default///////////////
   ROOT::Internal::TExecutor pool3;
      auto res3 = TExecutorPoolTest(pool3);
      if (res3!=0)
         return res3+33;
}

int main() {
	return PoolTest();
}
