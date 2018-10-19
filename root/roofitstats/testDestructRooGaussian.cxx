#include <random>
#include "RooDataSet.h"
#include "RooArgSet.h"
#include "RooRealVar.h"
#include "RooGaussian.h"
#include "RooPlot.h"

int main()
{
  RooRealVar mass("mass","",3.0,3.2);
  RooRealVar x("x","",3.0,3.2);
  RooRealVar sigma("sigma","",3.0,3.2);

  RooGaussian g("gaus", "", x, mass, sigma);
  auto f = mass.frame();
  //f->ResetBit(kCanDelete);
  g.plotOn(f);
  //  f->Draw();
  //delete f; 
}
