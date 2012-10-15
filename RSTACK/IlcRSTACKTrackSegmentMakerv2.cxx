/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/
/* $Id: IlcRSTACKTrackSegmentMakerv2.cxx 47121 2011-02-04 16:08:46Z policheh $ */

/* History of cvs commits:
 *
 * $Log$
 * Revision 1.6  2007/08/22 09:20:50  hristov
 * Updated QA classes (Yves)
 *
 * Revision 1.5  2007/07/11 13:43:30  hristov
 * New class IlcESDEvent, backward compatibility with the old IlcESD (Christian)
 *
 * Revision 1.4  2007/05/04 14:49:29  policheh
 * IlcRSTACKRecPoint inheritance from IlcCluster
 *
 * Revision 1.3  2007/04/25 19:39:42  kharlov
 * Track extracpolation improved
 *
 * Revision 1.2  2007/04/01 19:16:52  kharlov
 * D.P.: Produce EMCTrackSegments using TPC/ITS tracks (no CPV)
 *
 *
 */

//_________________________________________________________________________
// Implementation version 2 of algorithm class to construct RSTACK track segments
// Track segment for RSTACK is list of 
//        EMC RecPoint + (possibly) projection of TPC track
// To find TrackSegments we do the following: 
//  for each EMC RecPoints we look at
//   TPC projections radius fRtpc. 
//  If there is such a track
//   we make "Link" it is just indexes of EMC and TPC track and distance
//   between them in the RSTACK plane. 
//  Then we sort "Links" and starting from the 
//   least "Link" pointing to the unassined EMC and TPC assing them to 
//   new TrackSegment. 
// If there is no TPC track we make TrackSegment 
// consisting from EMC alone. There is no TrackSegments without EMC RecPoint.
//// In principle this class should be called from IlcRSTACKReconstructor, but 
// one can use it as well in standalone mode.
// Use  case:
//  root [0] IlcRSTACKTrackSegmentMakerv2 * t = new IlcRSTACKTrackSegmentMaker("gilc.root", "tracksegmentsname", "recpointsname")
//  Warning in <TDatabasePDG::TDatabasePDG>: object already instantiated
//               // reads gIlc from header file "gilc.root", uses recpoints stored in the branch names "recpointsname" (default = "Default")
//               // and saves recpoints in branch named "tracksegmentsname" (default = "recpointsname")                       
//  root [1] t->ExecuteTask()
//  root [3] t->SetTrackSegmentsBranch("max distance 5 cm")
//  root [4] t->ExecuteTask("deb all time") 
//                 
//*-- Author: Dmitri Peressounko (RRC Ki & SUBATECH) & Yves Schutz (SUBATECH) 
//

// --- ROOT system ---
#include "TFile.h"
#include "TTree.h"
#include "TBenchmark.h"

// --- Standard library ---
#include "Riostream.h"
// --- IlcRoot header files ---
#include "IlcRSTACKGeometry.h"
#include "IlcRSTACKTrackSegmentMakerv2.h"
#include "IlcRSTACKTrackSegment.h"
#include "IlcRSTACKLink.h"
#include "IlcRSTACKEmcRecPoint.h"
#include "IlcRSTACKCpvRecPoint.h"

#include "IlcESDEvent.h"
#include "IlcESDtrack.h"

ClassImp( IlcRSTACKTrackSegmentMakerv2) 


//____________________________________________________________________________
IlcRSTACKTrackSegmentMakerv2::IlcRSTACKTrackSegmentMakerv2() :
  IlcRSTACKTrackSegmentMaker(),
  fDefaultInit(kTRUE),
  fWrite(kFALSE),
  fNTrackSegments(0),
  fRtpc(0.f),
  fVtx(0.,0.,0.),
  fLinkUpArray(0),
  fTPCtracks(),
  fEmcFirst(0),
  fEmcLast(0),
  fModule(0),
  fTrackSegments(NULL)
{
  // default ctor (to be used mainly by Streamer)

  for(Int_t i=0; i<5; i++)fNtpcTracks[i]=0 ;
  InitParameters() ; 
}

