#include "TFile.h"
#include "TTree.h"
#include "TRandom.h"
#include "TSystem.h"
#include "TTreePerfStats.h"
#include "TVirtualStreamerInfo.h"
#include "TStreamerElement.h"
#include "TROOT.h"
#include "TStopwatch.h"
#include "TEnv.h"
#include "TFileCacheRead.h"
#include "TTreeCache.h"
#include "TError.h"
#include "TSystemDirectory.h"

/**
 * Test the local buffering code.
 * Basically we read through the file once and then look at the
 * file chunk statistics.
 * Largely adopted from the prefetch test
 */
int buffering()
{
   // Make sure we disable async prefetch and explicitly
   // enable buffering.
   gEnv->SetValue("TFile.AsyncPrefetching", 0);
   gEnv->SetValue("TFile.LocalBuffer", 1);

   // open the local if any
   TString filename("atlasFlushed.root");
   if (gSystem->AccessPathName(filename, kReadPermission) && filename.Index(":") == kNPOS) {
      // otherwise open the http file
      filename.Prepend("http://root.cern.ch/files/");
   }
   //filename = "atlasFlushed.root";
   
   TString library("atlasFlushed/atlasFlushed");
   fprintf(stderr, "Starting to load the library\n");
   gSystem->Load(library);

   fprintf(stderr,"Starting to open the file\n");
   TFile *file = TFile::Open(filename, "TIMEOUT=30");
   if (!file || file->IsZombie()) {
      Error("runPrefetchReading","Could not open the file %s within 30s", filename.Data());
      return 1;
   }
   fprintf(stderr,"The file has been opened, setting up the TTree\n");

   file->SetCacheRead(new TFileCacheRead(file, 0));

   // Try the known names :)
   const char *names [] = {"E", "Events", "CollectionTree", "ntuple", "T"};
   TTree *T = nullptr;
   for (unsigned int i = 0; i < sizeof(names)/sizeof(names[0]); ++i) {
      file->GetObject(names[i], T);
      if (T) break;
   }
   if (T==0) {
     Error("buffering","Could not find a tree which the conventional names in %s.",filename.Data());
     return 2;
   }

   Long64_t nentries = T->GetEntries();
   for (Long64_t i = 0; i < nentries; i++) {
       T->LoadTree(i);
       T->GetEntry(i);
   }
   unsigned blocks = file->GetCacheRead()->GetBufferedBlocks();

   fprintf(stderr, "fFileBlocks = %lu\n", blocks);

   file->Close();
   delete file;
   return (8-blocks);  // Correct number of blocks to cache is 8; return error if we saw a different #.
}
