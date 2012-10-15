void SETUP()
{
  // Load the ANALYSIS library
   gSystem->Load("libANALYSISilc");

   // Set the include paths
   gROOT->ProcessLine(".include ANALYSISilc");

   // Set our location, so that other packages can find us
   gSystem->Setenv("ANALYSISilc_INCLUDE", "ANALYSISilc");

   // Set our lib coordinates, so that other packages can link to us
   TString lib = TString::Format("-L%s -lANALYSISilc", gSystem->WorkingDirectory());
   gSystem->Setenv("ANALYSISilc_LIBS", lib.Data());
}
