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

/* $Id$ */

//--------------------------------------------------
// CalTrk is used to store Tracks and CaloCells information 
//
// Author: alexandre.shabetai@cern.ch & magali.estienne@subatech.in2p3.fr
//-------------------------------------------------

#include "IlcJetCalTrk.h"

#include "IlcVCaloCells.h"

ClassImp(IlcJetCalTrkTrack)

//////////////////////////////////////////////////////////////////

IlcJetCalTrkTrack::IlcJetCalTrkTrack():
  TObject(),
  fCalTrkTrackRef(),
  fCalTrkCutFlag(kFALSE),
  fCalTrkSignalFlag(kFALSE), 
  fCalTrkPtCorr(1.)
{
  // Default constructor
}  

//------------------------------------------------------------------------
IlcJetCalTrkTrack::IlcJetCalTrkTrack(IlcVTrack* track, Bool_t cutFlag, Bool_t signalFlag, Float_t ptCorr):
  TObject(),
  fCalTrkTrackRef(track),
  fCalTrkCutFlag(cutFlag),
  fCalTrkSignalFlag(signalFlag),
  fCalTrkPtCorr(ptCorr)
{
  // Constructor 2
}

//------------------------------------------------------------------------
IlcJetCalTrkTrack::IlcJetCalTrkTrack(IlcVParticle* track, Bool_t cutFlag, Bool_t signalFlag, Float_t ptCorr):
  TObject(),
  fCalTrkTrackRef(track),
  fCalTrkCutFlag(cutFlag),
  fCalTrkSignalFlag(signalFlag),
  fCalTrkPtCorr(ptCorr)
{
  // Constructor 3
}

//------------------------------------------------------------------------
IlcJetCalTrkTrack::IlcJetCalTrkTrack(const IlcJetCalTrkTrack& rCalTrk):
  TObject(rCalTrk),
  fCalTrkTrackRef(rCalTrk.fCalTrkTrackRef),
  fCalTrkCutFlag(rCalTrk.fCalTrkCutFlag),
  fCalTrkSignalFlag(rCalTrk.fCalTrkSignalFlag),
  fCalTrkPtCorr(rCalTrk.fCalTrkPtCorr)
{
  // Copy constructor
}

//------------------------------------------------------------------------
IlcJetCalTrkTrack& IlcJetCalTrkTrack::operator=(const IlcJetCalTrkTrack& rhs)
{
  // Assignment
  if (this != &rhs) {
   TObject::operator=(rhs);
   fCalTrkTrackRef   = rhs.fCalTrkTrackRef;
   fCalTrkCutFlag    = rhs.fCalTrkCutFlag;
   fCalTrkSignalFlag = rhs.fCalTrkSignalFlag;
   fCalTrkPtCorr     = rhs.fCalTrkPtCorr;
 }
  return *this;

}

//------------------------------------------------------------------------
void IlcJetCalTrkTrack::Clear(Option_t* /*option*/)
{
  // Clear objects
  fCalTrkTrackRef   = 0;
  fCalTrkCutFlag    = 0;
  fCalTrkSignalFlag = 0;
  fCalTrkPtCorr     = 1.;
 
}

//-----------------------------------------------------------------------
void IlcJetCalTrkTrack::Print(const Option_t* option) const
{
  cout << "Track: " << option << ", Pt: " << GetPt() << ", Eta: " << GetEta() << ", Phi: " << GetPhi() << endl;

}

//...........................................................................
//***************************************************************************
ClassImp(IlcJetCalTrkTrackKine)

  IlcJetCalTrkTrackKine::IlcJetCalTrkTrackKine():
    IlcJetCalTrkTrack(),
    fCalTrkPtReso(1.),
    fCalTrkTrackE(-999.),
    fCalTrkTrackPt(-999.),
    fCalTrkTrackP(-999.),
    fCalTrkTrackPx(-999.),
    fCalTrkTrackPy(-999.),
    fCalTrkTrackPz(-999.)
{
  // Default constructor
}

