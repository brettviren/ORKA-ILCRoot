#if !defined(__CINT__) || defined(__MAKECINT__) 
  #include <Riostream.h>
  #include "IlcTARGETgeom.h"
  #include "IlcTARGET.h"
  #include "IlcTARGETtrackerSA.h"
  #include "IlcTARGETVertexerFast.h"
  #include "IlcRun.h"
  #include "IlcRunLoader.h"
  #include "IlcTPCLoader.h"
  #include "IlcTARGETLoader.h"
  #include "TStopwatch.h"
  #include "IlcMagF.h"
#endif

Int_t IlcTARGETFindTracksSA(Int_t evin=0,Int_t nevents=1,char *opt="onlyTARGET+6/6",const Char_t *clusterFileName="clusters.root", const Char_t *tracksFileName="TARGET.TracksSA.root") {

  //This macro finds tracks in the TARGET Stand Alone and writes them in
  //the file TARGET.TracksSA.root as tracks of class IlcTARGETtracksV2.

  //This macro needs both IlcTARGETRecPoint (to find the vertex) and 
  //IlcTARGETclusterV2 reconstructed points (track finding). Clusters V2
  //must be saved in a file with a different name from that of RecPoint.

  //Options: write onlyTARGET to track only with the TARGET
  //         without the option onlyTARGET combined tracking TPC+TARGET 
  //         and TARGET stand-alone will be performed
  //
  //         write 6/6 to accept only tracks with 6 clusters
  //         write 5/6 to accept tracks with 5 clusters good over 6

   
   if (gIlc) {
      delete gIlc->GetRunLoader();
      delete gIlc; 
      gIlc=0;
   }
 
   IlcRunLoader* rl = IlcRunLoader::Open("gilc.root");
   if (rl == 0x0) {
      cerr<<"IlcTARGETFindTracksSA.C : Can not open session RL=NULL"<< endl;
      return 3;
   }
   
   Int_t retval = rl->LoadgIlc();
   if (retval) {
      cerr<<"IlcTARGETFindTracksSA.C : LoadgIlc returned error"<<endl;
      delete rl;
      return 3;
   }
   
   retval = rl->LoadHeader();
   if (retval) {
      cerr<<"IlcTARGETFindTracksSA.C : LoadHeader returned error"<<endl;
      delete rl;
      return 3;
   }
   gIlc=rl->GetIlcRun();
       
   IlcTARGETLoader* itsl = (IlcTARGETLoader*)rl->GetLoader("TARGETLoader");
   if (itsl == 0x0) {
      cerr<<"IlcTARGETFindTracksSA.C : Can not get TARGET loader"<<endl;
      return 4;
   }
   
   IlcTARGET *iTS = (IlcTARGET*)gIlc->GetDetector("TARGET");
   if (!iTS) {
      cerr<<"IlcTARGETFindTracksSA.C : Can not find the TARGET detector !"<<endl;
      return 6;
   }
   IlcTARGETgeom *geom = iTS->GetTARGETgeom();
   IlcKalmanTrack::SetConvConst(1000/0.299792458/rl->GetIlcRun()->Field()->SolenoidField());
   
   TString choice(opt);
   Bool_t onlyTARGET=choice.Contains("onlyTARGET");

   TStopwatch timer;
  
   for(Int_t iev=evin;iev<nevents;iev++){
     rl->GetEvent(iev);
     itsl->LoadRecPoints();
     //IlcTARGETVertexerPPZ* vertexer = new IlcTARGETVertexerPPZ("vertici.root");
     Double_t smear[3]={0.0150,0.0150,0.0150};
     IlcTARGETVertexerFast* vertexer = new IlcTARGETVertexerFast(smear);
     IlcTARGETVertex* vert = vertexer->FindVertexForCurrentEvent(iev);
     IlcTARGETtrackerSA tracker(geom,vert);  
     tracker.SetEventNumber(iev);
     
     itsl->UnloadRecPoints();
     itsl->SetRecPointsFileName(clusterFileName);
     itsl->LoadRecPoints();
    
     if(onlyTARGET){
       itsl->SetTracksFileName(tracksFileName);
       itsl->LoadTracks("recreate");
              
       TTree* treec = (TTree*)itsl->TreeR();
       TTree *itsTree=itsl->TreeT();
       if (!itsTree) {
	 itsl->MakeTree("T");
	 itsTree=itsl->TreeT();
       }
            
       tracker.FindTracks(treec,itsTree,iev,opt);
       itsl->WriteTracks("OVERWRITE");
     } 
     if(!onlyTARGET){
       itsl->LoadTracks("read");
       TTree *treev2=(TTree*)itsl->TreeT();
       TTree* treec = (TTree*)itsl->TreeR();
       tracker.UseFoundTracksV2(iev,treev2,treec);
       itsl->UnloadTracks();
       itsl->SetTracksFileName(tracksFileName);
       itsl->LoadTracks("recreate");
       TTree *itsTree=itsl->TreeT();
       if (!itsTree) {
	 itsl->MakeTree("T");
	 itsTree=itsl->TreeT();
       }
       tracker.FindTracks(treec,itsTree,iev,opt);
       itsl->WriteTracks("OVERWRITE");
       
    }
     
   }
   timer.Stop(); timer.Print();   
   delete geom; 

  
   return 0;
}