//____________________________________________________________________________
IlcRSTACKTrackSegmentMakerv2::IlcRSTACKTrackSegmentMakerv2(IlcRSTACKGeometry *geom) :
  IlcRSTACKTrackSegmentMaker(geom),
  fDefaultInit(kFALSE),
  fWrite(kFALSE),
  fNTrackSegments(0),
  fRtpc(0.f),
  fVtx(0.,0.,0.),
  fLinkUpArray(0),
  fTPCtracks(),
  fEmcFirst(0),
  fEmcLast(0),
  fModule(0),
  fTrackSegments(NULL)
{
  // ctor
  for(Int_t i=0; i<5; i++)fNtpcTracks[i]=0 ;

  InitParameters() ; 
  Init() ;
  fESD = 0;
}

//____________________________________________________________________________
IlcRSTACKTrackSegmentMakerv2::IlcRSTACKTrackSegmentMakerv2(const IlcRSTACKTrackSegmentMakerv2 & tsm) :
  IlcRSTACKTrackSegmentMaker(tsm),
  fDefaultInit(kFALSE),
  fWrite(kFALSE),
  fNTrackSegments(0),
  fRtpc(0.f),
  fVtx(0.,0.,0.),
  fLinkUpArray(0),
  fTPCtracks(),
  fEmcFirst(0),
  fEmcLast(0),
  fModule(0),
  fTrackSegments(NULL)
{
  // cpy ctor: no implementation yet
  // requested by the Coding Convention
  Fatal("cpy ctor", "not implemented") ;
}


//____________________________________________________________________________
 IlcRSTACKTrackSegmentMakerv2::~IlcRSTACKTrackSegmentMakerv2()
{ 
  // dtor
  // fDefaultInit = kTRUE if TrackSegmentMaker created by default ctor (to get just the parameters)
  if (!fDefaultInit)  
    delete fLinkUpArray ;
  if (fTrackSegments) {
    fTrackSegments->Delete();
    delete fTrackSegments;
  }
}

//____________________________________________________________________________
void  IlcRSTACKTrackSegmentMakerv2::FillOneModule()
{
  // Finds first and last indexes between which 
  // clusters from one RSTACK module are

  //First EMC clusters
  Int_t totalEmc = fEMCRecPoints->GetEntriesFast() ;
  for(fEmcFirst = fEmcLast; (fEmcLast < totalEmc) &&  
	((static_cast<IlcRSTACKRecPoint *>(fEMCRecPoints->At(fEmcLast)))->GetRSTACKMod() == fModule ); 
      fEmcLast ++)  ;
  
  //Now TPC tracks
  if(fESD){
    //Do it ones, only first time
    if(fModule==1){
      Int_t nTracks = fESD->GetNumberOfTracks();

      Int_t nRSTACKmod = fGeom->GetNModules() ;
      if(fTPCtracks[0].size()<(UInt_t)nTracks){
        for(Int_t i=0; i<nRSTACKmod; i++) 
          fTPCtracks[i].resize(nTracks) ;
      }
      for(Int_t i=0; i<5; i++)fNtpcTracks[i]=0 ;
      TVector3 inRSTACK ;
   
      //In this particular case we use fixed vertex position at zero
      Double_t vtx[3]={0.,0.,0.} ;
      IlcESDtrack *track;
      Double_t xyz[3] ;
      Double_t rEMC = fGeom->GetIPtoCrystalSurface() ; //Use here ideal geometry 
      for (Int_t iTrack=0; iTrack<nTracks; iTrack++) {
        track = fESD->GetTrack(iTrack);
        for(Int_t iTestMod=1; iTestMod<=nRSTACKmod; iTestMod++){
           Double_t modAngle=270.+fGeom->GetRSTACKAngle(iTestMod) ;
           modAngle*=TMath::Pi()/180. ;
           track->Rotate(modAngle);  
           if (!track->GetXYZAt(rEMC, fESD->GetMagneticField(), xyz))
             continue; //track coord on the cylinder of RSTACK radius
           if ((TMath::Abs(xyz[0])+TMath::Abs(xyz[1])+TMath::Abs(xyz[2]))<=0)
             continue;
           //Check if this track hits RSTACK
           inRSTACK.SetXYZ(xyz[0],xyz[1],xyz[2]);
           Int_t modNum ; 
           Double_t x,z ;
           fGeom->ImpactOnEmc(vtx, inRSTACK.Theta(), inRSTACK.Phi(), modNum, z, x) ;
           if(modNum==iTestMod){
             //Mark this track as one belonging to module
             TrackInRSTACK_t &t = fTPCtracks[modNum-1][fNtpcTracks[modNum-1]++] ; 
             t.track = track ;
             t.x = x ;
             t.z = z ;
             break ;
           }
        }
      }
    }
  } 

}

