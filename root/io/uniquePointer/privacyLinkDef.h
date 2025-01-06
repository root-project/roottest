#pragma link C++ class test+;

// We can generate the dictionary for this class even-though it has
// data members whose types is private.  There will be a clear error
// message if the user request the StreamerInfo for this class.
#pragma link C++ class test1+;

// Dictionary of private classes is not supported, uncommented this leads
// to the expected warning message explaining this.
// #pragma link C++ class test1::Inner+;

#pragma link C++ class test2+;
#pragma link C++ class test3+;
#pragma link C++ class test4+;

