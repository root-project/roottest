{
   TFile pcmfile("size_tTemplateDict_rdict.pcm");
   if (pcmfile.IsZombie()) {
      std::cerr << "Error: Failed to open root pcm file: " << pcmfile.GetName() << '\n';
      return 1;
   }
   TObjArray *protoArray = pcmfile.Get<TObjArray>("__ProtoClasses");
   if (!protoArray) {
      std::cerr << "Error: Failed to retrieve __ProtoClasses\n";
      return 2;
   }
   protoArray->Print();

   const char *classname = "reco::PFRecHitSoALayout<128,false>::ViewTemplateFreeParams<128,false,true,true>";
   auto cl = TClass::GetClass(classname);
   if (!cl) {
     std::cerr << "Error: Could not get the TClass for " << classname << "\n";
     return 3;
   }
   if (!cl->IsLoaded()) {
     std::cerr << "Error: The TClass is not loaded for " << classname << "\n";
     return 4;
   }
   
   return 0;
}
