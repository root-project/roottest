#ifdef __assertReload__
#define __assertReload__

{
#if !defined ClingWorkAroundUnloadingIOSTREAM
  gErrorIgnoreLevel = kError;
  gROOT->ProcessLine(".x ReloadScript.C++(\"Reload.root\")");
  gROOT->ProcessLine(".x ReloadScript.C++(\"Reload.root\")");
#endif
  return 0;
}

#endif
