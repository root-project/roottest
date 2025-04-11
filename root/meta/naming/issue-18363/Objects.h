#ifndef Objects_18363_hxx
#define Objects_18363_hxx

namespace reco { 
template <size_t I, bool B> struct PFRecHitSoALayout {
  template <size_t II, bool B1, bool B2, bool B3> struct ViewTemplateFreeParams {};
  using View = ViewTemplateFreeParams<I, B, true, true>; 
}; 
using PFRecHitSoA = PFRecHitSoALayout<128UL,false>; // try with 128ul
}

static reco::PFRecHitSoA abc;
static reco::PFRecHitSoALayout<128UL,false> a2;

#endif
