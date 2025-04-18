#include <TFile.h>
#include <TTree.h>

#include <memory>

#include "Event_v3.hxx"

int main()
{
   auto f = std::unique_ptr<TFile>(TFile::Open("root_test_autoptr.root"));
   auto t = (TTree *)f->Get("t");

   Event *event = nullptr;
   t->SetBranchAddress("event", &event);

   t->GetEntry(0);
   if (!event->fTrack || event->fTrack->fFoo != 1)
      return 1;

   t->GetEntry(1);
   if (event->fTrack)
      return 2;

   t->GetEntry(2);
   if (!event->fTrack || event->fTrack->fFoo != 3)
      return 3;

   return 0;
}
