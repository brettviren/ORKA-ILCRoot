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
/* $Id: IlcRSTACKRecPoint.cxx 54863 2012-02-28 12:10:29Z hristov $ */
//_________________________________________________________________________
//  Base Class for RSTACK Reconstructed Points  
//  Why should I put meaningless comments
//  just to satisfy
//  the code checker                
//-- Author: Gines Martinez (SUBATECH)

// --- ROOT system ---
#include "TPad.h"
#include "TGraph.h"
#include "TPaveText.h"
#include "TClonesArray.h"
#include "TGeoMatrix.h"

// --- Standard library ---

// --- IlcRoot header files ---
#include "IlcLog.h"
#include "IlcRSTACKLoader.h"
#include "IlcRSTACKGeometry.h"
#include "IlcRSTACKDigit.h"
#include "IlcRSTACKRecPoint.h"
#include "IlcGeomManager.h"

ClassImp(IlcRSTACKRecPoint)


//____________________________________________________________________________
IlcRSTACKRecPoint::IlcRSTACKRecPoint()
  : IlcCluster(),fRSTACKMod(0),
    fMulTrack(0),fMaxDigit(100),fMulDigit(0),fMaxTrack(200),
    fDigitsList(0),fTracksList(0),fAmp(0),
    fIndexInList(-1), // to be set when the point is already stored
    fLocPos(0,0,0)
{
  // ctor

}

//____________________________________________________________________________
IlcRSTACKRecPoint::IlcRSTACKRecPoint(const char * ) 
  : IlcCluster(),fRSTACKMod(0),
    fMulTrack(0),fMaxDigit(100),fMulDigit(0),fMaxTrack(200),
    fDigitsList(new Int_t[fMaxDigit]),fTracksList(new Int_t[fMaxTrack]),fAmp(0),
    fIndexInList(-1), // to be set when the point is already stored
    fLocPos(0,0,0)

{
  // ctor
  
}
//_______________________________________________________________________
IlcRSTACKRecPoint::~IlcRSTACKRecPoint()
{
  // dtor
  
  delete [] fDigitsList ; 
  delete [] fTracksList ;  
  
}
//____________________________________________________________________________
IlcRSTACKRecPoint::IlcRSTACKRecPoint(const IlcRSTACKRecPoint &rp) : 
  IlcCluster(rp),
  fRSTACKMod(rp.fRSTACKMod),fMulTrack(rp.fMulTrack),fMaxDigit(rp.fMaxDigit),
  fMulDigit(rp.fMulDigit),fMaxTrack(rp.fMaxTrack),fDigitsList(0x0),
  fTracksList(0x0),fAmp(rp.fAmp),fIndexInList(rp.fIndexInList), 
  fLocPos(rp.fLocPos)
{
  //copy ctor

  if (rp.fMulDigit>0) fDigitsList = new Int_t[rp.fMulDigit];
  for(Int_t i=0; i<fMulDigit; i++)
    fDigitsList[i] = rp.fDigitsList[i];

  if (rp.fMulTrack>0) fTracksList = new Int_t[rp.fMulTrack];
  for(Int_t i=0; i<fMulTrack; i++)
    fTracksList[i] = rp.fTracksList[i];
  
}
//____________________________________________________________________________
IlcRSTACKRecPoint& IlcRSTACKRecPoint::operator= (const IlcRSTACKRecPoint &rp)
{
  if(&rp == this) return *this;

  fRSTACKMod     = rp.fRSTACKMod;
  fMulTrack    = rp.fMulTrack;
  fMaxDigit    = rp.fMaxDigit;
  fMulDigit    = rp.fMulDigit;
  fMaxTrack    = rp.fMaxTrack;
  fAmp         = rp.fAmp;
  fIndexInList = rp.fIndexInList; 
  fLocPos = rp.fLocPos;

  if (fDigitsList != 0) delete [] fDigitsList;
  if (fMaxDigit>0) {
    fDigitsList = new Int_t[fMaxDigit];
    for(Int_t i=0; i<fMaxDigit; i++)
      fDigitsList[i] = rp.fDigitsList[i];
  }

  if (fTracksList != 0) delete [] fTracksList;
  if (fMaxTrack>0) {
    fTracksList = new Int_t[fMaxTrack];
    for(Int_t i=0; i<fMaxTrack; i++)
      fTracksList[i] = rp.fTracksList[i];
  }

  return *this;
}
//____________________________________________________________________________
Int_t IlcRSTACKRecPoint::DistancetoPrimitive(Int_t px, Int_t py)
{
  // Compute distance from point px,py to  a IlcRSTACKRecPoint considered as a Tmarker
  // Compute the closest distance of approach from point px,py to this marker.
  // The distance is computed in pixels units.

  TVector3 pos(0.,0.,0.) ;
  GetLocalPosition( pos) ;
  Float_t x =  pos.X() ;
  Float_t y =  pos.Z() ;
  const Int_t kMaxDiff = 10;
  Int_t pxm  = gPad->XtoAbsPixel(x);
  Int_t pym  = gPad->YtoAbsPixel(y);
  Int_t dist = (px-pxm)*(px-pxm) + (py-pym)*(py-pym);
  
  if (dist > kMaxDiff) return 9999;
  return dist;
}

