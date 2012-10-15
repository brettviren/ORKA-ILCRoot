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

/* $Id: IlcTARGEThit.cxx,v 1.1.1.1 2008/03/11 14:52:51 vitomeg Exp $ */

#include <Riostream.h>

#include<TLorentzVector.h>

#include "IlcRun.h"
#include "IlcTARGET.h"
#include "IlcTARGETgeom.h"
#include "IlcTARGEThit.h"
#include "IlcMC.h"


ClassImp(IlcTARGEThit)
////////////////////////////////////////////////////////////////////////
// Version: 0
// Written by Rene Brun, Federico Carminati, and Roberto Barbera
//
// Version: 1
// Modified and documented by Bjorn S. Nilsen
// July 11 1999
//
// IlcTARGEThit is the hit class for the TARGET. Hits are the information
// that comes from a Monte Carlo at each step as a particle mass through
// sensitive detector elements as particles are transported through a
// detector.
//
//Begin_Html
/*
<img src="picts/TARGET/IlcTARGEThit_Class_Diagram.gif">
</pre>
<br clear=left>
<font size=+2 color=red>
<p>This show the relasionships between the TARGET hit class and the rest of Ilcroot.
</font>
<pre>
*/
//End_Html
////////////////////////////////////////////////////////////////////////
// Inline Member functions:
//
// IlcTARGEThit()
//     The default creator of the IlcTARGEThit class.
//
// ~IlcTARGEThit()
//     The default destructor of the IlcTARGEThit class.
//
// int GetTrack()
//     See IlcHit for a full description. Returns the track number fTrack
// for this hit.
//
// SetTrack(int track)
//     See IlcHit for a full description. Sets the track number fTrack
// for this hit.
//
// Int_t GetTrackStatus()
//     Returns the value of the track status flag fStatus. This flag
// indicates the track status at the time of creating this hit. It is
// made up of the following 8 status bits from highest order to lowest
// order bits
// 0           :  IsTrackAlive():    IsTrackStop():IsTrackDisappeared():
// IsTrackOut():IsTrackExiting():IsTrackEntering():IsTrackInside()     .
// See IlcMC for a description of these functions. If the function is
// true then the bit is set to one, otherwise it is zero.
//
// Bool_t StatusInside()
//     Returns kTRUE if the particle producing this hit is still inside
// the present volume. Returns kFalse if this particle will be in another
// volume. {bit IsTrackInside is set or not}
//
// Bool_t StatusEntering()
//     Returns kTRUE if the particle producing this hit is has just enterd
// the present volume. Returns kFalse otherwise.  {bit IsTrackEntering is
// set or not}
//
// Bool_t StatusExiting()
//     Returns kTRUE if the particle producing this hit is will exit
// the present volume. Returns kFalse otherwise. {bit IsTrackExiting is set
// or not}
//
// Bool_t StatusOut()
//     Returns kTRUE if the particle producing this hit is goint exit the
// simulation. Returns kFalse otherwise. {bit IsTrackOut is set or not}
//
// Bool_t StatusDisappeared()
//     Returns kTRUE if the particle producing this hit is going to "disappear"
// for example it has interacted producing some other particles. Returns
//  kFalse otherwise. {bit IsTrackOut is set or not}
//
// Bool_t StatusStop()
//     Returns kTRUE if the particle producing this hit is has dropped below
// its energy cut off producing some other particles. Returns kFalse otherwise.
// {bit IsTrackOut is set or not}
//
// Bool_t StatuAlives()
//     Returns kTRUE if the particle producing this hit is going to continue
// to be transported. Returns kFalse otherwise. {bit IsTrackOut is set or not}
//
// Int_t GetLayer()
//     Returns the layer number, fLayer, for this hit.
//
// Int_t GetLadder()
//     Returns the ladder number, fLadder, for this hit.
//
// Int_t GetDetector()
//     Returns the detector number, fDet, for this hit.
//
// GetDetectorID(Int_t &layer, Int_t &ladder, Int_t &detector)
//     Returns the layer, ladder, and detector numbers, fLayer fLadder fDet,
// in one call.
//
// Float_t GetIonization()
//     Returns the energy lost, fDestep, by the particle creating this hit,
// in the units defined by the Monte Carlo.
//
// GetPositionG(Float_t &x, Float_t &y, Float_t &z)
//     Returns the global position, fX fY fZ, of this hit, in the units
// define by the Monte Carlo.
//
// GetPositionG(Double_t &x, Double_t &y, Double_t &z)
//     Returns the global position, fX fY fZ, of this hit, in the units
// define by the Monte Carlo.
//
// GetPositionG(Float_t &x, Float_t &y, Float_t &z, Float_t &tof)
//     Returns the global position and time of flight, fX fY fZ fTof, of
// this hit, in the units define by the Monte Carlo.
//
// GetPositionG(Double_t &x,Double_t &y,Double_t &z,Double_t &tof)
//     Returns the global position and time of flight, fX fY fZ fTof, of
// this hit, in the units define by the Monte Carlo.
//
// GetPositionL(Double_t &x,Double_t &y,Double_t &z)
//     Returns the local position, fX fY fZ, of this hit in the coordiante
// of this module, in the units define by the Monte Carlo.
//
// GetPositionG(Double_t &x,Double_t &y,Double_t &z,Double_t &tof)
//     Returns the local position and time of flight, fX fY fZ fTof, of
// this hit in the coordinates of this module, in the units define by the
//  Monte Carlo.
//
// Float_t GetXG()
//     Returns the global x position in the units defined by the Monte Carlo.
//
// Float_t GetYG()
//     Returns the global y position in the units defined by the Monte Carlo.
//
// Float_t GetYG()
//     Returns the global z position in the units defined by the Monte Carlo.
//
// Float_t GetTOF()
//     Returns the time of flight, fTof, of this hit, in the units defined
// by the Monte Carlo.
//
// GetMomentumG(Float_t &px, Float_t &py, Float_t &pz)
//     Returns the global momentum, fPx fPy fPz, of the particle that made
// this hit, in the units define by the Monte Carlo.
//
// GetMomentumG(Double_t &px,Double_t &py,Double_t &pz)
//     Returns the global momentum, fPx fPy fPz, of the particle that made
// this hit, in the units define by the Monte Carlo.
//
// GetMomentumL(Double_t &px,Double_t &py,Double_t &pz)
//     Returns the momentum, fPx fPy fPz in coordinate appropreate for this
// specific module, in the units define by the Monte Carlo.
//
// Float_t GetPXG()
//     Returns the global X momentum in the units defined by the Monte Carlo.
//
// Float_t GetPYG()
//     Returns the global Y momentum in the units defined by the Monte Carlo.
//
// Float_t GetPZG()
//     Returns the global Z momentum in the units defined by the Monte Carlo.
//
////////////////////////////////////////////////////////////////////////
//_____________________________________________________________________________
IlcTARGEThit::IlcTARGEThit():IlcHit(){
    // Default Constructor
    // Zero data member just to be safe.
    // Intputs:
    //    none.
    // Outputs:
    //    none.
    // Return:
    //    A default created IlcTARGEThit class.

    fStatus = 0; // Track Status
    fLayer  = 0;  // Layer number
    fLadder = 0; // Ladder number
    fDet    = 0;    // Detector number  
    fPx     = 0.0;     // PX of particle at the point of the hit
    fPy     = 0.0;     // PY of particle at the point of the hit
    fPz     = 0.0;     // PZ of particle at the point of the hit
    fDestep = 0.0; // Energy deposited in the current step
    fTof    = 0.0;    // Time of flight at the point of the hit
    fStatus0 = 0; // zero status bit by default.
    fx0     = 0.0;     // Starting point of this step
    fy0     = 0.0;     // Starting point of this step
    fz0     = 0.0;     // Starting point of this step
    ft0     = 0.0;     // Starting point of this step
}
IlcTARGEThit::IlcTARGEThit(Int_t shunt,Int_t track,Int_t *vol,Float_t edep,
		     Float_t tof,TLorentzVector &x,TLorentzVector &x0,
		     TLorentzVector &p) : IlcHit(shunt, track){
////////////////////////////////////////////////////////////////////////
// Create TARGET hit
//     The creator of the IlcTARGEThit class. The variables shunt and
// track are passed to the creator of the IlcHit class. See the IlcHit
// class for a full description. In the units of the Monte Carlo
////////////////////////////////////////////////////////////////////////
    // Intputs:
    //    Int_t shunt   See IlcHit
    //    Int_t track   Track number, see IlcHit
    //    Int_t *vol     Array of integer hit data,
    //                     vol[0] Layer where the hit is, 1-6 typicaly
    //                     vol[1] Ladder where the hit is.
    //                     vol[2] Detector number where the hit is
    //                     vol[3] Set of status bits
    //                     vol[4] Set of status bits at start
    // Outputs:
    //    none.
    // Return:
    //    A default created IlcTARGEThit class.

    fLayer      = vol[0];  // Layer number
    fLadder     = vol[2];  // Ladder number
    fDet        = vol[1];  // Detector number
    fStatus     = vol[3];  // Track status flags
    fStatus0    = vol[4];  // Track status flag for start position.
    fX          = x.X();   // Track X global position
    fY          = x.Y();   // Track Y global position
    fZ          = x.Z();   // Track Z global position
    fPx         = p.Px();  // Track X Momentum
    fPy         = p.Py();  // Track Y Momentum
    fPz         = p.Pz();  // Track Z Momentum
    fDestep     = edep;    // Track dE/dx for this step
    fTof        = tof   ;  // Track Time of Flight for this step
    fx0         = x0.X();  // Track X global position
    fy0         = x0.Y();  // Track Y global position
    fz0         = x0.Z();  // Track Z global position
    ft0         = x0.T();     // Starting point of this step
}
//______________________________________________________________________
IlcTARGEThit::IlcTARGEThit(Int_t shunt, Int_t track, Int_t *vol, Float_t *hits):
    IlcHit(shunt, track){
////////////////////////////////////////////////////////////////////////
// Create TARGET hit
//     The creator of the IlcTARGEThit class. The variables shunt and
// track are passed to the creator of the IlcHit class. See the IlcHit
// class for a full description. the integer array *vol contains, in order,
// fLayer = vol[0], fDet = vol[1], fLadder = vol[2], fStatus = vol[3].
// The array *hits contains, in order, fX = hits[0], fY = hits[1],
// fZ = hits[2], fPx = hits[3], fPy = hits[4], fPz = hits[5],
// fDestep = hits[6], and fTof = hits[7]. In the units of the Monte Carlo
////////////////////////////////////////////////////////////////////////
    // Intputs:
    //    Int_t shunt   See IlcHit
    //    Int_t track   Track number, see IlcHit
    //    Int_t *vol     Array of integer hit data,
    //                     vol[0] Layer where the hit is, 1-6 typicaly
    //                     vol[1] Ladder where the hit is.
    //                     vol[2] Detector number where the hit is
    //                     vol[3] Set of status bits
    //    Float_t *hits   Array of hit information
    //                     hits[0] X global position of this hit
    //                     hits[1] Y global position of this hit
    //                     hits[2] Z global position of this hit
    //                     hits[3] Px global position of this hit
    //                     hits[4] Py global position of this hit
    //                     hits[5] Pz global position of this hit
    //                     hits[6] Energy deposited by this step
    //                     hits[7] Time of flight of this particle at this step
    // Outputs:
    //    none.
    // Return:
    //    A standard created IlcTARGEThit class.
  fLayer      = vol[0];   // Layer number
  fLadder     = vol[2];   // Ladder number
  fDet        = vol[1];   // Detector number
  fStatus     = vol[3];   // Track status flags
  fX          = hits[0];  // Track X global position
  fY          = hits[1];  // Track Y global position
  fZ          = hits[2];  // Track Z global position
  fPx         = hits[3];  // Track X Momentum
  fPy         = hits[4];  // Track Y Momentum
  fPz         = hits[5];  // Track Z Momentum
  fDestep     = hits[6];  // Track dE/dx for this step
  fTof        = hits[7];  // Track Time of Flight for this step
  fStatus0 = 0;// Track Status of Starting point
  fx0 = 0.0;     // Starting point of this step
  fy0 = 0.0;     // Starting point of this step
  fz0 = 0.0;     // Starting point of this step
  ft0 = 0.0;     // Starting point of this step
}
//______________________________________________________________________
void IlcTARGEThit::GetPositionL(Float_t &x,Float_t &y,Float_t &z){
////////////////////////////////////////////////////////////////////////
//     Returns the position of this hit in the local coordinates of this
// module, and in the units of the Monte Carlo.
////////////////////////////////////////////////////////////////////////
    IlcTARGETgeom *gm = ((IlcTARGET*)gIlc->GetDetector("TARGET"))->GetTARGETgeom2();
    Float_t g[3],l[3];

    g[0] = fX;
    g[1] = fY;
    g[2] = fZ;
    if(gm) {
      gm->GtoL(fLayer,fLadder,fDet,g,l);
      x = l[0];
      y = l[1];
      z = l[2];
    } else {
      Error("IlcTARGEThit","NULL pointer to the geometry! return smth else",gm);
      // IlcTARGETv7 - SDD case
      x=fX;
      y=fY;
      z=fZ;
    }
    return;
}
//______________________________________________________________________
void IlcTARGEThit::GetPositionL(Float_t &x,Float_t &y,Float_t &z,Float_t &tof){
////////////////////////////////////////////////////////////////////////
//     Returns the position and time of flight of this hit in the local
// coordinates of this module, and in the units of the Monte Carlo.
////////////////////////////////////////////////////////////////////////
    IlcTARGETgeom *gm = ((IlcTARGET*)gIlc->GetDetector("TARGET"))->GetTARGETgeom2();
    Float_t g[3],l[3];

    g[0] = fX;
    g[1] = fY;
    g[2] = fZ;
    if(gm) {
      gm->GtoL(fLayer,fLadder,fDet,g,l);
      x = l[0];
      y = l[1];
      z = l[2];
    } else {
      Error("IlcTARGEThit","NULL pointer to the geometry! return smth else",gm);
      // IlcTARGETv7 - SDD case
      x=fX;
      y=fY;
      z=fZ;
    }
    tof = fTof;
    return;
}
//______________________________________________________________________
void IlcTARGEThit::GetPositionL0(Double_t &x,Double_t &y,Double_t &z,
			      Double_t &tof){
////////////////////////////////////////////////////////////////////////
//     Returns the initial position and time of flight of this hit in the local
// coordinates of this module, and in the units of the Monte Carlo.
////////////////////////////////////////////////////////////////////////
    IlcTARGETgeom *gm = ((IlcTARGET*)gIlc->GetDetector("TARGET"))->GetTARGETgeom2();
    Float_t g[3],l[3];

    g[0] = fx0;
    g[1] = fy0;
    g[2] = fz0;
    if(gm) {
      gm->GtoL(fLayer,fLadder,fDet,g,l);
      x = l[0];
      y = l[1];
      z = l[2];
    } else {
      Error("IlcTARGEThit","NULL pointer to the geometry! return smth else",gm);
      // IlcTARGETv7 - SDD case
      x=fx0;
      y=fy0;
      z=fz0;
    }
    tof = ft0;
    return;
}
//______________________________________________________________________
Float_t IlcTARGEThit::GetXL(){
////////////////////////////////////////////////////////////////////////
//     Returns the x position of this hit in the local coordinates of this
// module, and in the units of the Monte Carlo.
////////////////////////////////////////////////////////////////////////
    IlcTARGETgeom *gm = ((IlcTARGET*)gIlc->GetDetector("TARGET"))->GetTARGETgeom2();
    Float_t g[3],l[3];

    g[0] = fX;
    g[1] = fY;
    g[2] = fZ;
    if(gm) {
      gm->GtoL(fLayer,fLadder,fDet,g,l);
      return l[0];
    } else {
      Error("IlcTARGEThit","NULL pointer to the geometry! return smth else",gm);
      return fX;
    }
}
//______________________________________________________________________
Float_t IlcTARGEThit::GetYL(){
////////////////////////////////////////////////////////////////////////
//     Returns the y position of this hit in the local coordinates of this
// module, and in the units of the Monte Carlo.
////////////////////////////////////////////////////////////////////////
    IlcTARGETgeom *gm = ((IlcTARGET*)gIlc->GetDetector("TARGET"))->GetTARGETgeom2();
    Float_t g[3],l[3];

    g[0] = fX;
    g[1] = fY;
    g[2] = fZ;
    if (gm) {
      gm->GtoL(fLayer,fLadder,fDet,g,l);
      return l[1];
    } else {
      Error("IlcTARGEThit","NULL pointer to the geometry! return smth else",gm);
      return fZ;
    }
}
//______________________________________________________________________
Float_t IlcTARGEThit::GetZL(){
////////////////////////////////////////////////////////////////////////
//     Returns the z position of this hit in the local coordinates of this
// module, and in the units of the Monte Carlo.
////////////////////////////////////////////////////////////////////////
    IlcTARGETgeom *gm = ((IlcTARGET*)gIlc->GetDetector("TARGET"))->GetTARGETgeom2();
    Float_t g[3],l[3];

    g[0] = fX;
    g[1] = fY;
    g[2] = fZ;
    if(gm) {
      gm->GtoL(fLayer,fLadder,fDet,g,l);
      return l[2];
    } else {
      Error("IlcTARGEThit","NULL pointer to the geometry! return smth else",gm);
      return fY;
    }
}
//______________________________________________________________________
void IlcTARGEThit::GetMomentumL(Float_t &px,Float_t &py,Float_t &pz){
////////////////////////////////////////////////////////////////////////
//     Returns the momentum of this hit in the local coordinates of this
// module, and in the units of the Monte Carlo.
////////////////////////////////////////////////////////////////////////
    IlcTARGETgeom *gm = ((IlcTARGET*)gIlc->GetDetector("TARGET"))->GetTARGETgeom2();
    Float_t g[3],l[3];

    g[0] = fPx;
    g[1] = fPy;
    g[2] = fPz;
    if (gm) {
      gm->GtoLMomentum(fLayer,fLadder,fDet,g,l);
      px = l[0];
      py = l[1];
      pz = l[2];
    } else {
      Error("IlcTARGEThit","NULL pointer to the geometry! return smth else",gm);
      px=fPx;
      py=fPy;
      pz=fPz;
    }
    return;
}
//______________________________________________________________________
Float_t IlcTARGEThit::GetPXL(){
////////////////////////////////////////////////////////////////////////
//     Returns the X momentum of this hit in the local coordinates of this
// module, and in the units of the Monte Carlo.
////////////////////////////////////////////////////////////////////////
    IlcTARGETgeom *gm = ((IlcTARGET*)gIlc->GetDetector("TARGET"))->GetTARGETgeom2();
    Float_t g[3],l[3];

    g[0] = fPx;
    g[1] = fPy;
    g[2] = fPz;
    if (gm) {
      gm->GtoLMomentum(fLayer,fLadder,fDet,g,l);
      return l[0];
    } else {
      Error("IlcTARGEThit","NULL pointer to the geometry! return smth else",gm);
      return fPx;
    }
}
//______________________________________________________________________
Float_t IlcTARGEThit::GetPYL(){
////////////////////////////////////////////////////////////////////////
//     Returns the Y momentum of this hit in the local coordinates of this
// module, and in the units of the Monte Carlo.
////////////////////////////////////////////////////////////////////////
    IlcTARGETgeom *gm = ((IlcTARGET*)gIlc->GetDetector("TARGET"))->GetTARGETgeom2();
    Float_t g[3],l[3];

    g[0] = fPx;
    g[1] = fPy;
    g[2] = fPz;
    if (gm) {
      gm->GtoLMomentum(fLayer,fLadder,fDet,g,l);
      return l[1];
    } else {
      Error("IlcTARGEThit","NULL pointer to the geometry! return smth else",gm);
      return fPy;
    }

}
//______________________________________________________________________
Float_t IlcTARGEThit::GetPZL(){
////////////////////////////////////////////////////////////////////////
//     Returns the Z momentum of this hit in the local coordinates of this
// module, and in the units of the Monte Carlo.
////////////////////////////////////////////////////////////////////////
    IlcTARGETgeom *gm = ((IlcTARGET*)gIlc->GetDetector("TARGET"))->GetTARGETgeom2();
    Float_t g[3],l[3];

    g[0] = fPx;
    g[1] = fPy;
    g[2] = fPz;
    if (gm) {
      gm->GtoLMomentum(fLayer,fLadder,fDet,g,l);
      return l[2];
    } else {
      Error("IlcTARGEThit","NULL pointer to the geometry! return smth else",gm);
      return fPz;
    }

}
//___________________________________________________________________________;
Int_t IlcTARGEThit::GetModule(){
////////////////////////////////////////////////////////////////////////
//     Returns the module index number of the module where this hit was in.
////////////////////////////////////////////////////////////////////////
    IlcTARGETgeom *gm = ((IlcTARGET*)gIlc->GetDetector("TARGET"))->GetTARGETgeom2();

    if (gm) return gm->GetModuleIndex(fLayer,fLadder,fDet);
    else {
      Error("IlcTARGEThit","NULL pointer to the geometry! return smth else",gm);
      return 0;
    }
}
//______________________________________________________________________
TParticle * IlcTARGEThit::GetParticle() const {
////////////////////////////////////////////////////////////////////////
//     Returns the pointer to the TParticle for the particle that created
// this hit. From the TParticle all kinds of information about this 
// particle can be found. See the TParticle class.
////////////////////////////////////////////////////////////////////////
    return gIlc->GetMCApp()->Particle(GetTrack());
}  
//----------------------------------------------------------------------
void IlcTARGEThit::Print(ostream *os) const {
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
 
    fmt = os->setf(ios::scientific);  // set scientific floating point output
    *os << fTrack << " " << fX << " " << fY << " " << fZ << " ";
    fmt = os->setf(ios::hex); // set hex for fStatus only.
    *os << fStatus << " ";
    fmt = os->setf(ios::dec); // every thing else decimel.
    *os << fLayer << " " << fLadder << " " << fDet << " ";;
    *os << fPx << " " << fPy << " " << fPz << " ";
    *os << fDestep << " " << fTof;
    *os << " " << fx0 << " " << fy0 << " " << fz0;
//    *os << " " << endl;
    os->flags(fmt); // reset back to old formating.
    return;
}
//----------------------------------------------------------------------
void IlcTARGEThit::Read(istream *is) {
////////////////////////////////////////////////////////////////////////
// Standard input format for this class.
////////////////////////////////////////////////////////////////////////
 

    *is >> fTrack >> fX >> fY >> fZ;
    *is >> fStatus >> fLayer >> fLadder >> fDet >> fPx >> fPy >> fPz >>
	   fDestep >> fTof;
    *is >> fx0 >> fy0 >> fz0;
    return;
}
//----------------------------------------------------------------------
ostream &operator<<(ostream &os,IlcTARGEThit &p){
////////////////////////////////////////////////////////////////////////
// Standard output streaming function.
////////////////////////////////////////////////////////////////////////
 
    p.Print(&os);
    return os;
}
//----------------------------------------------------------------------
istream &operator>>(istream &is,IlcTARGEThit &r){
////////////////////////////////////////////////////////////////////////
// Standard input streaming function.
////////////////////////////////////////////////////////////////////////
 
    r.Read(&is);
    return is;
}
//----------------------------------------------------------------------
