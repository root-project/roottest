#include <thread>
#include "TMemFile.h"
#include "TSystem.h"
#include "TROOT.h"
#include <atomic>
#include <iostream>

static std::atomic<TFile*> gFileA{nullptr};
static std::atomic<TFile*> gFileB{nullptr};
static std::atomic_bool gWait{true};
static std::atomic_int gErrorCount{0};
static std::atomic_int gReadyCount{0};

#if 0
(1) thread one create TFile, gDirectorynow points to that file.
(2) thread two delete TFile, the destructor calls CleanTargetswhich has 4 distinct phase
(a) take the TFile spin lock and update all theTContextthat points to the file
(b) still hold the spin lock clean the other thread s directory.
(c) deal with theTContext that were being destructed at the same time
(d) update the local gDirectory

If between (2)(a) and (2)(b), thread (1) starts the creation of a TContext, and
is held at the start of RegisterContext after thread 2 release the spin lock,
thread 1 might awaken only after the TFile object has been deleted and thus
RegisterContext would access delete memory.

If during the destruction of the TFile by thread 2, thread (1) starts the
creation of a TContext, but is suspended right before the start of RegisterContext,
when it comes back it will use deleted memory to try to acquire the spin lock.

#endif

void thread_one()
{
   // create the file (a)
   std::cerr << "thread one create a.root\n";
   auto localFileA = new TMemFile("a.root", "RECREATE");
   gFileA = localFileA;
   //std::cerr << "gFileA : " << (void*)gFileA << '\n';
   //std::cerr << "t1.1 gDirectory : " << (void*)gDirectory << '\n';

   // other thread delete file (a)
   if (0) {
      std::cerr << "thread one waits for thread two\n";
      while (gWait) {};
      std::cout << "t1.2 gDirectory : " << (void*)gDirectory << '\n';
   }

   {
      std::cerr << "thread one creates context\n";

      // To provoke the original problem, change the code to
      // intentionally use the syntax specifying the 'previous' directory
      // to emulate the case where the TContext constructor would be executed
      // after the start of the TFile destruction but before gDirectory has
      // been updated (See above comments for a more precise description)
      // TDirectory::TContext ctxt{localFileA, nullptr};

      // To increase the likelyhood (with his is very small) that the race
      // condition is reprduce, one can add a 'sleep' in
      //    TDirectory::TContext::RegisterCurrentDirectory()
      // for "only" this invocation. For example at the time of this writing,
      // using the following in RegisterCurrentDirectory did guarantee to
      // provoke the race condition:
      //    static int count = 0;
      //    ++count;
      //    if (count == 14) {
      //      auto peek = TDirectory::CurrentDirectory().load();
      //      do {
      //        gSystem->Sleep(1000);
      //        peek = TDirectory::CurrentDirectory().load();
      //      } while (peek == current);
      //    }
      // For the problem to appear we need to have the file deleted between
      // the gDirectory read and the TContext update)
      TDirectory::TContext ctxt;

      // create another file (b)
      std::cerr << "thread one create b.root\n";
      gFileB = new TMemFile("b.root", "RECREATE");

      // delete file (b)
      std::cerr << "thread one deletes b.root\n";
      auto old = gFileB.exchange(nullptr);
      delete old;
   }

   std::cout << "gFileA : " << (void*)gFileA << '\n';
   std::cout << "thread one gDirectory : " << (void*)gDirectory << ' ' << gDirectory->GetName() << '\n';

}

void thread_two()
{
   TDirectory::TContext ctxt;

   std::cerr << "thread two waits for thread one\n";
   while (gFileA == nullptr) {}
   std::cerr << "thread two returns from waiting for thread one\n";
   // Add this sleep here to increase chance to provoke the race condition
   // if RegisterCurrentDirectory was also instrumented we a sleep
   // (we need to do the delete between the gDirectory read and the TContext update)
   if (false) {
      std::cerr << "thread two sleep 100ms\n";
      gSystem->Sleep(100);
   }
   // deletes the file (a)
   std::cerr << "thread two deletes a.root\n";
   auto old = gFileA.exchange(nullptr);
   delete old;
   std::cerr << "thread two completed delete a.root\n";
   std::cout << "thread two gDirectory: " << (TDirectory*)gDirectory << ' ' << gDirectory->GetName() << '\n';

   gWait = false;

}

