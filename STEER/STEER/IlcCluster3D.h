#ifndef ILCCLUSTER3D_H
#define ILCCLUSTER3D_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//-------------------------------------------------------------------------
//                         Class IlcCluster3D
//  This is an extension of the IlcCluster class for the case when
//  the sensitive plane this cluster belongs to is arbitrarily oriented
//  in space.  This class can serve as the base for the TOF and HMPID
//  clusters.
//
//  cvetan.cheshkov@cern.ch  & jouri.belikov@cern.ch     5/6/2007
//-------------------------------------------------------------------------

#include "IlcCluster.h"

class TGeoHMatrix;
class TGeoPNEntry;

class IlcCluster3D : public IlcCluster {
public:
  IlcCluster3D();
  IlcCluster3D(UShort_t volId, 
     Float_t x,   Float_t y,   Float_t z,
     Float_t sx2, Float_t sxy, Float_t sxz,
                  Float_t sy2, Float_t syz, 
                               Float_t sz2, const Int_t *lab = NULL);
  IlcCluster3D(const IlcCluster3D& cluster);
  virtual ~IlcCluster3D() {;}

  virtual Bool_t GetGlobalCov(Float_t cov[6]) const;

  Float_t GetSigmaX2() const {return fSigmaX2;}
  Float_t GetSigmaXY() const {return fSigmaXY;}
  Float_t GetSigmaXZ() const {return fSigmaXZ;}

private:
  IlcCluster3D &operator=(const IlcCluster3D& cluster);

  Float_t fSigmaX2;  // Additional elements 
  Float_t fSigmaXY;  // of 
  Float_t fSigmaXZ;  // the covariance matrix 
  
  ClassDef(IlcCluster3D,1) // Barrel detectors cluster
};

#endif
