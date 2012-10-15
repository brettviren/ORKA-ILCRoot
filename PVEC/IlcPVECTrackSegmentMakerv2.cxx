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
/* $Id: IlcPVECTrackSegmentMakerv2.cxx 47121 2011-02-04 16:08:46Z policheh $ */

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
 * IlcPVECRecPoint inheritance from IlcCluster
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
// Implementation version 2 of algorithm class to construct PVEC track segments
// Track segment for PVEC is list of 
//        EMC RecPoint + (possibly) projection of TPC track
// To find TrackSegments we do the following: 
//  for each EMC RecPoints we look at
//   TPC projections radius fRtpc. 
//  If there is such a track
//   we make "Link" it is just indexes of EMC and TPC track and distance
//   between them in the PVEC plane. 
//  Then we sort "Links" and starting from the 
//   least "Link" pointing to the unassined EMC and TPC assing them to 
//   new TrackSegment. 
// If there is no TPC track we make TrackSegment 
// consisting from EMC alone. There is no TrackSegments without EMC RecPoint.
//// In principle this class should be called from IlcPVECReconstructor, but 
// one can use it as well in standalone mode.
// Use  case:
//  root [0] IlcPVECTrackSegmentMakerv2 * t = new IlcPVECTrackSegmentMaker("gilc.root", "tracksegmentsname", "recpointsname")
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
#include "IlcPVECGeometry.h"
#include "IlcPVECTrackSegmentMakerv2.h"
#include "IlcPVECTrackSegment.h"
#include "IlcPVECLink.h"
#include "IlcPVECEmcRecPoint.h"
#include "IlcPVECCpvRecPoint.h"

#include "IlcESDEvent.h"
#include "IlcESDtrack.h"

ClassImp( IlcPVECTrackSegmentMakerv2) 


