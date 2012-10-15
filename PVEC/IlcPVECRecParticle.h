#ifndef ILCPVECRECPARTICLE_H
#define ILCPVECRECPARTICLE_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPVECRecParticle.h 53186 2011-11-25 14:28:01Z kharlov $ */

//_________________________________________________________________________
//  A Reconstructed Particle in PVEC    
//  To become a general class of IlcRoot ?        
//  why not      
//*-- Author: Yves Schutz (SUBATECH)

// --- ROOT system ---

// --- Standard library ---

// --- IlcRoot header files ---

#include "IlcPVECFastRecParticle.h"

class TParticle ;
#include  "TVector3.h"  

class IlcPVECRecParticle : public IlcPVECFastRecParticle {

 public:
  
  IlcPVECRecParticle() ; 
  IlcPVECRecParticle(const IlcPVECRecParticle & rp) ;  // ctor
  virtual ~IlcPVECRecParticle(){  }

  Int_t   GetPVECTSIndex()const {    return fPVECTrackSegment ;  }
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
  void    SetTrackSegment(Int_t index){fPVECTrackSegment = index; }

  typedef TClonesArray RecParticlesList ; 
  
private:
  IlcPVECRecParticle & operator = (const IlcPVECRecParticle & /*rp*/);

private:

  Int_t fPVECTrackSegment ; // pointer to the associated track segment in PVEC  
  Bool_t fDebug ; // to steer debug output
  TVector3 fPos ; // position in the global ilc coordinate system 

  ClassDef(IlcPVECRecParticle,3)  // Reconstructed Particle
};

#endif // IlcPVECRECPARTICLE_H
