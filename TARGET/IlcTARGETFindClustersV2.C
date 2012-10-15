/****************************************************************************
 *           Origin: I.Belikov, CERN, Jouri.Belikov@cern.ch                 *
 ****************************************************************************/

#if !defined(__CINT__) || defined(__MAKECINT__)
  #include <Riostream.h>

  #include "IlcRun.h"
  #include "IlcRunLoader.h"
  #include "IlcTARGETLoader.h"
  #include "IlcTARGET.h"
  #include "IlcTARGETgeom.h"
  #include "IlcTARGETclustererV2.h"

  #include "TTree.h"
  #include "TStopwatch.h"
#endif

extern IlcRun *gIlc;

Int_t IlcTARGETFindClustersV2(Int_t nev=5, Char_t SlowOrFast='s') {

   cerr<<"Looking for clusters...\n";

   if (gIlc) {
      delete gIlc->GetRunLoader();
      delete gIlc; 
      gIlc=0;
   }
 
   IlcRunLoader *rl = IlcRunLoader::Open("gilc.root");
   if (rl == 0x0) {
      cerr<<"IlcTARGETFindClustersV2.C : Can not open session RL=NULL"<< endl;
      return 1;
   }
     
   IlcTARGETLoader *itsl = (IlcTARGETLoader*)rl->GetLoader("TARGETLoader");
   if (itsl == 0x0) {
      cerr<<"IlcTARGETFindClustersV2.C : can not get TARGET loader"<< endl;
      return 2;
   }

   rl->LoadKinematics();

   Int_t retval = rl->LoadgIlc();
   if (retval) {
      cerr<<"IlcTARGETFindClustersV2.C : LoadgIlc returned error"<< endl;
      delete rl;
      return 3;
   }

   gIlc=rl->GetIlcRun();
   IlcTARGET *TARGET  = (IlcTARGET*)gIlc->GetModule("TARGET");
   if (!TARGET) { cerr<<"Can't find the TARGET !\n"; delete rl; return 3; }
   IlcTARGETgeom *geom=TARGET->GetTARGETgeom();

   itsl->LoadRecPoints("recreate");
   if (SlowOrFast=='s') itsl->LoadDigits("read");
   else itsl->LoadHits("read");
   
   if(SlowOrFast=='s'){
     IlcTARGETclustererV2 clusterer(geom);

     TStopwatch timer;
     if (nev>rl->GetNumberOfEvents()) nev=rl->GetNumberOfEvents();
     for (Int_t i=0; i<nev; i++) {
       cerr<<"Processing event number: "<<i<<endl;
       rl->GetEvent(i);
       
       TTree *out=itsl->TreeR();
       if (!out) {
	 itsl->MakeTree("R");
	 out=itsl->TreeR();
       }
       
       TTree *in=itsl->TreeD();
       if (!in) {
	 cerr<<"Can't get digits tree !\n";
	 return 4;
       }
       clusterer.Digits2Clusters(in,out);       
       itsl->WriteRecPoints("OVERWRITE");
       timer.Stop(); timer.Print();
     }

   } else{
     
     for(Int_t i=0;i<3;i++){
       TARGET->SetSimulationModel(i,new IlcTARGETsimulationFastPoints());
     }

     TStopwatch timer;
     for (Int_t i=0; i<nev; i++) {
       rl->GetEvent(i);
       if(itsl->TreeR()== 0x0) itsl->MakeTree("R");
       TTree* in = (TTree*)itsl->TreeH();
       TTree* out= (TTree*)itsl->TreeR();
       timer.Start();
       TARGET->Hits2Clusters(in,out);
       timer.Stop(); timer.Print();
       itsl->WriteRecPoints("OVERWRITE");
     }
   }

   delete rl;

   return 0;
}



