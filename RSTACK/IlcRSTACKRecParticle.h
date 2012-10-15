#ifndef ILCRSTACKRECPARTICLE_H
#define ILCRSTACKRECPARTICLE_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcRSTACKRecParticle.h 53186 2011-11-25 14:28:01Z kharlov $ */

//_________________________________________________________________________
//  A Reconstructed Particle in RSTACK    
//  To become a general class of IlcRoot ?        
//  why not      
//*-- Author: Yves Schutz (SUBATECH)

// --- ROOT system ---

// --- Standard library ---

// --- IlcRoot header files ---

#include "IlcRSTACKFastRecParticle.h"

class TParticle ;
#include  "TVector3.h"  

class IlcRSTACKRecParticle : public IlcRSTACKFastRecParticle {

 public:
  
  IlcRSTACKRecParticle() ; 
  IlcRSTACKRecParticle(const IlcRSTACKRecParticle & rp) ;  // ctor
  virtual ~IlcRSTACKRecParticle(){  }

  Int_t   GetRSTACKTSIndex()const {    return fRSTACKTrackSegment ;  }
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
  void    SetTrackSegment(Int_t index){fRSTACKTrackSegment = index; }

  typedef TClonesArray RecParticlesList ; 
  
private:
  IlcRSTACKRecParticle & operator = (const IlcRSTACKRecParticle & /*rp*/);

private:

  Int_t fRSTACKTrackSegment ; // pointer to the associated track segment in RSTACK  
  Bool_t fDebug ; // to steer debug output
  TVector3 fPos ; // position in the global ilc coordinate system 

  ClassDef(IlcRSTACKRecParticle,3)  // Reconstructed Particle
};

#endif // IlcRSTACKRECPARTICLE_H
