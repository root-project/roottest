#include "TThread.h"
#include <Riostream.h>

void *handle(void *ptr)
{
   long nr = (long) ptr;
   void *mem = NULL;
   for (int i = 0; ; i++) {
      mem = malloc(8);
      free(mem);
   }
   return 0;
}

void testSigIll()
{
   TThread *h1 = new TThread("h1", handle, (void*) 1);
   h1->Run();
   TThread *h2 = new TThread("h2", handle, (void*) 2);
   h2->Run();
   TThread *h3 = new TThread("h3", handle, (void*) 3);
   h3->Run();

   kill(getpid(),SIGILL);

   h1->Join();
   h2->Join();
   h3->Join();
}

