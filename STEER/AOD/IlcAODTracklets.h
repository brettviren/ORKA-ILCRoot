/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcAODTracklets.h 50615 2011-07-16 23:19:19Z hristov $ */

//-------------------------------------------------------------------------
//     AOD class to store tracklets
//     Author: Jan Fiete Grosse-Oetringhaus, CERN
//     Class created from IlcMultiplicity
//-------------------------------------------------------------------------

#ifndef ILCAODTRACKLETS_H
#define ILCAODTRACKLETS_H

#include <TNamed.h>

class IlcAODTracklets : public TNamed 
{
 public:
  IlcAODTracklets();
  IlcAODTracklets(const char* name, const char* title);
  IlcAODTracklets(const IlcAODTracklets& evt); 
  IlcAODTracklets& operator=(const IlcAODTracklets& evt);

  virtual ~IlcAODTracklets();

  void CreateContainer(Int_t nTracks);
  void DeleteContainer();

  Bool_t SetTracklet(Int_t pos, Double32_t theta, Double32_t phi, Double32_t deltaPhi, Int_t labelL1, Int_t labelL2);

  Int_t GetNumberOfTracklets() const { return fNTracks; }
  inline Double32_t GetTheta(Int_t i) const;
  inline Double32_t GetPhi(Int_t i) const;
  inline Double32_t GetDeltaPhi(Int_t i) const;
  inline Int_t   GetLabel(Int_t i, Int_t layer) const;
  inline void    SetLabel(Int_t i, Int_t layer,Int_t label);

 protected:
  Int_t      fNTracks;       // Number of tracklets
  Double32_t *fTheta;        //[fNTracks] array with theta values
  Double32_t *fPhi;          //[fNTracks] array with phi values
  Double32_t *fDeltaPhi;     //[fNTracks] array with delta phi values
  Int_t      *fLabels;       //[fNTracks] array with labels of cluster in L1 used for the tracklet
  Int_t      *fLabelsL2;     //[fNTracks] array with labels of cluster in L2 used for the tracklet


  ClassDef(IlcAODTracklets, 3);
};

Double32_t IlcAODTracklets::GetTheta(Int_t i) const 
{ 
  if (i>=0 && i<fNTracks) 
  {
    return fTheta[i];
  }
  else 
    Error("GetTheta","Invalid track number %d",i); return -9999.;
}

Double32_t IlcAODTracklets::GetPhi(Int_t i) const 
{ 
  if (i>=0 && i<fNTracks) 
  {
    return fPhi[i];
  }
  else 
    Error("GetPhi","Invalid track number %d",i); return -9999.;
}

Double32_t IlcAODTracklets::GetDeltaPhi(Int_t i) const 
{
  if (i>=0 && i<fNTracks) 
  {
    return fDeltaPhi[i];
  }
  else 
    Error("GetDeltaPhi","Invalid track number %d",i); return -9999.;
}

Int_t IlcAODTracklets::GetLabel(Int_t i, Int_t layer) const 
{
  if (i>=0 && i<fNTracks) 
  {
    return (layer == 0) ? fLabels[i] : fLabelsL2[i];
  }
  else 
    Error("GetLabel","Invalid track number %d",i); return -9999;
}


void IlcAODTracklets::SetLabel(Int_t i, Int_t layer,Int_t label)  
{
  if (i>=0 && i<fNTracks) 
  {
    if(layer == 0) fLabels[i] = label;
    else fLabelsL2[i] = label;
  }
}


#endif
