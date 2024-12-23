int  privacyLoad() {
  gSystem->Load("privacy");

  // On Windows <fstream> leads to a forward declaration of
  // std::filesystem::path and since we do not generate a dictonary for that
  // class, there is also no autoparsing.  This leads to a state that is
  // different from the state in other platform and incurred an additional
  // spurrious:
  // ```Warning in <TClass::Init>: no dictionary for class filesystem::path is available```
  // (due to the forward declaration only state).
  gInterpreter->Declare("#include <filesystem>");

  const char *names[] = {
    "test",
    "test1",
    "test2", 
    "test3",
    "test4"
  };

  for(auto name : names) {
    auto cl = TClass::GetClass(name);
    if (!cl) {
      std::cout << "Could not find class " << name << '\n';
      return 1;
    }
    std::cout << "Data member list for " << name << '\n';
    for(auto member : *cl->GetStreamerInfo()->GetElements()) {
      if (auto el = dynamic_cast<TStreamerElement*>(member)) {
         std::cout << "  " << el->GetTypeName();
         if (el->IsaPointer() && !TString(el->GetTypeName()).Contains("*"))
            std::cout << "*";
         std::cout << " " << el->GetName() << '\n';
      }
    }
    std::cout << '\n';
  }

  return 0;
}
