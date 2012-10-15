void SETUP()
{

   // Load library
   gSystem->Load("libPVBARUtils");

   // Set the Include paths
//   gSystem->SetIncludePath("-I$ROOTSYS/include -IPVBAR");
   gROOT->ProcessLine(".include PVBARUtils");

   // Set our location, so that other packages can find us
   gSystem->Setenv("PVBARUtils_INCLUDE", "PVBARUtils");
}
