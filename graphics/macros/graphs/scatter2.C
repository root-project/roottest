/// \file
/// \ingroup tutorial_graphs
/// \notebook
/// Draw a scatter plot.
///
/// \macro_image
/// \macro_code
///
/// \author Olivier Couet

void scatter2()
{
   auto c1 = new TCanvas();
   gStyle->SetPalette(kBird, 0, 0.6); // define a transparent palette

   const int n = 100;
   double x[n];
   double y[n];
   double z[n];
   double c[n];
   double s[n];

   // Define four random data set
   auto r  = new TRandom();
   for (int i=0; i<n; i++) {
      x[i] = 100*r->Rndm(i);
      y[i] = 200*r->Rndm(i);
      z[i] = 10*r->Rndm(i);
      c[i] = 300*r->Rndm(i);
      s[i] = 400*r->Rndm(i);
   }

   auto scatter = new TScatter2D(n, x, y, z, c, s);
   scatter->SetMarkerStyle(20);
   scatter->SetTitle("Scatter plot;X;Y;Z");
   scatter->Draw("A");
}
