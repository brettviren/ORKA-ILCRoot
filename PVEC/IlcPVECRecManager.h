#ifndef IlcPVECRecManager_H
#define IlcPVECRecManager_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//_________________________________________________________________________
// Base class for the management by the PVEC reconstruction.
// It contains only virtual member functions
// which will be implemented for the Emc and CPV reconstruction
// in the appropriate derived classes.
// Author  : Boris Polichtchouk (IHEP, Protvino)
// 6 March 2001

#include "TNamed.h"

class IlcPVECRecManager : public TNamed {

 public:

  IlcPVECRecManager();
  virtual ~IlcPVECRecManager(void) {}

  virtual void AG(Float_t e, Float_t dx, Float_t dy, Float_t& A, Float_t& gradX, Float_t& gradY ) = 0;

  virtual Float_t OneGamChi2(Float_t ai, Float_t ei, Float_t etot, Float_t& gi) const = 0 ;
  virtual Float_t TwoGamChi2(Float_t ai, Float_t ei, Float_t etot, Float_t& gi) const = 0;

  virtual Float_t OneGamChisqCut()const = 0 ;
  virtual Float_t OneGamInitialStep() const = 0;
  virtual Float_t OneGamChisqMin() const = 0;
  virtual Float_t OneGamStepMin() const = 0;
  virtual Int_t OneGamNumOfIterations() const = 0;

  virtual Float_t TwoGamInitialStep() const = 0;
  virtual Float_t TwoGamChisqMin() const = 0;
  virtual Float_t TwoGamEmin() const = 0;
  virtual Float_t TwoGamStepMin() const = 0;
  virtual Int_t TwoGamNumOfIterations() const = 0;

  virtual Float_t KillGamMinEnergy() const = 0;
  virtual Float_t MergeGammasMinDistanceCut() const = 0;

  virtual void SetTwoPointsMinDistance(Float_t dist) = 0;
  virtual void SetPointMinEnergy(Float_t emin) = 0;

  ClassDef(IlcPVECRecManager,1)

} ;

#endif // IlcPVECRecManager_H