//___________________________________________________________________________
 void IlcRSTACKRecPoint::Draw(Option_t *option)
 {
   // Draw this IlcRSTACKRecPoint with its current attributes
   
   AppendPad(option);
 }

//______________________________________________________________________________
void IlcRSTACKRecPoint::ExecuteEvent(Int_t event, Int_t, Int_t)
{
  // Execute action corresponding to one event
  // This member function is called when a IlcRSTACKRecPoint is clicked with the locator
  //
  // If Left button is clicked on IlcRSTACKRecPoint, the digits are switched on    
  // and switched off when the mouse button is released.

  //  static Int_t pxold, pyold;

  static TGraph *  digitgraph = 0 ;
  static TPaveText* clustertext = 0 ;
  
  if (!gPad->IsEditable()) return;
  
  switch (event) {
    
    
  case kButton1Down:{
    IlcRSTACKDigit * digit ;
  
    IlcRSTACKGeometry * RSTACKgeom =  IlcRSTACKGeometry::GetInstance() ;

    Int_t iDigit;
    Int_t relid[4] ;
  
    const Int_t kMulDigit=IlcRSTACKRecPoint::GetDigitsMultiplicity() ;
    Float_t * xi = new Float_t [kMulDigit] ; 
    Float_t * zi = new Float_t [kMulDigit] ;
    
    for(iDigit = 0; iDigit < kMulDigit; iDigit++) {
      Fatal("IlcRSTACKRecPoint::ExecuteEvent", "-> Something wrong with the code"); 
      digit = 0 ; //dynamic_cast<IlcRSTACKDigit *>((fDigitsList)[iDigit]);
      RSTACKgeom->AbsToRelNumbering(digit->GetId(), relid) ;
      RSTACKgeom->RelPosInModule(relid, xi[iDigit], zi[iDigit]) ;
    }
    
    if (!digitgraph) {
      digitgraph = new TGraph(fMulDigit,xi,zi);
      digitgraph-> SetMarkerStyle(5) ; 
      digitgraph-> SetMarkerSize(1.) ;
      digitgraph-> SetMarkerColor(1) ;
      digitgraph-> Draw("P") ;
    }
    if (!clustertext) {
      
      TVector3 pos(0.,0.,0.) ;
      GetLocalPosition(pos) ;
      clustertext = new TPaveText(pos.X()-10,pos.Z()+10,pos.X()+50,pos.Z()+35,"") ;
      Text_t  line1[40] ;
      Text_t  line2[40] ;
      snprintf(line1,40,"Energy=%1.2f GeV",GetEnergy()) ;
      snprintf(line2,40,"%d Digits",GetDigitsMultiplicity()) ;
      clustertext ->AddText(line1) ;
      clustertext ->AddText(line2) ;
      clustertext ->Draw("");
    }
    gPad->Update() ; 
    Print("dummy") ;
    delete[] xi ; 
    delete[] zi ; 
   }
  
break;
  
  case kButton1Up:
    if (digitgraph) {
      delete digitgraph  ;
      digitgraph = 0 ;
    }
    if (clustertext) {
      delete clustertext ;
      clustertext = 0 ;
    }
    
    break;
    
  }
}
//____________________________________________________________________________
void IlcRSTACKRecPoint::EvalAll(TClonesArray * /* digits */) 
{
  //evaluates (if necessary) all RecPoint data members 

}

//____________________________________________________________________________
void IlcRSTACKRecPoint::EvalRSTACKMod(IlcRSTACKDigit * digit) 
{
  // Returns the RSTACK module in which the RecPoint is found

  if( fRSTACKMod == 0){
  Int_t relid[4] ; 
 
  IlcRSTACKGeometry * RSTACKgeom = (IlcRSTACKGeometry::GetInstance());

  RSTACKgeom->AbsToRelNumbering(digit->GetId(), relid) ;
  fRSTACKMod = relid[0];
  }
}

//____________________________________________________________________________
void IlcRSTACKRecPoint::GetGlobalPosition(TVector3 & gpos, TMatrixF & gmat) const
{
  // returns the position of the cluster in the global reference system of ILC
  // and the uncertainty on this position

   IlcRSTACKGeometry * RSTACKgeom = (IlcRSTACKGeometry::GetInstance());
   RSTACKgeom->GetGlobalRSTACK(this, gpos, gmat);
  
}

