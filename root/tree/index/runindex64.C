#include "TFile.h"
#include "TChain.h"
#include "Riostream.h"

bool test(TTree*);

const char* fname = "index64.root";
// Apple M1 has long double == double; these values exceed its range
// and cannot be represented as (even temporary) expression results.
// There would be a warning if you'd try.
static constexpr bool shortlongdouble = sizeof(long double) < 16; // was true for __APPLE__ and __arm64__
const Long64_t bigval   = shortlongdouble ? 0xFFFFFFFFFFFF : 0xFFFFFFFFFFFFFFF; // still positive number
const ULong64_t biguval = shortlongdouble ?  0xFFFFFFFFFFFF0 : 0xFFFFFFFFFFFFFFF0; // "negative" number
const Long64_t maxMajor = 0x1ffffffff;
const Long64_t maxMinor = 0x7fffffff; // unless major = 0

int runindex64(){

  ///////////////////////////////////////////////////
  // Make a tree and a file and write them to disk //
  ///////////////////////////////////////////////////

  TFile file(fname, "recreate");
  TTree *tree = new TTree("testTree", "my test tree");
  ULong64_t     run, event;
  // ULong64 is "l"
  tree->Branch("run", &run, "run/l");
  tree->Branch("event", &event, "event/l");

  ULong64_t   runs[] = { 8,5,5,5,      0,       0,  5,      4, 6, biguval,          1, maxMajor, maxMajor+1};
  ULong64_t events[] = { 0,1,3,2, bigval, biguval,  5, bigval, 3,  bigval, maxMinor+1, maxMinor, 0};
  for(int i=0; i<sizeof(events)/sizeof(*events); i++){
    run = runs[i];
    event = events[i];
    tree->Fill();
  }
  tree->Write();
  //tree->Scan("run:event","","colsize=30");

  cout<<"Tree BuildIndex returns "<<tree->BuildIndex("run", "event")<<endl;
  for (int i=0; i<sizeof(events)/sizeof(*events); i++) {
    run = runs[i];
    event = events[i];
    // in the cout, artificially convert to short longdouble for comparing with just one .ref instead of two based on the platforms
    ULong64_t     run2 = run;
    ULong64_t     event2 = event;
    if (!shortlongdouble) {
       if (run == bigval) run2 = 0xFFFFFFFFFFFF;
       else if(run == biguval) run2 = 0xFFFFFFFFFFFF0;
       if(event == bigval) event2 = 0xFFFFFFFFFFFF;
       else if(event == biguval) event2 = 0xFFFFFFFFFFFF0;
    }
    cout << i << ": Run " << run2 << ", Event " << event2 << " found at entry number: " << tree->GetEntryNumberWithIndex(run, event) << endl;
  }

  test(tree);
  file.Close();

  ////////////////////////////////////////////////////
  // Try loading  back in as a TChain               //
  ////////////////////////////////////////////////////
  TChain *chain = new TChain("testTree");
  chain->Add(fname);
  bool result = !test(chain);

  delete chain;

  return result;
  
}

bool test(TTree *chain)
{
  cout<<"Entries in chain: "<<chain->GetEntries()<<endl;
  cout<<"BuildIndex returns "<<chain->BuildIndex("run", "event")<<endl;
  cout<<"Try to get value that is not in the chain, this should return a -1:"<<endl;
  cout<<chain->GetEntryWithIndex(500)<<endl;
  cout<<"Try to get value that is in the chain, this should return a 4:"<<endl;
  cout<<(int)chain->GetEntryNumberWithIndex(0, bigval)<<endl;
  return (chain->GetEntryNumberWithIndex(500)==-1);
}