void thread_three()
{
   std::cerr << "thread tree create a.root\n";
   auto localFileA = new TMemFile("a.root", "RECREATE");
   gFileA = localFileA;
   TDirectory *current = gDirectory;
   if (current != localFileA) {
      std::cerr << "ERROR: thread three gDirectory does not point to the file after creation\n";
      ++gErrorCount;
      return;
   }
   std::cout << "t3.1 gDirectory : " << (void*)gDirectory << ' ' << gDirectory->GetName() << '\n';
   ++gReadyCount;
   while(gWait) {};

   // Now thread_six has deleted the file and our gDirectory should no longer
   // points to it.
   current = gDirectory;
   std::cout << "t3.2 gDirectory : " << (void*)current << '\n';
   if (current == localFileA) {
      std::cerr << "ERROR: thread three gDirectory still points to a.root after its deletion\n";
      ++gErrorCount;
      return;
   }
}

void thread_four()
{
   while(!gFileA) {};
   TDirectory *localFileA = gFileA;
   gDirectory = gFileA;
   {
      TDirectory::TContext ctxt;
      ++gReadyCount;
      while(gWait) {};
   }
   // Now thread_six has deleted the file and our gDirectory should no longer
   // points to it.
   TDirectory *current = gDirectory;
   std::cout << "t4.2 gDirectory : " << (void*)current << '\n';
   if (current == localFileA) {
      std::cerr << "ERROR: thread four gDirectory still points to a.root after its deletion\n";
      ++gErrorCount;
      return;
   }
}

void thread_five()
{
   while(!gFileA) {};
   TDirectory *localFileA = gFileA;
   gDirectory = gFileA;
   {
      TDirectory::TContext ctxt{gROOT};
      ++gReadyCount;
      while(gWait) {};
   }
   // Now thread_six has deleted the file and our gDirectory should no longer
   // points to it.
   TDirectory *current = gDirectory;
   std::cout << "t5.2 gDirectory : " << (void*)current << '\n';
   if (current == localFileA) {
      std::cerr << "ERROR: thread five gDirectory still points to a.root after its deletion\n";
      ++gErrorCount;
      return;
   }
}

void thread_six()
{
   std::cerr << "thread six waits for thread three\n";
   while (gReadyCount < 3) {};
   std::cout << "t6.1 gDirectory : " << (void*)gDirectory << '\n';

   std::cerr << "thread six deletes a.root\n";
   auto localFileA = gFileA.exchange(nullptr);
   delete localFileA;
   std::cerr << "thread six completed deletion of a.root\n";
   gWait = false;
}

int tcontext_thread_hoping_rare_race()
{
   ROOT::EnableThreadSafety();

   std::cerr << "Testing race condition between TContext and gDirectory updates\n";

   std::thread t1(thread_one);
   std::thread t2(thread_two);
   t2.join();
   t1.join();
   if (gFileA || gFileB) {
      std::cerr << "gDirectory/TContext rare race test: One of the two file is not deleted\n";
      return 1;
   }
   return 0;
}

int tcontext_thread_hoping_gdirectory_update()
{
   ROOT::EnableThreadSafety();

   std::cerr << "Testing update of gDirectory upon file deletion\n";

   gWait = true;
   std::thread t3(thread_three);
   std::thread t4(thread_four);
   std::thread t5(thread_five);
   std::thread t6(thread_six);
   t6.join();
   t5.join();
   t4.join();
   t3.join();
   if (gFileA || gFileB) {
      std::cerr << "gDirectory update test: One of the two file is not deleted\n";
      return 1;
   }
   return gErrorCount;
}


int assert_tcontext_thread_hoping()
{
   return tcontext_thread_hoping_gdirectory_update()
      + tcontext_thread_hoping_rare_race();
}



