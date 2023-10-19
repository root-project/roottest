#include "ns_privacy.hh"

int privacyInterp() {

  const char *names[] = {
    "privacy::test",
    "privacy::test1",
    "privacy::test2", 
    "privacy::test3",
    "privacy::test4"
  };

  for(auto name : names) {
    auto cl = TClass::GetClass(name);
    if (!cl) {
      std::cout << "Could not find class " << name << "\n";
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
