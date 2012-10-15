/**************************************************************************
 * Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 *                                                                        *
// Author: The ILC Off-line Project. 
 // Part of the code has been developed by Alice Off-line Project. 
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

/* $Id: IlcTARGETRecPoint.cxx,v 1.1.1.1 2008/03/11 14:52:50 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
//  Reconstructed space point class for set:TARGET   
//  Reconstructed points are expressed simultaneously in two different 
//  reference frames, both differing from the global system.
//  The first is referred to the sensor (see IlcTARGETsegmentation for the
//  definition) and each point is represented by two coordinates: fXloc and
//  fZloc. This system in the code is referred to as "local"
//  The second is used for tracking (V2, SA and MI versions) and the X axis 
//  represents the radial coordinate (this system is, in the bending plane, 
//  a rotated system w.r.t. the global reference system). 
//  Each reaconstructed point is represented by two coordinates: fY and fZ, 
//  inherited from IlcCluster. This system in the code is referred to as 
//  "trackingV2".
///////////////////////////////////////////////////////////////////////////////


#include "IlcTARGETRecPoint.h"
#include "IlcTARGETgeom.h"
ClassImp(IlcTARGETRecPoint)

//_____________________________________________________________
IlcTARGETRecPoint::IlcTARGETRecPoint(): IlcCluster() {
    // default creator
    fYloc=fXloc=fZloc=fdEdX=0.;
    fQ=0; fLayer=0; fNz=fNy=1; fType=0; fDeltaProb=0;fChargeRatio=0;
    fDet=0;
    fModule=-1;
    fGeom=0;
}

//_____________________________________________________________
IlcTARGETRecPoint::IlcTARGETRecPoint(IlcTARGETgeom* geom): IlcCluster() {
    // default creator
    fYloc=fXloc=fZloc=fdEdX=0.;
    fQ=0; fLayer=0; fNz=fNy=1; fType=0; fDeltaProb=0;fChargeRatio=0;
    fDet=0;
    fModule=-1;
    fGeom=geom;
}

//________________________________________________________________________
IlcTARGETRecPoint::IlcTARGETRecPoint(Int_t module,IlcTARGETgeom* geom,Int_t *lab,Float_t *hit, Int_t *info):IlcCluster(0,0.,hit[0],hit[1], 0.,hit[2],hit[3],lab){
  //standard constructor used in IlcTARGETClusterFinderV2
  fXloc=fYloc=fZloc=0.;
  fIndex=lab[3];
  fQ=hit[4];
  fNy    = info[0];
  fNz    = info[1];
  fLayer = info[2];
  fDet=0;
  fChargeRatio = 0;
  fType=0;
  fModule=-1;
  fDeltaProb=0.;
  
  fGeom = geom;
//  fGeom->DetLToTrackingV2(module,fXloc,fZloc,fY,fZ);
  
  if(module<fGeom->GetStartSDD()) fdEdX=0.;
  if(module>=fGeom->GetStartSDD() && module<fGeom->GetStartSSD()){
    fdEdX=fQ*1e-6;
  }
  if(module>=fGeom->GetStartSSD()) fdEdX=fQ*2.16;
  
  
}
//_______________________________________________________________________
IlcTARGETRecPoint::IlcTARGETRecPoint(const IlcTARGETRecPoint& pt):IlcCluster(pt){
  //Copy constructor
  fXloc = pt.fXloc;
  fZloc = pt.fZloc;
  fYloc = pt.fYloc;
  fdEdX = pt.fdEdX;
  fIndex= pt.fIndex;
  fModule=pt.fModule;
  fDet=pt.fDet;
  fQ = pt.fQ;
  fNy = pt.fNy;
  fNz = pt.fNz;
  fLayer = pt.fLayer;
  fChargeRatio = pt.fChargeRatio;
  fType = pt.fType;
  fDeltaProb = pt.fDeltaProb;
  fGeom = pt.fGeom;

}

//______________________________________________________________________
IlcTARGETRecPoint& IlcTARGETRecPoint::operator=(const IlcTARGETRecPoint& /* source */){
  // Assignment operator
  // Assignment is not allowed. The method is protected to avoid misuse.
  Fatal("= operator","Assignment operator not allowed\n");
  return *this;
}

//________________________________________________________________________
IlcTARGETRecPoint::IlcTARGETRecPoint(Int_t *lab,Float_t *hit, Int_t *info):IlcCluster(0,0.,hit[0],hit[1], 0.,hit[2],hit[3],lab){
  //standard constructor used in IlcTARGETClusterFinderV2
  fYloc=fXloc=fZloc=fdEdX=0.;
  fIndex=lab[3];
  fQ=hit[4];
  fNy    = info[0];
  fNz    = info[1];
  fLayer = info[2];
  fChargeRatio = 0;
  fType=0;
  fModule=-1;
  fDet=0;
  fDeltaProb=0.;  
  fGeom = 0;
}

//----------------------------------------------------------------------
void IlcTARGETRecPoint::Print(ostream *os){
    ////////////////////////////////////////////////////////////////////////
    // Standard output format for this class.
    ////////////////////////////////////////////////////////////////////////
#if defined __GNUC__
#if __GNUC__ > 2
    ios::fmtflags fmt;
#else
    Int_t fmt;
#endif
#else
#if defined __ICC || defined __ECC || defined __xlC__
    ios::fmtflags fmt;
#else
    Int_t fmt;
#endif
#endif
 
    fmt = os->setf(ios::fixed);  // set fixed floating point output
    *os << GetLabel(0)<< " " << GetLabel(1) << " " << GetLabel(2) << " ";
    *os << fXloc << " " << fZloc << " " << fQ << " ";
    fmt = os->setf(ios::scientific); // set scientific for dEdX.
    *os << fdEdX << " ";
    fmt = os->setf(ios::fixed); // every fixed
    *os << GetSigmaY2() << " " << GetSigmaZ2();
    os->flags(fmt); // reset back to old formating.
    return;
}
//----------------------------------------------------------------------
void IlcTARGETRecPoint::Read(istream *is){
////////////////////////////////////////////////////////////////////////
// Standard input format for this class.
////////////////////////////////////////////////////////////////////////
 
  Float_t SigmaY2 = GetSigmaY2();
  Float_t SigmaZ2 = GetSigmaZ2();
  *is >> GetTracks()[0] >> GetTracks()[1] >> GetTracks()[2] >> fXloc >> fZloc >> fQ;
  *is >> fdEdX >> SigmaY2 >> SigmaZ2;
    return;
}
//----------------------------------------------------------------------
ostream &operator<<(ostream &os,IlcTARGETRecPoint &p){
////////////////////////////////////////////////////////////////////////
// Standard output streaming function.
////////////////////////////////////////////////////////////////////////
 
    p.Print(&os);
    return os;
}
//----------------------------------------------------------------------
istream &operator>>(istream &is,IlcTARGETRecPoint &r){
////////////////////////////////////////////////////////////////////////
// Standard input streaming function.
////////////////////////////////////////////////////////////////////////
 
    r.Read(&is);
    return is;
}
//----------------------------------------------------------------------
