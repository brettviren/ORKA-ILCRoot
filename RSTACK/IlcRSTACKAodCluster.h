#ifndef ILCRSTACKAODCLUSTER_H
#define ILCRSTACKAODCLUSTER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */


//_________________________________________________________________________
//  AliAodCaloCluster version for RSTACK (used for recalibration)
//           
//*-- Author: Dmitri Peressounko (RRC KI)

// --- ROOT system ---

// --- Standard library ---

// --- IlcRoot header files ---
class IlcRSTACKPIDv1 ;
class IlcRSTACKCalibData ;
class IlcAODCaloCells ;

#include "IlcAODCaloCluster.h"

class IlcRSTACKAodCluster : public IlcAODCaloCluster  {

public:

  IlcRSTACKAodCluster() ;
  IlcRSTACKAodCluster(const IlcAODCaloCluster & clu) ; 
 
  virtual ~IlcRSTACKAodCluster() ;  

  void  EvalAll(Float_t logWeight, TVector3 &vtx) ; //re-calculate all cluster parameters
  void  Recalibrate(IlcRSTACKCalibData * calibData,IlcAODCaloCells *phsCells) ; //Apply recalibration to this cluster
  void  EnergyCorrection() ;  //Apply non-linearity correction
  void  EvalPID(IlcRSTACKPIDv1 * pid) ;           //re-evaluate identification parameters

protected:
 
  void EvalCoord(Float_t logWeight, TVector3 &vtx) ;  //calculate coordinate-related parameters (position, dispersion)
  void EvalEnergy() ; //re-calculate energy of the cluster

  Bool_t fRecalibrated ;  //Have this cluster been recalibrated
    
  ClassDef(IlcRSTACKAodCluster,1)  // (RSTACK AOD cluster)

};

#endif // IlcRSTACKAODCLUSTER_H