//------------------------------------------------------------------------
IlcJetCalTrkTrackKine::IlcJetCalTrkTrackKine(IlcVParticle* track, Bool_t cutFlag, Bool_t signalFlag, Float_t ptReso) : 
  IlcJetCalTrkTrack(track,cutFlag,signalFlag),
  fCalTrkPtReso(ptReso),
  fCalTrkTrackE(-999.),
  fCalTrkTrackPt(-999.),
  fCalTrkTrackP(-999.),
  fCalTrkTrackPx(-999.),
  fCalTrkTrackPy(-999.),
  fCalTrkTrackPz(-999.)
{
  // Constructor 2
  CalcPx(); CalcPy(); CalcPz(); CalcP(); CalcPt(); CalcE();

}

//------------------------------------------------------------------------
void IlcJetCalTrkTrackKine::Clear(Option_t* option)
{
  // Clear objects
  fCalTrkPtReso     = 1.;
  fCalTrkTrackE     = -999;
  fCalTrkTrackPt    = -999;
  fCalTrkTrackP     = -999;
  fCalTrkTrackPx    = -999;
  fCalTrkTrackPy    = -999;
  fCalTrkTrackPz    = -999;
  IlcJetCalTrkTrack::Clear(option);

}

//------------------------------------------------------------------------
Float_t IlcJetCalTrkTrackKine::CalcE()
{
  // Particle energy
  if(fCalTrkTrackE==-999){
    if ( fCalTrkPtReso != 1 ){
      fCalTrkTrackE = TMath::Sqrt(GetPx()*GetPx()+GetPy()*GetPy()+GetPz()*GetPz()+GetM()*GetM());
    }
    else {fCalTrkTrackE = GetParticle()->E(); }
  }
  
  return fCalTrkTrackE;
  
}

//------------------------------------------------------------------------
Float_t IlcJetCalTrkTrackKine::CalcPt()
{
  // Particle transverse momentum
  if(fCalTrkTrackPt==-999){
    if ( fCalTrkPtReso != 1 ){
      fCalTrkTrackPt = TMath::Sqrt(GetPx()*GetPx()+GetPy()*GetPy());
    }
    else {fCalTrkTrackPt = GetParticle()->Pt();}
  }

  return fCalTrkTrackPt;

}

//------------------------------------------------------------------------
Float_t IlcJetCalTrkTrackKine::CalcP()
{
  // Particle momentum
  if(fCalTrkTrackP==-999){
    if ( fCalTrkPtReso != 1 ){
      fCalTrkTrackP = TMath::Sqrt(GetPx()*GetPx()+GetPy()*GetPy()+GetPz()*GetPz());
    }
    else {fCalTrkTrackP = GetParticle()->P(); }
  }
  
  return fCalTrkTrackP;

}

//...........................................................................
//***************************************************************************

ClassImp(IlcJetCalTrkEvent)

  IlcJetCalTrkEvent::IlcJetCalTrkEvent():
    TObject(),
    fJetCalTrkTrack(0x0),
    fJetCalTrkCell(0x0),
    fNJetCalTrkTrack(0)
{
  // Default constructor
}

//----------------------------------------------------------------
IlcJetCalTrkEvent::IlcJetCalTrkEvent(Short_t opt,Bool_t kine,Bool_t kIsHighMult):
  TObject(),
  fJetCalTrkTrack(0x0),
  fJetCalTrkCell(0x0),
  fNJetCalTrkTrack(0)
{
  // Constructor 2
  if (kine==0) {
    // Tracks (real or MC)
    if(opt%2==!0 || opt==0){
      fJetCalTrkTrack = new TClonesArray("IlcJetCalTrkTrack", kIsHighMult*3800+200);
    }
  }
  else { // Kine cases
    fJetCalTrkTrack = new TClonesArray("IlcJetCalTrkTrackKine", kIsHighMult*3800+200);
  }

}

