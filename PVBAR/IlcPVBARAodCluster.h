#ifndef ILCPVBARAODCLUSTER_H
#define ILCPVBARAODCLUSTER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */


//_________________________________________________________________________
//  AliAodCaloCluster version for PVBAR (used for recalibration)
//           
//*-- Author: Dmitri Peressounko (RRC KI)

// --- ROOT system ---

// --- Standard library ---

// --- IlcRoot header files ---
class IlcPVBARPIDv1 ;
class IlcPVBARCalibData ;
class IlcAODCaloCells ;

#include "IlcAODCaloCluster.h"

class IlcPVBARAodCluster : public IlcAODCaloCluster  {

public:

  IlcPVBARAodCluster() ;
  IlcPVBARAodCluster(const IlcAODCaloCluster & clu) ; 
 
  virtual ~IlcPVBARAodCluster() ;  

  void  EvalAll(Float_t logWeight, TVector3 &vtx) ; //re-calculate all cluster parameters
  void  Recalibrate(IlcPVBARCalibData * calibData,IlcAODCaloCells *phsCells) ; //Apply recalibration to this cluster
  void  EnergyCorrection() ;  //Apply non-linearity correction
  void  EvalPID(IlcPVBARPIDv1 * pid) ;           //re-evaluate identification parameters

protected:
 
  void EvalCoord(Float_t logWeight, TVector3 &vtx) ;  //calculate coordinate-related parameters (position, dispersion)
  void EvalEnergy() ; //re-calculate energy of the cluster

  Bool_t fRecalibrated ;  //Have this cluster been recalibrated
    
  ClassDef(IlcPVBARAodCluster,1)  // (PVBAR AOD cluster)

};

#endif // IlcPVBARAODCLUSTER_H
