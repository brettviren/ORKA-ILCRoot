#ifndef ILCNEUTRALTRACKPARAM_H
#define ILCNEUTRALTRACKPARAM_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/*****************************************************************************
 *              "Neutral" track parametrisation class                        *
 * At the moment we use a standard IlcExternalTrackParam with 0 curvature.   *
 *                                                                           *
 *        Origin: A.Dainese, I.Belikov                                       *
 *****************************************************************************/

#include "IlcExternalTrackParam.h"

class IlcNeutralTrackParam: public IlcExternalTrackParam {
 public:
  IlcNeutralTrackParam();
  IlcNeutralTrackParam(const IlcNeutralTrackParam &);
  IlcNeutralTrackParam& operator=(const IlcNeutralTrackParam & trkPar);
  IlcNeutralTrackParam(Double_t x, Double_t alpha, 
			const Double_t param[5], const Double_t covar[15]);
  IlcNeutralTrackParam(const IlcVTrack *vTrack);
  IlcNeutralTrackParam(Double_t xyz[3],Double_t pxpypz[3],
			Double_t cv[21],Short_t sign);
  virtual ~IlcNeutralTrackParam(){}

  virtual Short_t  Charge() const { return 0; }
  virtual Double_t GetC(Double_t /*b*/) const { return 0.; }

 private:

  ClassDef(IlcNeutralTrackParam, 1) // track with zero charge
};

#endif
