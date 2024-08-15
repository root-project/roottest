// This is a test included with minimal editing from
// item ROOT-3594

#include "ROOT/TProcessExecutor.hxx"

#include "TClonesArray.h"
#include "TFile.h"
#include "TH1F.h"
#include "TRefArray.h"
#include "TTree.h"
#include "TSystem.h"

#include <iostream>

class ObjA : public TObject {
public:
   UInt_t    fObjAVal;
   TRefArray fObjB;
   void      Clear(Option_t *)
   {
      fObjAVal = 0;
      fObjB.Clear();
   }

   ClassDef(ObjA, 1);
};

class ObjB : public TObject {
public:
   UInt_t fObjBVal;
   void   Clear(Option_t *) { fObjBVal = 0; }

   ClassDef(ObjB, 1);
};

class Top : public TObject {
public:
   Top()
   {
      fObjAArray = new TClonesArray(ObjA::Class(), 10);
      fObjBArray = new TClonesArray(ObjB::Class(), 10);
   }
   ~Top()
   {
      delete fObjAArray;
      delete fObjBArray;
   }

   TClonesArray *fObjAArray; //->
   TClonesArray *fObjBArray; //->
   void          Clear(Option_t          */*opt*/ = "")
   {
      fObjAArray->Clear("C");
      fObjBArray->Clear("C");
   }

   ClassDef(Top, 1);
};

bool fpCompare(double val, double ref)
{
   return 0.00001 > fabs(val - ref);
}

const char *gTheFileName = "trefTest.root";

int createfile()
{
   Top  *atop = new Top();
   TFile afile(gTheFileName, "recreate");
   TTree tree("tree", "tree");

   tree.BranchRef();
   tree.Branch("top", atop);

   for (size_t j = 0; j < 2; j++) {
      Int_t saveNumber = TProcessID::GetObjectCount();
      for (size_t i = 0; i < 10; i++) {
         ObjA *a     = static_cast<ObjA *>(atop->fObjAArray->New(i));
         ObjB *b     = static_cast<ObjB *>(atop->fObjBArray->New(i));
         a->fObjAVal = i * 100 + j;
         b->fObjBVal = i * 10 + j * 2;
         a->fObjB.Add(b);
      }
      tree.Fill();
      atop->Clear();
      TProcessID::SetObjectCount(saveNumber);
   }
   tree.Write();
   afile.Close();
   return 0;
}

class CreateFileInProcessRAAI {
public:
   CreateFileInProcessRAAI()
   {
      // We need to create the file from another PID for the test to be effective
      // NOTE: TSystem::AccessPathName returns true if the file can NOT be accessed
      if (gSystem->AccessPathName(gTheFileName)) {
         ROOT::TProcessExecutor pe(1);
         std::vector<int> args = {0};
         pe.Map([](){return createfile();}, 1);
      }
   }
   ~CreateFileInProcessRAAI() { gSystem->Unlink(gTheFileName); }
};

int trefTest()
{

   CreateFileInProcessRAAI cfip_raai;

   TFile afile(gTheFileName);
   auto  tree = afile.Get<TTree>("tree");
   tree->Draw("((ObjB*)(fObjAArray.fObjB.At(0))).fObjBVal >> hist(100, 0, 100)");
   auto       hist     = (TH1F *)gDirectory->Get("hist");
   std::cout << hist << std::endl;
   const auto nEntries = hist->GetEntries();  // 20
   const auto mean     = hist->GetMean();     // 46
   const auto rms      = hist->GetRMS();      // 28.74021572639983
   const auto skewness = hist->GetSkewness(); // 0.05219694055188104
   const auto kurtosis = hist->GetKurtosis(); // -1.2194668103524087

   int retcode = 0;
   if (nEntries != 20) {
      retcode++;
      std::cerr << "Wrong number of entries: expected 20, obtained " << nEntries << std::endl;
   }
   if (!fpCompare(mean, 46.)) {
      retcode++;
      std::cerr << "Wrong mean: expected 46, obtained " << mean << std::endl;
   }
   if (!fpCompare(rms, 28.74021572639983)) {
      retcode++;
      std::cerr << "Wrong rms: expected 28.74021572639983, obtained " << rms << std::endl;
   }
   if (!fpCompare(skewness, 0.05219694055188104)) {
      retcode++;
      std::cerr << "Wrong skewness: expected 0.05219694055188104, obtained " << skewness << std::endl;
   }
   if (!fpCompare(kurtosis, -1.2194668103524087)) {
      retcode++;
      std::cerr << "Wrong kurtosis: expected -1.2194668103524087, obtained " << kurtosis << std::endl;
   }

   return retcode;
}
