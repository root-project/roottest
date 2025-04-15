#include <TFile.h>
#include <TTree.h>

#include <memory>

#include "Event_v2.hxx"

int main()
{
   auto f = std::unique_ptr<TFile>(TFile::Open("root_test_autoptr.root", "RECREATE"));
   auto t = new TTree("t", "");

   Event event;
   t->Branch("event", &event);

   event.fTrack = new Track{1};
   t->Fill();
   delete event.fTrack.fRawPtr;

   event.fTrack = nullptr;
   t->Fill();
   delete event.fTrack.fRawPtr;

   event.fTrack = new Track{3};
   t->Fill();
   delete event.fTrack.fRawPtr;

   t->Write();
   f->Close();

   return 0;
}
