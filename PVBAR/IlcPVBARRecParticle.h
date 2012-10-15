#ifndef ILCPVBARRECPARTICLE_H
#define ILCPVBARRECPARTICLE_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPVBARRecParticle.h 53186 2011-11-25 14:28:01Z kharlov $ */

//_________________________________________________________________________
//  A Reconstructed Particle in PVBAR    
//  To become a general class of IlcRoot ?        
//  why not      
//*-- Author: Yves Schutz (SUBATECH)

// --- ROOT system ---

// --- Standard library ---

// --- IlcRoot header files ---

#include "IlcPVBARFastRecParticle.h"

class TParticle ;
#include  "TVector3.h"  

class IlcPVBARRecParticle : public IlcPVBARFastRecParticle {

 public:
  
  IlcPVBARRecParticle() ; 
  IlcPVBARRecParticle(const IlcPVBARRecParticle & rp) ;  // ctor
  virtual ~IlcPVBARRecParticle(){  }

  Int_t   GetPVBARTSIndex()const {    return fPVBARTrackSegment ;  }
  virtual Int_t GetNPrimariesToRecParticles() const ;
  virtual Int_t GetNPrimaries() const ;
  TVector3 GetPos() const { return fPos ; } 
  virtual const TParticle * GetPrimary(Int_t index) const ;
  virtual const TParticle * GetPrimary() const ;
  Int_t GetPrimaryIndex() const ;
  const Float_t *GetPID() { return fPID ; }
  void    SetDebug() { fDebug = kTRUE ; } 
  void    SetPID(Int_t type, Float_t weight) ; 
  void    SetPos(TVector3 pos) { fPos.SetXYZ( pos.X(), pos.Y(), pos.Z() ); } 
  void    UnsetDebug() { fDebug = kFALSE ; }
  void    SetTrackSegment(Int_t index){fPVBARTrackSegment = index; }

  typedef TClonesArray RecParticlesList ; 
  
private:
  IlcPVBARRecParticle & operator = (const IlcPVBARRecParticle & /*rp*/);

private:

  Int_t fPVBARTrackSegment ; // pointer to the associated track segment in PVBAR  
  Bool_t fDebug ; // to steer debug output
  TVector3 fPos ; // position in the global ilc coordinate system 

  ClassDef(IlcPVBARRecParticle,3)  // Reconstructed Particle
};

#endif // IlcPVBARRECPARTICLE_H
