#if !defined(__CINT__) || defined(__MAKECINT__)
  #include <Riostream.h>
  #include "IlcCascadeVertexer.h"
  #include "TFile.h"
  #include "TKey.h"
  #include "TStopwatch.h"

  #include "IlcRun.h"
  #include "IlcMagF.h"
  #include "IlcESD.h"
  #include "IlcTracker.h"
  #include "IlcRunLoader.h"
#endif

Int_t IlcCascadeFindVertices(Int_t nev=5) {
   cerr<<"Looking for cascade vertices...\n";

   if (gIlc) {
      delete gIlc->GetRunLoader();
      delete gIlc;
      gIlc=0;
   }
   IlcRunLoader* rl = IlcRunLoader::Open("gilc.root");
   if (rl == 0x0) {
      cerr<<"IlcCascadeFindVertices.C : Can not open session RL=NULL"<< endl;
      return 1;
   }

   if (rl->LoadgIlc()) {
      cerr<<"IlcV0FindVertices.C : LoadgIlc returned error"<<endl;
      delete rl;
      return 3;
   }

   // Magnetic field
   IlcTracker::SetFieldMap(gIlc->Field(),1); // 1 means uniform magnetic field
   Double_t cuts[]={33.,    // max. allowed chi2
                    0.05,   // min. allowed V0 impact parameter 
                    0.008,  // window around the Lambda mass 
                    0.035,  // min. allowed bachelor's impact parameter 
                    0.10,   // max. allowed DCA between a V0 and a track
                    0.9985,// max. allowed cosine of the cascade pointing angle
                    0.9,    // min. radius of the fiducial volume
                    2.9     // max. radius of the fiducial volume
                   };
   TStopwatch timer;
   IlcCascadeVertexer *vertexer=new IlcCascadeVertexer(cuts);

   Int_t rc=0;
   if (nev>rl->GetNumberOfEvents()) nev=rl->GetNumberOfEvents();

   TFile *casf=TFile::Open("IlcESDcas.root","RECREATE");
   if ((!casf)||(!casf->IsOpen())) {
      cerr<<"Can't IlcESDcas.root !\n"; return 1;
   }
   TFile *v0f=TFile::Open("IlcESDv0.root");
   if ((!v0f)||(!v0f->IsOpen())) {
      cerr<<"Can't IlcESDv0.root !\n"; return 1;
   }

   TKey *key=0;
   TIter next(v0f->GetListOfKeys());
   for (Int_t i=0; i<nev; i++) {
     v0f->cd();
     if ((key=(TKey*)next())==0) break;
     cerr<<"Processing event number: "<<i<<endl;
     IlcESD *event=(IlcESD*)key->ReadObj();

     rc=vertexer->V0sTracks2CascadeVertices(event);

     if (rc==0) {
        Char_t ename[100]; 
        sprintf(ename,"%d",i);
        casf->cd();
        if (!event->Write(ename)) rc++;
     } 
     if (rc) {
        cerr<<"Something bad happened...\n";
     }
     delete event;
   }
   delete vertexer;
   timer.Stop(); timer.Print();
    
   v0f->Close();
   casf->Close();

   delete rl;

   return rc;
}
