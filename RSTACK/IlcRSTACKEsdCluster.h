#ifndef ILCRSTACKESDCLUSTER_H
#define ILCRSTACKESDCLUSTER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */


//_________________________________________________________________________
//  IlcESDCaloCluster version for RSTACK (used for recalibration)
//           
//*-- Author: Dmitri Peressounko (RRC KI)

// --- ROOT system ---

// --- Standard library ---

// --- IlcRoot header files ---
class IlcRSTACKPIDv1 ;
class IlcRSTACKCalibData ;
class IlcESDCaloCells ;

#include "IlcESDCaloCluster.h"

class IlcRSTACKEsdCluster : public IlcESDCaloCluster  {

public:

  IlcRSTACKEsdCluster() ;
  IlcRSTACKEsdCluster(const IlcESDCaloCluster & clu) ; 
 
  virtual ~IlcRSTACKEsdCluster() ;  

  void  EvalAll(Float_t logWeight, TVector3 &vtx) ; //re-calculate all cluster parameters
  void  Recalibrate(IlcRSTACKCalibData * calibData,IlcESDCaloCells *phsCells) ; //Apply recalibration to this cluster
  void  EnergyCorrection() ;  //Apply non-linearity correction
  void  EvalPID(IlcRSTACKPIDv1 * pid) ;           //re-evaluate identification parameters

protected:
 
  void EvalCoord(Float_t logWeight, TVector3 &vtx) ;  //calculate coordinate-related parameters (position, dispersion)
  void EvalEnergy() ; //re-calculate energy of the cluster

  Bool_t fRecalibrated ;  //Have this cluster been recalibrated
    
  ClassDef(IlcRSTACKEsdCluster,3)  // (RSTACK ESD cluster)

};

#endif // IlcRSTACKESDCLUSTER_H
