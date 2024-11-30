int numErrors = 0;

void check(const char *what) {
  if (!TClassTable::GetProto(what)) {
    std::cerr << "FAILED to get TProtoClass for \"" << what << "\"\n";
    ++numErrors;
  }
}

void check_not(const char *what) {
  if (TClassTable::GetProto(what)) {
    std::cerr << "FAILED to NOT get TProtoClass for \"" << what
              << "\" (expected nullptr but got != nullptr)\n";
    ++numErrors;
  }
}

int execROOT10804(const char *className) {
  if (gSystem->Load("libROOT10804_dictrflx") < 0) {
    std::cerr << "Failed to load ROOT10804Dict!\n";
    return 1;
  }
  if (className[0] == '!')
    check_not(className + 1);
  else
    check(className);

  return numErrors ? 1 : 0;
}
