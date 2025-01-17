void ctrans(const char *filename)
{
  std::string fullpath{filename};

  // Add directory of the input file to the dynamic path so that the
  // library can be loaded.
  auto lastslash = fullpath.find_last_of("/");
  std::string directory{fullpath.substr(0, lastslash)};
  gSystem->AddDynamicPath(directory.c_str());
  TFile *_file0 = TFile::Open(filename);
  gSystem->Load("libTreeFormulaEvent");

  TTree *from = (TTree*) _file0->Get("T");
  auto f = new TFile("clone.root","RECREATE");
  from->CloneTree(-1,"fast");
  //to->Import(from);
  f->Write();
  delete _file0; delete f;
  gSystem->Exec(Form("mv clone.root %s",filename));
}