//____________________________________________________________________________
void  IlcRSTACKTrackSegmentMakerv2::GetDistanceInRSTACKPlane(IlcRSTACKEmcRecPoint * emcClu,
                                                         IlcESDtrack *track,
                                                         Float_t &dx, Float_t &dz) const
{
  // Calculates the distance between the EMC RecPoint and the CPV RecPoint
  // Clusters are sorted in "rows" and "columns" of width 1 cm

  TVector3 emcGlobal; // Global position of the EMC recpoint
  fGeom->GetGlobalRSTACK((IlcRSTACKRecPoint*)emcClu,emcGlobal);

  //Calculate actual distance to the RSTACK surface
  Double_t modAngle=270.+fGeom->GetRSTACKAngle(emcClu->GetRSTACKMod()) ;
  modAngle*=TMath::Pi()/180. ;
  Double_t rEMC = emcGlobal.X()*TMath::Cos(modAngle)+emcGlobal.Y()*TMath::Sin(modAngle) ;
  track->Rotate(modAngle);
  Double_t xyz[3] ;
  if(!track->GetXYZAt(rEMC, fESD->GetMagneticField(), xyz)){
    dx=999. ;
    dz=999. ;
    return ; //track coord on the cylinder of RSTACK radius
  }
  if((TMath::Abs(xyz[0])+TMath::Abs(xyz[1])+TMath::Abs(xyz[2]))<=0){
    dx=999. ; 
    dz=999. ;
    return ;
  } 

  dx=(emcGlobal.X()-xyz[0])*TMath::Sin(modAngle)-(emcGlobal.Y()-xyz[1])*TMath::Cos(modAngle) ;
  dx=TMath::Sign(dx,(Float_t)(emcGlobal.X()-xyz[0])) ; //set direction
  dz=emcGlobal.Z()-xyz[2] ;

  return ;
}
//____________________________________________________________________________
void  IlcRSTACKTrackSegmentMakerv2::Init()
{
  // Make all memory allocations that are not possible in default constructor
  
  fLinkUpArray  = new TClonesArray("IlcRSTACKLink", 1000); 
  fTrackSegments = new TClonesArray("IlcRSTACKTrackSegment",100);
}

//____________________________________________________________________________
void  IlcRSTACKTrackSegmentMakerv2::InitParameters()
{
  //Initializes parameters
  fRtpc      = 4. ;
  fEmcFirst  = 0 ;    
  fEmcLast   = 0 ;   
  fLinkUpArray = 0 ;
  fWrite                   = kTRUE ;
}