//______________________________________________________________________________
void IlcRSTACKRecPoint::Paint(Option_t *)
{
  // Paint this IlcRecPoint as a TMarker  with its current attributes
  
  TVector3 pos(0.,0.,0.)  ;
  GetLocalPosition(pos)   ;
  Coord_t x = pos.X()     ;
  Coord_t y = pos.Z()     ;
  Color_t markercolor = 1 ;
  Size_t  markersize = 1. ;
  Style_t markerstyle = 5 ;
  
  if (!gPad->IsBatch()) {
    gVirtualX->SetMarkerColor(markercolor) ;
    gVirtualX->SetMarkerSize (markersize)  ;
    gVirtualX->SetMarkerStyle(markerstyle) ;
  }
  gPad->SetAttMarkerPS(markercolor,markerstyle,markersize) ;
  gPad->PaintPolyMarker(1,&x,&y,"") ;
}
//______________________________________________________________________________
void IlcRSTACKRecPoint::GetLocalPosition(TVector3 & pos) const
{
  // returns the position of the cluster in the local reference system 
  // of the sub-detector
  
  pos = fLocPos;
}

//____________________________________________________________________________
void IlcRSTACKRecPoint::EvalLocal2TrackingCSTransform()
{
  //Evaluates local to "tracking" c.s. transformation (B.P.).
  //All evaluations should be completed before calling for this function.
  //See ILC PPR Chapter 5 p.18 for "tracking" c.s. definition,
  //or just ask Jouri Belikov. :)

  if(IsEmc()) {
    SetVolumeId(IlcGeomManager::LayerToVolUID(IlcGeomManager::kRSTACK1,GetRSTACKMod()-1));
  }
  else
    SetVolumeId(IlcGeomManager::LayerToVolUID(IlcGeomManager::kRSTACK2,GetRSTACKMod()-1));

  Double_t lxyz[3] = {fLocPos.X(),fLocPos.Y(),fLocPos.Z()};
  Double_t txyz[3] = {0,0,0};
  Double_t dy;
  Double_t crystalShift;

  IlcRSTACKGeometry * RSTACKgeom = IlcRSTACKGeometry::GetInstance();
  IlcRSTACKEMCAGeometry* geoEMCA = RSTACKgeom->GetEMCAGeometry(); 

  //Calculate offset to crystal surface.
  //See fCrystalShift code in IlcRSTACKGeometry::Init()).

  const Float_t * inthermo = geoEMCA->GetInnerThermoHalfSize() ;
  const Float_t * strip    = geoEMCA->GetStripHalfSize() ;
  const Float_t * splate   = geoEMCA->GetSupportPlateHalfSize();
  const Float_t * crystal  = geoEMCA->GetCrystalHalfSize() ;
  const Float_t * pin      = geoEMCA->GetAPDHalfSize() ;
  const Float_t * preamp   = geoEMCA->GetPreampHalfSize() ;
  crystalShift = -inthermo[1]+strip[1]+splate[1]+crystal[1]-geoEMCA->GetAirGapLed()/2.+pin[1]+preamp[1] ;

  if(IsEmc()) {
    dy = crystalShift;
    lxyz[2] = -lxyz[2]; //Opposite z directions in EMC matrix and local frame!!!
  }
  else
    dy = RSTACKgeom->GetCPVBoxSize(1)/2.; //center of CPV module

  lxyz[1] = lxyz[1] - dy;

  const TGeoHMatrix* tr2loc = GetTracking2LocalMatrix();
  if(!tr2loc) IlcFatal(Form("No Tracking2LocalMatrix found for VolumeID=%d",GetVolumeId()));

  tr2loc->MasterToLocal(lxyz,txyz);
  SetX(txyz[0]); SetY(txyz[1]); SetZ(txyz[2]);

  if(IlcLog::GetGlobalDebugLevel()>0) {
    TVector3 gpos; TMatrixF gmat;
    GetGlobalPosition(gpos,gmat);
    Float_t gxyz[3];
    GetGlobalXYZ(gxyz);
    TString emc;
    if(IsEmc()) 
      emc="EMC";
    else
      emc="CPV";
    IlcInfo(Form("lCS-->(%.3f,%.3f,%.3f), tCS-->(%.3f,%.3f,%.3f), gCS-->(%.3f,%.3f,%.3f),  gCScalc-->(%.3f,%.3f,%.3f), module %d %s",
		 fLocPos.X(),fLocPos.Y(),fLocPos.Z(),
		 GetX(),GetY(),GetZ(),
		 gpos.X(),gpos.Y(),gpos.Z(),
		 gxyz[0],gxyz[1],gxyz[2],GetRSTACKMod(),emc.Data()));
  }

}
