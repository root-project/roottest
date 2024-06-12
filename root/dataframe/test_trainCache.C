#ifdef R__WIN32
R__LOAD_LIBRARY(libROOTDataFrame);
#endif

const auto treeName = "TotemNtuple";
const auto fileName = "bigFile.root";
const auto branchName = "track_rp_3.y";
const auto branchName2 = "track_rp_3.z";
const auto firstEvt = 10;
const auto lastEvt = 20;
const auto sumRef = 27.6706;
const auto epsilon = 0.001;
const auto refNBytes = 166;
const auto refNBytesRDF = 332;

class RBytesReadRAII
{
   unsigned int fInitBytes = 0U;
   TFile &fFile;
public:
   RBytesReadRAII(TChain &chain):fFile(*chain.GetFile()), fInitBytes(chain.GetFile()->GetBytesRead()){}
   unsigned int GetBytesRead() {return fFile.GetBytesRead() - fInitBytes;}
   ~RBytesReadRAII(){
      std::cout << " ====> Bytes Read: " << fFile.GetBytesRead() - fInitBytes << std::endl;
   }
};

int WithTChain()
{
   std::cout << "\n----------- With TChain" << std::endl;

   TChain chain(treeName);
   chain.Add(fileName);
   chain.SetMakeClass(1);

   chain.SetBranchStatus("*", false);
   chain.SetBranchStatus(branchName, true);

   chain.SetCacheEntryRange(firstEvt, lastEvt);
   chain.AddBranchToCache(branchName, true);
   chain.StopCacheLearningPhase();

   RBytesReadRAII rbRAII(chain);

   double y(0);
   double sum(0.);
   chain.SetBranchAddress(branchName, &y);

   for (auto i : ROOT::TSeqU(firstEvt, lastEvt)) {
      chain.GetEntry(i);
      sum += y;
   }

   std::cout << "Sum is " << sum << std::endl;

   int ret = 0;
   if (std::abs(sum - sumRef) > epsilon) {
      std::cout << "With TChain: the sum (" << sum
                << ") is too different from the reference (" << sumRef << ")\n";
      ret +=1;
   }

   chain.GetTree()->PrintCacheStats();

   if (refNBytes != rbRAII.GetBytesRead()) {
      std::cout << "With TChain: the bytes read (" << rbRAII.GetBytesRead()
                << ") are different from the reference (" << refNBytes << ")\n";
      ret +=1;
   }


   return ret;
}

int WithTTreeReader()
{
   std::cout << "\n----------- With TTreeReader" << std::endl;

   TChain chain(treeName);
   chain.Add(fileName);

   TTreeReader reader(&chain);
   reader.SetEntriesRange(firstEvt, lastEvt);
   TTreeReaderValue<double> vd(reader, branchName);
   RBytesReadRAII rbRAII(chain);

   double sum(0.);
   int index = 0;
   while(reader.Next()){
      sum += *vd;
   }

   std::cout << "Sum is " << sum << std::endl;

   int ret = 0;
   if (std::abs(sum - sumRef) > epsilon) {
      std::cout << "With WithTTreeReader: the sum (" << sum
                << ") is too different from the reference (" << sumRef << ")\n";
      ret +=1;
   }

   chain.GetTree()->PrintCacheStats();

   if (refNBytes != rbRAII.GetBytesRead()) {
      std::cout << "With WithTTreeReader: the bytes read (" << rbRAII.GetBytesRead()
                << ") are different from the reference (" << refNBytes << ")\n";
      ret +=1;
   }

   return ret;
}

int WithTTreeReaderAndTwoBranches()
{
   std::cout << "\n----------- With TTreeReaderAndTwoBranches" << std::endl;

   TChain chain(treeName);
   chain.Add(fileName);

   TTreeReader reader(&chain);

   // We read branch 2 to try to confuse the cache
   reader.SetEntriesRange(firstEvt, lastEvt);
   TTreeReaderValue<double> vd2(reader, branchName2);

   while(reader.Next()){
      *vd2;
   }

   // Now we restart, attach a new reader and read the branch
   reader.Restart();
   TTreeReaderValue<double> vd(reader, branchName);
   reader.SetEntriesRange(firstEvt, lastEvt);

   RBytesReadRAII rbRAII(chain);

   double sum(0.);
   int index = 0;
   while(reader.Next()){
      sum += *vd;
   }

   std::cout << "Sum is " << sum << std::endl;

   int ret = 0;
   if (std::abs(sum - sumRef) > epsilon) {
      std::cout << "With TTreeReaderAndTwoBranches: the sum (" << sum
                << ") is too different from the reference (" << sumRef << ")\n";
      ret +=1;
   }

   chain.GetTree()->PrintCacheStats();

   if (refNBytes != rbRAII.GetBytesRead()) {
      std::cout << "With TTreeReaderAndTwoBranches: the bytes read (" << rbRAII.GetBytesRead()
                << ") are different from the reference (" << refNBytes << ")\n";
      ret +=1;
   }

   return ret;
}

int WithRDF()
{
   std::cout << "\n----------- With RDataFrame" << std::endl;

   TChain chain(treeName);
   chain.Add(fileName);
//    chain.SetCacheEntryRange(firstEvt, lastEvt);

   RBytesReadRAII rbRAII(chain);

   const auto sum = *ROOT::RDataFrame(chain).Range(firstEvt, lastEvt).Sum<double>(branchName);

   std::cout << "Sum is " << sum << std::endl;

   int ret = 0;
   if (std::abs(sum - sumRef) > epsilon) {
      std::cout << "With RDF: the sum (" << sum
                << ") is too different from the reference (" << sumRef << ")\n";
      ret +=1;
   }

   chain.GetTree()->PrintCacheStats();

   if (refNBytesRDF != rbRAII.GetBytesRead()) {
      std::cout << "With RDF: the bytes read (" << rbRAII.GetBytesRead()
                << ") are different from the reference (" << refNBytes << ")\n";
      ret +=1;
   }

   return ret;

}

int test_trainCache() {

   int ret(0);
   ret += WithTChain();
   ret += WithTTreeReader();
   ret += WithTTreeReaderAndTwoBranches();
   ret += WithRDF();
   return ret;

}
