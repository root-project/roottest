#include "RooRealVar.h"
#include "RooGaussian.h"

int main()
{
  RooRealVar mass("mass","",3.0,3.2);
  RooRealVar x("x","",3.0,3.2);
  RooRealVar sigma("sigma","",3.0,3.2);

  RooGaussian g("gaus", "", x, mass, sigma);
  auto f = mass.frame();
  g.plotOn(f);
}
