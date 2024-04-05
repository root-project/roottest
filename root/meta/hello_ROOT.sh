#! /bin/sh
printf '#include <iostream>\nvoid hello_ROOT() { std::cout << "Hello ROOT!" << std::endl; }' >> hello_ROOT.C
root -l -b -q hello_ROOT.C+
root -l -b -q hello_ROOT_C.so
rm hello_ROOT.C hello_ROOT_C.so
