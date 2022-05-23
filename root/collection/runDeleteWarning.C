{
   // Avoid actually accessing deleted memory but keeping the side-effect
   // by replacing new/delete by new-with-placement/destructor.
   auto slab = new char[2 * sizeof(TObject)];
   auto o = new (slab) TObject();
   auto l = new TList();
   l->SetName("my own list");
   l->Add(o);
   o->~TObject(); // instead of delete o;
   l->Clear();
   o = new (slab + sizeof(TObject)) TObject();
   l->Add(o);
   o->~TObject(); // instead delete o;
   l->Delete();
   return 0;
}