//____________________________________________________________________________
void  IlcRSTACKTrackSegmentMakerv2::MakeLinks()
{ 
  // Finds distances (links) between all EMC and CPV clusters, 
  // which are not further apart from each other than fRcpv 
  // and sort them in accordance with this distance
  
  fLinkUpArray->Clear() ;    

  IlcRSTACKEmcRecPoint * emcclu ;

  Int_t iLinkUp  = 0 ;
  
  Int_t iEmcRP;
  for(iEmcRP = fEmcFirst; iEmcRP < fEmcLast; iEmcRP++ ) {
    emcclu = static_cast<IlcRSTACKEmcRecPoint *>(fEMCRecPoints->At(iEmcRP)) ;
    TVector3 vecEmc ;
    emcclu->GetLocalPosition(vecEmc) ;
    Int_t mod=emcclu->GetRSTACKMod() ;
    for(Int_t itr=0; itr<fNtpcTracks[mod-1] ; itr++){
      TrackInRSTACK_t &t = fTPCtracks[mod-1][itr] ;
      //calculate raw distance
      Double_t rawdx = t.x - vecEmc.X() ;
      Double_t rawdz = t.z - vecEmc.Z() ;
      if(TMath::Sqrt(rawdx*rawdx+rawdz*rawdz)<fRtpc){
        Float_t dx,dz ;
        //calculate presize difference accounting misalignment
        GetDistanceInRSTACKPlane(emcclu, t.track, dx,dz) ;     
        Int_t itrack = t.track->GetID() ;
        new ((*fLinkUpArray)[iLinkUp++])  IlcRSTACKLink(dx, dz, iEmcRP, itrack, -1) ;
      }      
    }
  } 
  fLinkUpArray->Sort() ;  //first links with smallest distances
}

//____________________________________________________________________________
void  IlcRSTACKTrackSegmentMakerv2::MakePairs()
{ 
  // Using the previously made list of "links", we found the smallest link - i.e. 
  // link with the least distance between EMC and CPV and pointing to still 
  // unassigned RecParticles. We assign these RecPoints to TrackSegment and 
  // remove them from the list of "unassigned". 

  //Make arrays to mark clusters already chosen
  Int_t * emcExist = 0;

  if(fEmcLast > fEmcFirst)
    emcExist = new Int_t[fEmcLast-fEmcFirst] ;
  else
    IlcFatal(Form("fEmcLast > fEmcFirst: %d > %d is not true!",fEmcLast,fEmcFirst));
  
  Int_t index;
  for(index = 0; index <fEmcLast-fEmcFirst; index ++)
    emcExist[index] = 1 ;
  
  
  Bool_t * tpcExist = 0;
  Int_t nTracks = fESD->GetNumberOfTracks();
  if(nTracks>0)
    tpcExist = new Bool_t[nTracks] ;
  
  for(index = 0; index <nTracks; index ++)
    tpcExist[index] = 1 ;
  
  
  // Finds the smallest links and makes pairs of CPV and EMC clusters with smallest distance 
  TIter nextUp(fLinkUpArray) ;
  
  IlcRSTACKLink * linkUp ;
  
  IlcRSTACKCpvRecPoint * nullpointer = 0 ;
  
  while ( (linkUp =  static_cast<IlcRSTACKLink *>(nextUp()) ) ){  

    if(emcExist[linkUp->GetEmc()-fEmcFirst] != -1){

      if(tpcExist[linkUp->GetCpv()]){ //Track still exist
         Float_t dx,dz ; 
         linkUp->GetXZ(dx,dz) ;
	 new ((* fTrackSegments)[fNTrackSegments]) 
	   IlcRSTACKTrackSegment(static_cast<IlcRSTACKEmcRecPoint *>(fEMCRecPoints->At(linkUp->GetEmc())) , 
			       nullpointer,
			       linkUp->GetTrack(),dx,dz) ;
       (static_cast<IlcRSTACKTrackSegment *>(fTrackSegments->At(fNTrackSegments)))->SetIndexInList(fNTrackSegments);
       fNTrackSegments++ ;
       emcExist[linkUp->GetEmc()-fEmcFirst] = -1 ; //Mark emc  that Cpv was found 
       //mark track as already used 
       tpcExist[linkUp->GetCpv()] = kFALSE ;
      } //if CpvUp still exist
    } 
  }        

  //look through emc recPoints left without CPV
  if(emcExist){ //if there is emc rec point
    Int_t iEmcRP ;
    for(iEmcRP = 0; iEmcRP < fEmcLast-fEmcFirst  ; iEmcRP++ ){
      if(emcExist[iEmcRP] > 0 ){
       new ((*fTrackSegments)[fNTrackSegments])  
         IlcRSTACKTrackSegment(static_cast<IlcRSTACKEmcRecPoint *>(fEMCRecPoints->At(iEmcRP+fEmcFirst)), 
                           nullpointer) ;
       (static_cast<IlcRSTACKTrackSegment *>(fTrackSegments->At(fNTrackSegments)))->SetIndexInList(fNTrackSegments);
       fNTrackSegments++;    
      } 
    }
  }
  delete [] emcExist ; 
  delete [] tpcExist ; 
}

