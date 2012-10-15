#ifndef ILCMEANVERTEX_H
#define ILCMEANVERTEX_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcMeanVertex.h 50615 2011-07-16 23:19:19Z hristov $ */
/*****************************************************************************
 *                                                                           *
 * This class contains the coordinates of the mean primary vertex position   *
 * computed by IlcITSMeanVertex                                              *
 *                                                                           *
*****************************************************************************/
#include "IlcESDVertex.h"

class IlcMeanVertex : public IlcESDVertex {
 public:
  IlcMeanVertex();
  IlcMeanVertex(Double_t pos[3],Double_t err[3],Double_t cov[6],Int_t nevents, Float_t notracklets, Float_t avertracklets, Float_t signotrackl);
  virtual ~IlcMeanVertex() {}

  Int_t GetNumberOfContributingEvents() const { return GetNContributors(); }
  void GetErrorsOnPosition(Double_t err[3]) const;
  Float_t GetTotalNumbOfTracklets() const { return fTotTracklets; }
  Float_t GetAverageNumbOfTracklets() const { return fAverTracklets; }
  Float_t GetSigmaOnAvNumbOfTracks() const { return fSigmaOnAverTrack; }

 protected:
  Double32_t fErrW[3];       // errors on vertex coordinates (weighted average)
  Float_t      fTotTracklets;   // total number of tracklets used for M.V.
  Float_t    fAverTracklets;  // average number of tracklets per event
  Float_t fSigmaOnAverTrack;  // sigma on fAverTracklets

  ClassDef(IlcMeanVertex,1)  // Class for mean Vertex   
}; 

#endif
