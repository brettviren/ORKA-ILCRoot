#ifndef ILCPVBARIMPACT_H
#define ILCPVBARIMPACT_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPVBARImpact.h 53186 2011-11-25 14:28:01Z kharlov $ */

/* History of cvs commits:
 *
 * $Log$
 * Revision 1.2  2005/05/28 14:19:04  schutz
 * Compilation warnings fixed by T.P.
 *
 */

//_________________________________________________________________________
//  Hit impact class for PVBAR     
//  A hit impact in PVBAR is a set of parameters of a track which
//  enters the detector for the first time.
//  Track parameters are:
//  - track number
//  - primary particle number
//  - type of a particle
//  - impact coordinate
//  - impact 4-momentum
//               
//*-- Author: Yuri Kharlov (IHEP, Protvino/SUBATECH, Nantes)

// --- ROOT system ---
#include <TLorentzVector.h>

// --- IlcRoot header files ---
#include "IlcHit.h"

// --- Standard library ---

class IlcPVBARImpact : public IlcHit {

public:
  
  IlcPVBARImpact();              // default ctor 
  IlcPVBARImpact(const IlcPVBARImpact & hit) ; 
  IlcPVBARImpact(Int_t shunt, Int_t primary, Int_t track, Int_t pid, TLorentzVector p, Float_t *xyz);
  virtual ~IlcPVBARImpact(void) { } // dtor 
  
  Int_t   GetPid(void)         const { 
    // returns the particle PDG code which initiates this hit
    return fPid ; 
  }
  Int_t   GetPrimary(void)     const { 
    // returns the primary particle id at the origin of this hit 
    return fPrimary ; 
  }
  TLorentzVector GetMomentum() const {
    // returns momentum of the particle which initiated this hit
    return  fMomentum;
  }
  void Print(const Option_t * = "")const;

private:
  IlcPVBARImpact & operator = (const IlcPVBARImpact & /*impact*/);

private:

  Int_t          fPid ;       // type of the particle that initiates that hit 
  Int_t          fPrimary ;   // Primary particles at the origine of the hit
  TLorentzVector fMomentum;   // 4-momentum of the particle

  ClassDef(IlcPVBARImpact,1)  // Hit impact for PVBAR

} ;

//////////////////////////////////////////////////////////////////////////////

#endif // ILCPVBARIMPACT_H
