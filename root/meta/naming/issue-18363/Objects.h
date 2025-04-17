#ifndef Objects_18363_hxx
#define Objects_18363_hxx

namespace reco { 
template <size_t I, bool B> struct PFRecHitSoALayout {
   template <size_t II, bool B1, bool B2, bool B3> struct ViewTemplateFreeParams {};
   using View = ViewTemplateFreeParams<I, B, true, true>;
};
using PFRecHitSoA = PFRecHitSoALayout<128,false>; // The result is the same with 128UL or 128ul
}

#endif