//____________________________________________________________________________
void  IlcRSTACKTrackSegmentMakerv2::Clusters2TrackSegments(Option_t *option)
{
  // Steering method to perform track segment construction for events
  // in the range from fFirstEvent to fLastEvent.
  // This range is optionally set by SetEventRange().
  // if fLastEvent=-1 (by default), then process events until the end.
  
  if(strstr(option,"tim"))
    gBenchmark->Start("RSTACKTSMaker");
 
  if(strstr(option,"print")) {
    Print() ; 
    return ; 
  }
  
  //Make some initializations 
  fNTrackSegments = 0 ;
  fEmcFirst = 0 ;    
  fEmcLast  = 0 ;   
    
  fTrackSegments->Clear();

  //    if(!ReadRecPoints(ievent))   continue; //reads RecPoints for event ievent
    
  for(fModule = 1; fModule <= fGeom->GetNModules() ; fModule++ ) {
    FillOneModule() ; 
    MakeLinks() ;
    MakePairs() ;
  }

  if(strstr(option,"deb"))
    PrintTrackSegments(option);
    
  if(strstr(option,"tim")){
    gBenchmark->Stop("RSTACKTSMaker");
    Info("Exec", "took %f seconds for making TS", 
	 gBenchmark->GetCpuTime("RSTACKTSMaker")); 
  }
}

//____________________________________________________________________________
void IlcRSTACKTrackSegmentMakerv2::Print(const Option_t *)const
{
  //  Print TrackSegmentMaker parameters

  TString message("") ;
  if( strcmp(GetName(), "") != 0 ) {
    message = "\n======== IlcRSTACKTrackSegmentMakerv2 ========\n" ; 
    message += "Making Track segments\n" ;
    message += "with parameters:\n" ; 
    message += "     Maximal EMC - TPC distance (cm) %f\n" ;
    message += "============================================\n" ;
    Info("Print", message.Data(),fRtpc) ;
  }
  else
    Info("Print", "IlcRSTACKTrackSegmentMakerv2 not initialized ") ;

}

//____________________________________________________________________________
void IlcRSTACKTrackSegmentMakerv2::PrintTrackSegments(Option_t * option)
{
  // option deb - prints # of found TrackSegments
  // option deb all - prints as well indexed of found RecParticles assigned to the TS

  Info("PrintTrackSegments", "Results from TrackSegmentMaker:") ; 
  printf("        Found %d TrackSegments\n", fTrackSegments->GetEntriesFast() ); 
  
  if(strstr(option,"all")) {  // printing found TS
    printf("TrackSegment #  EMC RP#  CPV RP#\n") ; 
    Int_t index;
    for (index = 0 ; index <fTrackSegments->GetEntriesFast() ; index++) {
      IlcRSTACKTrackSegment * ts = (IlcRSTACKTrackSegment * )fTrackSegments->At(index) ; 
      printf("   %d           %d        %d \n", ts->GetIndexInList(), ts->GetEmcIndex(), ts->GetTrackIndex() ) ; 
    }	
  }
}
