#include <stdlib.h>

#include "Riostream.h"
#include "TROOT.h"
#include "TObject.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"

class TEventRAC : public TObject {

private:
   Int_t         fSize;
   Float_t      *fArray; //[fSize]

public:
   TEventRAC(Int_t size = 1000);
   TEventRAC(const TEventRAC& small);
   virtual ~TEventRAC();
   TEventRAC &operator=(const TEventRAC &small);

   void          Clear(Option_t *option ="");
   void          Build();
   void          DumpData();
   Int_t         GetSize() const { return fSize; }
   Float_t        *GetSmall() const { return fArray; }

   ClassDef(TEventRAC, 1);
};

TEventRAC::TEventRAC(Int_t size)
{
   fSize  = size;
   fArray = new Float_t[fSize];
   for(Int_t i=0;i<fSize;++i) {
//      if (i%6==0) fArray[i] = gRandom->Rndm(1);
      if (i%6==0) fArray[i] = Float_t(i);
      else fArray[i] = fArray[i-1];
   }
}

TEventRAC::TEventRAC(const TEventRAC& small)
{
   Float_t *intermediate = small.GetSmall();
   Int_t size = small.GetSize();
   fArray = new Float_t[size];
   for(int i=0;i<size;++i)
      fArray[i] = intermediate[i];
}

TEventRAC::~TEventRAC()
{
   Clear();
   delete fArray;
   fSize = 0;
}

void TEventRAC::Build()
{
   for(int i=0;i<fSize;++i) {
      if (i%6==0) fArray[i] = Float_t(i);
      else fArray[i] = fArray[i-1];
   }
}

void TEventRAC::DumpData()
{
   for(int i=0;i<fSize;++i)
      printf("entry[%d] = %f\n",i,fArray[i]);
}

void TEventRAC::Clear(Option_t * /*option*/)
{
   for(int i=0;i<fSize;++i)
      fArray[i] = 0;
}

void writeWithRAC()
{
   gROOT->SetRandomAccessCompression(1);
   Int_t nevent    = 1000;
   Int_t complevel = 6;
   Int_t read      = 0;
   Int_t rac       = 0;
   Int_t     bufsize;
   TFile     *file;
   TTree     *tree;
   TBranch   *branch;
   TEventRAC *eventr;
   file = new TFile("TWithRAC.root","RECREATE","TTree benchmark ROOT file");
   eventr = new TEventRAC();
   file->SetCompressionLevel(complevel);
   tree = new TTree("T","An example of a ROOT tree");
   tree->SetAutoSave(1000000000); // autosave when 1 Gbyte written
   tree->SetCacheSize(10000000);  // set a 10 MBytes cache (useless when writing local files)
   bufsize = 64000;
   TTree::SetBranchStyle(1);
   branch = tree->Branch("eventr", &eventr, bufsize,0);
   tree->BranchRef();
   for (int ev = 0; ev < nevent; ev++) {
      eventr->Build();
      tree->Fill();  //fill the tree
   }
   file = tree->GetCurrentFile(); //just in case we switched to a new file
   file->Write();
   file->Close();
}

void writeWoutRAC()
{
   gROOT->SetRandomAccessCompression(0);
   Int_t nevent    = 1000;
   Int_t complevel = 6;
   Int_t read      = 0;
   Int_t rac       = 0;
   Int_t     bufsize;
   TFile     *file;
   TTree     *tree;
   TBranch   *branch;
   TEventRAC *eventn;
   file = new TFile("TWoutRAC.root","RECREATE","TTree benchmark ROOT file");
   eventn = new TEventRAC();
   file->SetCompressionLevel(complevel);
   tree = new TTree("T","An example of a ROOT tree");
   tree->SetAutoSave(1000000000); // autosave when 1 Gbyte written
   tree->SetCacheSize(10000000);  // set a 10 MBytes cache (useless when writing local files)
   bufsize = 64000;
   TTree::SetBranchStyle(1);
   branch = tree->Branch("eventn", &eventn, bufsize,0);
   tree->BranchRef();
   for (int ev = 0; ev < nevent; ev++) {
      eventn->Build();
      tree->Fill();  //fill the tree
   }
   file = tree->GetCurrentFile(); //just in case we switched to a new file
   file->Write();
   file->Close();
}

void readWithRAC()
{
   TFile     *file   = 0;
   TTree     *tree   = 0;
   TBranch   *branch = 0;
   TEventRAC *eventr = 0;

   eventr = new TEventRAC();
   file  = new TFile("TWithRAC.root");
   tree   = (TTree*)file->Get("T");
   branch = tree->GetBranch("eventr");
   branch->SetAddress(&eventr);
   Int_t nevent = (Int_t)tree->GetEntries();
   Int_t cachesize = -1;
   tree->SetCacheSize(cachesize);
   tree->SetCacheLearnEntries(1);
   tree->SetCacheEntryRange(0,nevent);
   Int_t ev = Int_t(nevent*gRandom->Rndm(1));
   tree->LoadTree(ev);
   tree->GetEntry(ev);
   eventr->DumpData();
}

void readWoutRAC()
{
   TFile     *file   = 0;
   TTree     *tree   = 0;
   TBranch   *branch = 0;
   TEventRAC *eventn = 0;

   eventn = new TEventRAC();
   file  = new TFile("TWoutRAC.root");
   tree   = (TTree*)file->Get("T");
   branch = tree->GetBranch("eventn");
   branch->SetAddress(&eventn);
   Int_t nevent = (Int_t)tree->GetEntries();
   Int_t cachesize = -1;
   tree->SetCacheSize(cachesize);
   tree->SetCacheLearnEntries(1);
   tree->SetCacheEntryRange(0,nevent);
   Int_t ev = Int_t(nevent*gRandom->Rndm(1));
   tree->LoadTree(ev);
   tree->GetEntry(ev);
   eventn->DumpData();
}

void testRAC()
{
   writeWithRAC();
   readWithRAC();
   writeWoutRAC();
   readWoutRAC();
}