//____________________________________________________________________________
IlcPVECTrackSegmentMakerv2::IlcPVECTrackSegmentMakerv2() :
  IlcPVECTrackSegmentMaker(),
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
IlcPVECTrackSegmentMakerv2::IlcPVECTrackSegmentMakerv2(IlcPVECGeometry *geom) :
  IlcPVECTrackSegmentMaker(geom),
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
IlcPVECTrackSegmentMakerv2::IlcPVECTrackSegmentMakerv2(const IlcPVECTrackSegmentMakerv2 & tsm) :
  IlcPVECTrackSegmentMaker(tsm),
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
 IlcPVECTrackSegmentMakerv2::~IlcPVECTrackSegmentMakerv2()
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
void  IlcPVECTrackSegmentMakerv2::FillOneModule()
{
  // Finds first and last indexes between which 
  // clusters from one PVEC module are

  //First EMC clusters
  Int_t totalEmc = fEMCRecPoints->GetEntriesFast() ;
  for(fEmcFirst = fEmcLast; (fEmcLast < totalEmc) &&  
	((static_cast<IlcPVECRecPoint *>(fEMCRecPoints->At(fEmcLast)))->GetPVECMod() == fModule ); 
      fEmcLast ++)  ;
  
  //Now TPC tracks
  if(fESD){
    //Do it ones, only first time
    if(fModule==1){
      Int_t nTracks = fESD->GetNumberOfTracks();

      Int_t nPVECmod = fGeom->GetNModules() ;
      if(fTPCtracks[0].size()<(UInt_t)nTracks){
        for(Int_t i=0; i<nPVECmod; i++) 
          fTPCtracks[i].resize(nTracks) ;
      }
      for(Int_t i=0; i<5; i++)fNtpcTracks[i]=0 ;
      TVector3 inPVEC ;
   
      //In this particular case we use fixed vertex position at zero
      Double_t vtx[3]={0.,0.,0.} ;
      IlcESDtrack *track;
      Double_t xyz[3] ;
      Double_t rEMC = fGeom->GetIPtoCrystalSurface() ; //Use here ideal geometry 
      for (Int_t iTrack=0; iTrack<nTracks; iTrack++) {
        track = fESD->GetTrack(iTrack);
        for(Int_t iTestMod=1; iTestMod<=nPVECmod; iTestMod++){
           Double_t modAngle=270.+fGeom->GetPVECAngle(iTestMod) ;
           modAngle*=TMath::Pi()/180. ;
           track->Rotate(modAngle);  
           if (!track->GetXYZAt(rEMC, fESD->GetMagneticField(), xyz))
             continue; //track coord on the cylinder of PVEC radius
           if ((TMath::Abs(xyz[0])+TMath::Abs(xyz[1])+TMath::Abs(xyz[2]))<=0)
             continue;
           //Check if this track hits PVEC
           inPVEC.SetXYZ(xyz[0],xyz[1],xyz[2]);
           Int_t modNum ; 
           Double_t x,z ;
           fGeom->ImpactOnEmc(vtx, inPVEC.Theta(), inPVEC.Phi(), modNum, z, x) ;
           if(modNum==iTestMod){
             //Mark this track as one belonging to module
             TrackInPVEC_t &t = fTPCtracks[modNum-1][fNtpcTracks[modNum-1]++] ; 
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
void  IlcPVECTrackSegmentMakerv2::GetDistanceInPVECPlane(IlcPVECEmcRecPoint * emcClu,
                                                         IlcESDtrack *track,
                                                         Float_t &dx, Float_t &dz) const
{
  // Calculates the distance between the EMC RecPoint and the CPV RecPoint
  // Clusters are sorted in "rows" and "columns" of width 1 cm

  TVector3 emcGlobal; // Global position of the EMC recpoint
  fGeom->GetGlobalPVEC((IlcPVECRecPoint*)emcClu,emcGlobal);

  //Calculate actual distance to the PVEC surface
  Double_t modAngle=270.+fGeom->GetPVECAngle(emcClu->GetPVECMod()) ;
  modAngle*=TMath::Pi()/180. ;
  Double_t rEMC = emcGlobal.X()*TMath::Cos(modAngle)+emcGlobal.Y()*TMath::Sin(modAngle) ;
  track->Rotate(modAngle);
  Double_t xyz[3] ;
  if(!track->GetXYZAt(rEMC, fESD->GetMagneticField(), xyz)){
    dx=999. ;
    dz=999. ;
    return ; //track coord on the cylinder of PVEC radius
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
void  IlcPVECTrackSegmentMakerv2::Init()
{
  // Make all memory allocations that are not possible in default constructor
  
  fLinkUpArray  = new TClonesArray("IlcPVECLink", 1000); 
  fTrackSegments = new TClonesArray("IlcPVECTrackSegment",100);
}

//____________________________________________________________________________
void  IlcPVECTrackSegmentMakerv2::InitParameters()
{
  //Initializes parameters
  fRtpc      = 4. ;
  fEmcFirst  = 0 ;    
  fEmcLast   = 0 ;   
  fLinkUpArray = 0 ;
  fWrite                   = kTRUE ;
}


//____________________________________________________________________________
void  IlcPVECTrackSegmentMakerv2::MakeLinks()
{ 
  // Finds distances (links) between all EMC and CPV clusters, 
  // which are not further apart from each other than fRcpv 
  // and sort them in accordance with this distance
  
  fLinkUpArray->Clear() ;    

  IlcPVECEmcRecPoint * emcclu ;

  Int_t iLinkUp  = 0 ;
  
  Int_t iEmcRP;
  for(iEmcRP = fEmcFirst; iEmcRP < fEmcLast; iEmcRP++ ) {
    emcclu = static_cast<IlcPVECEmcRecPoint *>(fEMCRecPoints->At(iEmcRP)) ;
    TVector3 vecEmc ;
    emcclu->GetLocalPosition(vecEmc) ;
    Int_t mod=emcclu->GetPVECMod() ;
    for(Int_t itr=0; itr<fNtpcTracks[mod-1] ; itr++){
      TrackInPVEC_t &t = fTPCtracks[mod-1][itr] ;
      //calculate raw distance
      Double_t rawdx = t.x - vecEmc.X() ;
      Double_t rawdz = t.z - vecEmc.Z() ;
      if(TMath::Sqrt(rawdx*rawdx+rawdz*rawdz)<fRtpc){
        Float_t dx,dz ;
        //calculate presize difference accounting misalignment
        GetDistanceInPVECPlane(emcclu, t.track, dx,dz) ;     
        Int_t itrack = t.track->GetID() ;
        new ((*fLinkUpArray)[iLinkUp++])  IlcPVECLink(dx, dz, iEmcRP, itrack, -1) ;
      }      
    }
  } 
  fLinkUpArray->Sort() ;  //first links with smallest distances
}

//____________________________________________________________________________
void  IlcPVECTrackSegmentMakerv2::MakePairs()
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
  
  IlcPVECLink * linkUp ;
  
  IlcPVECCpvRecPoint * nullpointer = 0 ;
  
  while ( (linkUp =  static_cast<IlcPVECLink *>(nextUp()) ) ){  

    if(emcExist[linkUp->GetEmc()-fEmcFirst] != -1){

      if(tpcExist[linkUp->GetCpv()]){ //Track still exist
         Float_t dx,dz ; 
         linkUp->GetXZ(dx,dz) ;
	 new ((* fTrackSegments)[fNTrackSegments]) 
	   IlcPVECTrackSegment(static_cast<IlcPVECEmcRecPoint *>(fEMCRecPoints->At(linkUp->GetEmc())) , 
			       nullpointer,
			       linkUp->GetTrack(),dx,dz) ;
       (static_cast<IlcPVECTrackSegment *>(fTrackSegments->At(fNTrackSegments)))->SetIndexInList(fNTrackSegments);
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
         IlcPVECTrackSegment(static_cast<IlcPVECEmcRecPoint *>(fEMCRecPoints->At(iEmcRP+fEmcFirst)), 
                           nullpointer) ;
       (static_cast<IlcPVECTrackSegment *>(fTrackSegments->At(fNTrackSegments)))->SetIndexInList(fNTrackSegments);
       fNTrackSegments++;    
      } 
    }
  }
  delete [] emcExist ; 
  delete [] tpcExist ; 
}

//____________________________________________________________________________
void  IlcPVECTrackSegmentMakerv2::Clusters2TrackSegments(Option_t *option)
{
  // Steering method to perform track segment construction for events
  // in the range from fFirstEvent to fLastEvent.
  // This range is optionally set by SetEventRange().
  // if fLastEvent=-1 (by default), then process events until the end.
  
  if(strstr(option,"tim"))
    gBenchmark->Start("PVECTSMaker");
 
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
    gBenchmark->Stop("PVECTSMaker");
    Info("Exec", "took %f seconds for making TS", 
	 gBenchmark->GetCpuTime("PVECTSMaker")); 
  }
}

//____________________________________________________________________________
void IlcPVECTrackSegmentMakerv2::Print(const Option_t *)const
{
  //  Print TrackSegmentMaker parameters

  TString message("") ;
  if( strcmp(GetName(), "") != 0 ) {
    message = "\n======== IlcPVECTrackSegmentMakerv2 ========\n" ; 
    message += "Making Track segments\n" ;
    message += "with parameters:\n" ; 
    message += "     Maximal EMC - TPC distance (cm) %f\n" ;
    message += "============================================\n" ;
    Info("Print", message.Data(),fRtpc) ;
  }
  else
    Info("Print", "IlcPVECTrackSegmentMakerv2 not initialized ") ;

}

//____________________________________________________________________________
void IlcPVECTrackSegmentMakerv2::PrintTrackSegments(Option_t * option)
{
  // option deb - prints # of found TrackSegments
  // option deb all - prints as well indexed of found RecParticles assigned to the TS

  Info("PrintTrackSegments", "Results from TrackSegmentMaker:") ; 
  printf("        Found %d TrackSegments\n", fTrackSegments->GetEntriesFast() ); 
  
  if(strstr(option,"all")) {  // printing found TS
    printf("TrackSegment #  EMC RP#  CPV RP#\n") ; 
    Int_t index;
    for (index = 0 ; index <fTrackSegments->GetEntriesFast() ; index++) {
      IlcPVECTrackSegment * ts = (IlcPVECTrackSegment * )fTrackSegments->At(index) ; 
      printf("   %d           %d        %d \n", ts->GetIndexInList(), ts->GetEmcIndex(), ts->GetTrackIndex() ) ; 
    }	
  }
}
