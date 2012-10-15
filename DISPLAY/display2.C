// This macro displays the hits belonging to a track for selected detectors
// Input: in the tracks contains the interesting tracks
//        ntracks is the number of interesing tracks
//        The default values correspond to "Show everything"
// Note: For the moment it works only with HIJING events, the PYTHIA is
//       still not supported
//#include <ClassTable.h>

void display2(const char *filename="gilc.root", Int_t nevent=0,
              Int_t *tracks=0, Int_t ntracks=0)
{
   // Dynamically link some shared libs
   if (gClassTable->GetID("IlcRun") < 0) {
      gROOT->LoadMacro("loadlibs.C");
      loadlibs();
   } else {
      delete gIlc->GetRunLoader();
      delete gIlc;
      gIlc = 0;
   }
   //gSystem->Load("libIlcL3Src");
   gSystem->Load("libDISPLAY");

   // Connect the ROOT Gilc file containing Geometry, Kine and Hits
   IlcRunLoader *rl = 0;
   TFile *file = (TFile*)gROOT->GetListOfFiles()->FindObject(filename);
   if(file) {
      Info("display2.C", "gilc.root is already open");
   }
   rl = IlcRunLoader::Open(filename, "DISPLAYED EVENT");

   if (rl == 0) {
      Error("display2.C", "can not get Run Loader, exiting...");
      return;
   }

   // Get IlcRun object from file or create it if not on file
   rl->LoadgIlc();

   gIlc = rl->GetIlcRun();
   if (!gIlc) {
      Error("display2.C", "IlcRun object not found on file, exiting...");
      return;
  }

   // Create Event Display object
   IlcDisplay2 *edisplay = new IlcDisplay2(gClient->GetRoot(), 900, 700);
   // if (ntracks > 0) edisplay->SetTracksToDisplay(tracks, ntracks);

   // Display the requested event
   rl->GetEvent(nevent);
   rl->LoadKinematics();
   rl->LoadHeader();
   rl->LoadHits();

   //edisplay->FindModules();
   edisplay->ShowNextEvent(0);
}



