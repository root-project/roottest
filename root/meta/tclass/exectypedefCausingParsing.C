void exectypedefCausingParsing(){
auto c = TClass::GetClass("theTD");
if (!c){
   std::cout << "ERROR: cannot get TClass for \"theTD\"\n";
   }
if (0 == gInterpreter->AutoParse("theTD")){
   std::cout << "ERROR: autoparsing should happen only at this point, not before.\n";
   }
}
