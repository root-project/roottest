#ifndef privacy_hh
#define privacy_hh
#include <iostream>
#include <filesystem>
#include <tuple>

namespace fs = std::filesystem;

class test
{
public:
    test() = default;
    ~test() = default;

private:
    fs::path path; ///<!  
    fs::path persistent_path;
};

class test1 {
private:
   struct Inner { struct Most {}; };
   std::unique_ptr<Inner> unique_data;
   Inner *ptr_data;
};

class test2 {
private:
   test1 transient_data; ///<!
   test1 persistent_data;
};

class test3 {
private: 
   struct Inner {};
   std::tuple<int, Inner> persistent_data;
   std::tuple<int, int, Inner> transient_data; ///<!
};

class test4 {
private:
   test3 transient_data; ///<!
};

#endif
