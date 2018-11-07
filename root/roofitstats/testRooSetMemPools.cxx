#include "RooDataSet.h"
#include "RooArgSet.h"
#include <memory>
#include <vector>
#include <set>

/**
 * RooArgSets and RooDataSets are assumed to be unique in RooFit when their pointer is unique.
 * Therefore, they were using (broken: static destruction problems) custom allocators.
 * This test tests a memory pool that enforces the same uniqueness, but solves the static
 * destruction problems.
 *
 * When the memory pool becomes obsolete, so becomes this test. For this, RooArgSets need
 * to be identified using a unique ID instead of their pointer.
 */


typedef std::vector<std::unique_ptr<RooArgSet>>  ArgVec_t;
typedef std::vector<std::unique_ptr<RooDataSet>> DataVec_t;
std::set<void*> seenPtrs;

template <class Set_t>
void populate(std::vector<std::unique_ptr<Set_t>> & theVec, std::size_t n) {
  for (std::size_t i = 0; i < n; ++i) {
      theVec.emplace_back(new Set_t());

      auto success = seenPtrs.insert(theVec.back().get());
      if (! success.second) {
        std::cerr << theVec.back().get() << " doesn't seem to be unique." << std::endl;
        throw;
      }

    }
}

template <class T>
void testAllocs() {
    std::vector<std::unique_ptr<T>> theSets;
    theSets.reserve(10000);
    populate(theSets, 5000);
    std::cout << "Allocated " << theSets.size() << " sets." << std::endl;

    theSets.resize(2000);
    std::cout << "Down to " << theSets.size() << " sets." << std::endl;
    populate(theSets, 5000);
    std::cout << "Up to " << theSets.size() << " sets." << std::endl;


    std::srand(1337);
    theSets.erase(std::remove_if(theSets.begin(),
        theSets.end(),
        [](std::unique_ptr<T> &){return std::rand() % 20 < 18;}),
        theSets.end());
    std::cout << "Random deletions leave " << theSets.size() << " sets." << std::endl;

    populate(theSets, 5000);
    std::cout << "More allocations: " << theSets.size() << " sets." << std::endl;
    theSets.clear();
    std::cout << "Cleared." << std::endl;
    seenPtrs.clear();
    std::cout << "Done.\n\n" << std::endl;
}

int main()
{
  testAllocs<RooArgSet>();

  testAllocs<RooDataSet>();
}
