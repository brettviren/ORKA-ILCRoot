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
/* $Id: IlcPVECTracker.cxx 43671 2010-09-19 18:45:08Z kharlov $ */

/* History of cvs commits:
 *
 * $Log$
 * Revision 1.9  2007/10/10 09:05:10  schutz
 * Changing name QualAss to QA
 *
 * Revision 1.8  2007/08/28 12:55:08  policheh
 * Loaders removed from the reconstruction code (C.Cheshkov)
 *
 * Revision 1.7  2007/08/07 14:12:03  kharlov
 * Quality assurance added (Yves Schutz)
 *
 * Revision 1.6  2007/08/03 14:41:37  cvetan
 * Missing header files
 *
 * Revision 1.5  2007/08/03 13:52:16  kharlov
 * Working skeleton of matching the ESD tracks and ESD clusters (Iouri Belikov)
 *
 */

#include <TClonesArray.h>
#include <TMath.h>

#include <IlcLog.h>
#include "IlcPVECTracker.h"
#include "IlcPVECEmcRecPoint.h"
#include "IlcPVECGeometry.h"
#include "IlcESDEvent.h"
#include "IlcESDtrack.h"
#include "IlcPVECTrackSegmentMakerv1.h"
#include "IlcPVECPIDv1.h"

//-------------------------------------------------------------------------
//                          PVEC tracker.
// Matches ESD tracks with the PVEC and makes the PID.  
//
//-------------------------------------------------------------------------

ClassImp(IlcPVECTracker)

Bool_t IlcPVECTracker::fgDebug = kFALSE ;  

//____________________________________________________________________________
IlcPVECTracker::IlcPVECTracker(): 
  IlcTracker()
{
  //--------------------------------------------------------------------
  // The default constructor
  //--------------------------------------------------------------------
  for (Int_t i=0; i<5; i++) 
      fModules[i]=new TClonesArray("IlcPVECEmcRecPoint",777);

}

//____________________________________________________________________________
IlcPVECTracker::~IlcPVECTracker() 
{
  //--------------------------------------------------------------------
  // The destructor
  //--------------------------------------------------------------------
  for (Int_t i=0; i<5; i++) {
      (fModules[i])->Delete();
      delete fModules[i];
  }
}

//____________________________________________________________________________
Int_t IlcPVECTracker::LoadClusters(TTree *cTree) {
  //--------------------------------------------------------------------
  // This function loads the PVEC clusters
  //--------------------------------------------------------------------

  TObjArray *arr=NULL;
  TBranch *branch=cTree->GetBranch("PVECEmcRP");
  if (branch==0) {
    IlcError("No branch with the EMC clusters found !");
    return 1;
  }
  branch->SetAddress(&arr);

  for(Int_t m=0;m<5; m++) fModules[m]->Clear("C") ;

  Int_t nclusters=0;
  Int_t nentr=(Int_t)branch->GetEntries();
  for (Int_t i=0; i<nentr; i++) {
    if (!branch->GetEvent(i)) continue;
    Int_t ncl=arr->GetEntriesFast();
    while (ncl--) {
      IlcPVECEmcRecPoint *cl=(IlcPVECEmcRecPoint*)arr->UncheckedAt(ncl);

      Int_t m=cl->GetPVECMod();
      if ((m<1)||(m>5)) {
         IlcError(Form("Wrong module index: %d !",m));
         continue ;
      }

      // Here is how the alignment is treated
      // Misalignment is already in cluster coordinates
//      if (!cl->Misalign()) IlcWarning("Can't misalign this cluster !");

      cl->SetBit(14,kFALSE); // The clusters are not yet attached to any track

      TClonesArray &module=*fModules[m-1];
      Int_t idx=module.GetEntriesFast();
      new (module[idx]) IlcPVECEmcRecPoint(*cl); 

      nclusters++;

    }
  }  
  arr->Delete();
  Info("LoadClusters","Number of loaded clusters: %d",nclusters);

  return 0;

}

