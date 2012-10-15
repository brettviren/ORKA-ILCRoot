#ifndef ILCPVECESDCLUSTER_H
#define ILCPVECESDCLUSTER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */


//_________________________________________________________________________
//  IlcESDCaloCluster version for PVEC (used for recalibration)
//           
//*-- Author: Dmitri Peressounko (RRC KI)

// --- ROOT system ---

// --- Standard library ---

// --- IlcRoot header files ---
class IlcPVECPIDv1 ;
class IlcPVECCalibData ;
class IlcESDCaloCells ;

#include "IlcESDCaloCluster.h"

class IlcPVECEsdCluster : public IlcESDCaloCluster  {

public:

  IlcPVECEsdCluster() ;
  IlcPVECEsdCluster(const IlcESDCaloCluster & clu) ; 
 
  virtual ~IlcPVECEsdCluster() ;  

  void  EvalAll(Float_t logWeight, TVector3 &vtx) ; //re-calculate all cluster parameters
  void  Recalibrate(IlcPVECCalibData * calibData,IlcESDCaloCells *phsCells) ; //Apply recalibration to this cluster
  void  EnergyCorrection() ;  //Apply non-linearity correction
  void  EvalPID(IlcPVECPIDv1 * pid) ;           //re-evaluate identification parameters

protected:
 
  void EvalCoord(Float_t logWeight, TVector3 &vtx) ;  //calculate coordinate-related parameters (position, dispersion)
  void EvalEnergy() ; //re-calculate energy of the cluster

  Bool_t fRecalibrated ;  //Have this cluster been recalibrated
    
  ClassDef(IlcPVECEsdCluster,3)  // (PVEC ESD cluster)

};

#endif // IlcPVECESDCLUSTER_H
