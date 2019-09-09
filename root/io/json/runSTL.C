{
// Fill out the code of the actual test
#ifndef SECOND_RUN
   gROOT->ProcessLine(".L test_classes.h+");
#endif

#if defined(ClingWorkAroundMissingDynamicScope) && !defined(SECOND_RUN)
#define SECOND_RUN
   gROOT->ProcessLine(".x runSTL.C");
#else

   TJsonEx7 ex7; ex7.Init();
   TJsonEx8 ex8; ex8.Init();
   TJsonEx12 ex12; ex12.Init();
   TJsonEx13 ex13; ex13.Init();
   TJsonEx15 ex15; ex15.Init();
   TString json;

   cout << " ====== different STL containers TJsonEx7 ===== " << endl;
   json = TBufferJSON::ToJSON(&ex7);
   testJsonReading(json);
   cout << json << endl << endl;
   cout << " ====== STL as base class TJsonEx8 ===== " << endl;
   json = TBufferJSON::ToJSON(&ex8);
   testJsonReading(json);
   cout << json << endl << endl;
   cout << " ====== STL vector of complex classes TJsonEx12 ===== " << endl;
   json = TBufferJSON::ToJSON(&ex12);
   testJsonReading(json);
   cout << json << endl << endl;
   cout << " ====== More exotic STL containers as set or map with TRef inside TJsonEx13 ===== " << endl;
   json = TBufferJSON::ToJSON(&ex13);
   testJsonReading(json);
   cout << json << endl << endl;
   cout << " ====== Store derived from vector class as vector TJsonEx15 ===== " << endl;
   json = TBufferJSON::ToJSON(&ex15);
   auto res15 = TBufferJSON::FromJSON<TJsonEx15>(json.Data());
   TString json2 = TBufferJSON::ToJSON(res15.get());
   printf("TJsonEx15 store/read/store %s len1:%d len2:%d\n", json == json2 ? "MATCHED" : "FAILED", json.Length(), json2.Length());
   cout << json << endl << endl;

#endif
#ifdef ClingWorkAroundBrokenUnnamedReturn
   gApplication->Terminate(0);
#else
   return 0;
#endif
}
