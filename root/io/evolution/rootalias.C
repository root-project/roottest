int compile(int type, const char *what)
{
   TString work;
   TString mytype( what );
   mytype.ReplaceAll("longlong","long long");
   if (mytype[0] == 'u') {
      mytype = Form("unsigned %s",mytype.Data()+1);
   }
   work.Form(" -DCASE_%s \"-DMYTYPE=%s\" -DWHAT=\\\"%s\\\" ",what,mytype.Data(),what);
   gSystem->AddIncludePath( work );
   TString lib( Form("lib%s",what) );

   if (type==0) {
      return !gSystem->CompileMacro("../float16.cxx","k",lib);
   } else if (type==1) {
      return !gSystem->CompileMacro("../maptovector.cxx","k",lib);
   } else {
      return 0;
   }
}

int compile(const char *what)
{
   const char *stltypes[] = { "vec", "list", "map", "multimap" };
   for( int i = 0; i < sizeof(stltypes)/sizeof(char*); ++i ) {
      if (strcmp(what,stltypes[i])==0) {
         return compile(1,what);
      }
   }
   return compile(0,what);
}

void run(const char *what) {
   if (0 == compile(what))
#if defined(ClingWorkAroundMissingDynamicScope) || defined(__CLING__)
      gROOT->ProcessLine("write();");
#else
      write();
#endif
}