//----------------------------------------------------------------
IlcJetCalTrkEvent::~IlcJetCalTrkEvent()
{
 // destructor
 if (fJetCalTrkTrack) delete fJetCalTrkTrack;
 if (fJetCalTrkCell)  delete fJetCalTrkCell;

}
//----------------------------------------------------------------
IlcJetCalTrkEvent::IlcJetCalTrkEvent(const IlcJetCalTrkEvent& rCalTrkEvent):
  TObject(),
  fJetCalTrkTrack(rCalTrkEvent.fJetCalTrkTrack),
  fJetCalTrkCell(rCalTrkEvent.fJetCalTrkCell),
  fNJetCalTrkTrack(rCalTrkEvent.fNJetCalTrkTrack)
{
  // Copy constructor
}

//----------------------------------------------------------------
IlcJetCalTrkEvent& IlcJetCalTrkEvent::operator=(const IlcJetCalTrkEvent& rhs)
{
  // Assignment
  if (this != &rhs) {
   TObject::operator=(rhs);
   if (fJetCalTrkTrack) delete fJetCalTrkTrack; 
   if (fJetCalTrkCell) delete  fJetCalTrkCell;
   fJetCalTrkTrack  = rhs.fJetCalTrkTrack;
   fJetCalTrkCell   = rhs.fJetCalTrkCell;
   fNJetCalTrkTrack = rhs.fNJetCalTrkTrack;
  }
  
  return *this;

}

//----------------------------------------------------------------
IlcJetCalTrkTrack* IlcJetCalTrkEvent::AddCalTrkTrack(IlcVTrack* track, Bool_t cutFlag, Bool_t signalFlag, Float_t ptCorr)
{
  // Add a track to the CalTrkEvent  
  TClonesArray &tJetCalTrkTrack = *fJetCalTrkTrack ;
  IlcJetCalTrkTrack *n = new(tJetCalTrkTrack[fNJetCalTrkTrack++]) IlcJetCalTrkTrack(track, cutFlag, signalFlag, ptCorr) ;
  return n ;

}

//----------------------------------------------------------------
IlcJetCalTrkTrack* IlcJetCalTrkEvent::AddCalTrkTrack(IlcVParticle* track, Bool_t cutFlag, Bool_t signalFlag, Float_t ptCorr)
{
  // Add a track to the CalTrkEvent
  TClonesArray &tJetCalTrkTrack = *fJetCalTrkTrack ;
  IlcJetCalTrkTrack *n = new(tJetCalTrkTrack[fNJetCalTrkTrack++]) IlcJetCalTrkTrack(track, cutFlag, signalFlag, ptCorr) ;
  return n ;

}

//_________________________________________________________________
IlcJetCalTrkTrackKine* IlcJetCalTrkEvent::AddCalTrkTrackKine(IlcVParticle* track, Bool_t cutFlag, Bool_t signalFlag, Float_t ptReso)
{
  // Add a track to the CalTrkEvent
  TClonesArray &tJetCalTrkTrack = *fJetCalTrkTrack ;
  IlcJetCalTrkTrackKine *n = new(tJetCalTrkTrack[fNJetCalTrkTrack++]) IlcJetCalTrkTrackKine(track, cutFlag, signalFlag, ptReso) ;
  return n ;

}


//----------------------------------------------------------------
IlcJetCalTrkTrack* IlcJetCalTrkEvent::GetCalTrkTrack(Int_t i)
{
  // Get track i
  return (IlcJetCalTrkTrack*) fJetCalTrkTrack->At(i);

}

//-----------------------------------------------------------------
void IlcJetCalTrkEvent::Clear(Option_t* /*option*/)
{
  // Clear object

  if(fJetCalTrkTrack)  fJetCalTrkTrack->Clear("C"); // array of Tracks
  fNJetCalTrkTrack     = 0; // Number of tracks
}

//________________________________________________________________
void  IlcJetCalTrkEvent::Print(const Option_t* /*option*/) const
{
  // prints event information
  cout<< "Number of tracks:" << fNJetCalTrkTrack << endl;

}
