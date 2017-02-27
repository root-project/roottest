#include "TFile.h"
#include "TTreeReader.h"
#include "TTreeReaderArray.h"
#define ARRAYHOLDER_STDARRAY
#include "arrayHolder.h"

void colWiseWrite_5() {
   auto file = TFile::Open("file_colWiseWrite_5.root", "RECREATE");
   auto tree = new TTree("mytree", "mytree");

   std::vector<MetaArrayHolder2> v_metaholder2;

   tree->Branch("mybranch", &v_metaholder2);
   for (int i=0;i<3;++i) {
      v_metaholder2.emplace_back(i,i+1,i+2);
      tree->Fill();
   }
   file->Write();
   file->Close();
}
