{
typedef unsigned long l_size_t; const int n1 = 1;
TString a1[n1];
#ifdef ClingWorkAroundPrintfIssues
fprintf(stderr,"a1 is %s %s\n",((l_size_t)a1) ? "not_null" : "null",((l_size_t)&(a1[0])) ? "not_null" : "null");
#else
fprintf(stdout,"a1 is %s %s\n",((l_size_t)a1) ? "not_null" : "null",((l_size_t)&(a1[0])) ? "not_null" : "null");
#endif
const int n2 = 2;
TString a2[n2];
#ifdef ClingWorkAroundPrintfIssues
fprintf(stderr,"a2 is %s %s\n",((l_size_t)a2) ? "not_null" : "null",((l_size_t)&(a2[0])) ? "not_null" : "null");
#else
fprintf(stdout,"a2 is %s %s\n",((l_size_t)a2) ? "not_null" : "null",((l_size_t)&(a2[0])) ? "not_null" : "null");
#endif
gROOT->ProcessLine(".g a1");
gROOT->ProcessLine(".g a2");
}