//____________________________________________________________________________
Int_t IlcPVECTracker::PropagateBack(IlcESDEvent *esd) {
  //--------------------------------------------------------------------
  // Called by IlcReconstruction 
  // Performs the track matching with the PVEC modules
  // Makes the PID
  //--------------------------------------------------------------------

  Int_t nt=esd->GetNumberOfTracks();

  // *** Select and sort the ESD track in accordance with their quality
  Double_t *quality=new Double_t[nt];
  Int_t *index=new Int_t[nt];  
  for (Int_t i=0; i<nt; i++) {
     IlcESDtrack *esdTrack=esd->GetTrack(i);
     quality[i] = esdTrack->GetSigmaY2() + esdTrack->GetSigmaZ2();
  }
  TMath::Sort(nt,quality,index,kFALSE);

  IlcPVECGeometry * geom = IlcPVECGeometry::GetInstance() ;

  // *** Start the matching
  TVector3 vecEmc ;   // Local position of EMC recpoint
  Double_t bz = GetBz() ; //For approximate matching
  Double_t b[3];  //For final matching
  Double_t gposTrack[3] ;
  Int_t matched=0;
  for (Int_t i=0; i<nt; i++) {
     IlcESDtrack *esdTrack=esd->GetTrack(index[i]);

     // Skip the tracks having "wrong" status (has to be checked/tuned)
     ULong_t status = esdTrack->GetStatus();
     if ((status & IlcESDtrack::kTPCout)   == 0) continue;
//     if ((status & IlcESDtrack::kTRDout)   == 0) continue;
//     if ((status & IlcESDtrack::kTRDrefit) == 1) continue;

     //Continue extrapolation from TPC outer surface
     const IlcExternalTrackParam *outerParam=esdTrack->GetOuterParam();
     if (!outerParam) continue;
     IlcExternalTrackParam t(*outerParam);

     t.GetBxByBz(b) ;

     //Loop over PVEC modules
     Double_t dx=0,dz=0;
     Double_t minDistance=999. ;
     Int_t emcIndex=0  ;
     for(Int_t mod=1; mod<=5; mod++){
       if(fModules[mod-1]->GetEntriesFast()==0) //no PVEC clusters in this module or module does not exist
         continue ;
       
       //Approximate direction to the current PVEC module
       Double_t phiMod=(330.-20.*mod)/180.*TMath::Pi() ;
       if(!t.Rotate(phiMod))
         continue ;
       TVector3 globaPos ;
       geom->Local2Global(mod, 0.,0., globaPos) ;
       const Double_t rPVEC = globaPos.Pt() ; //Distance to center of  PVEC module
       const Double_t kYmax = 72.+10. ; //Size of the module (with some reserve) in phi direction
       const Double_t kZmax = 64.+20. ; //Size of the module (with some reserve) in z direction
 
       Double_t y;                       // Some tracks do not reach the PVEC
       if (!t.GetYAt(rPVEC,bz,y)) continue; //    because of the bending

       Double_t z;
       if(!t.GetZAt(rPVEC,bz,z))
         continue ;
       if (TMath::Abs(z) > kZmax)
         continue; // Some tracks miss the PVEC in Z
       if(TMath::Abs(y) < kYmax){
         t.PropagateToBxByBz(rPVEC,b);        // Propagate to the matching module
      //t.CorrectForMaterial(...); // Correct for the TOF material, if needed
         t.GetXYZ(gposTrack) ;
         TVector3 globalPositionTr(gposTrack) ;
         TVector3 localPositionTr ;
         geom->Global2Local(localPositionTr,globalPositionTr,mod) ;
         for(Int_t icl=0;icl<fModules[mod-1]->GetEntriesFast();icl++){
           IlcPVECEmcRecPoint * clu =static_cast<IlcPVECEmcRecPoint*>(fModules[mod-1]->At(icl)) ;
           clu->GetLocalPosition(vecEmc) ;
           Double_t ddx = vecEmc.X()-localPositionTr.X();
           Double_t ddz = vecEmc.Z()-localPositionTr.Z();
           Double_t d2 = ddx*ddx + ddz*ddz;
           if(d2 < minDistance) {
             dx = ddx ;
             dz = ddz ;
             emcIndex=clu->GetIndexInList() ;
             minDistance=d2 ;
           }
         }
         break ;
       }
     } //Loop over modules
 
     if(minDistance<999.){
       //found some match
       esdTrack->SetStatus(IlcESDtrack::kPHOSmatch) ;
       esdTrack->SetPHOScluster(-emcIndex) ; //Should be ESDCaloCluster index which is not known yet. Will be transformed later in FillESD().
       esdTrack->SetPHOSdxdz(dx,dz) ;
       matched++;
     }

  }
    
  Info("PropagateBack","Number of matched tracks: %d",matched);

  delete[] quality;
  delete[] index;

  return 0;

}

//____________________________________________________________________________
IlcCluster *IlcPVECTracker::GetCluster(Int_t index) const {
  //--------------------------------------------------------------------
  // Returns the pointer to a given cluster
  //--------------------------------------------------------------------
  Int_t m=(index & 0xf0000000) >> 28;  // Module number
  Int_t i=(index & 0x0fffffff) >> 00;  // Index within the module
  
  return (IlcCluster*)(fModules[m])->UncheckedAt(i);
}

//____________________________________________________________________________
void IlcPVECTracker::UnloadClusters() {
  //--------------------------------------------------------------------
  // This function unloads the PVEC clusters
  //--------------------------------------------------------------------
//  for (Int_t i=0; i<5; i++) (fModules[i])->Delete();
}
