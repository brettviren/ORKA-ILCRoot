#ifndef ILCANALYSISTASKPI0CILCBSELECTION_H
#define ILCANALYSISTASKPI0CILCBSELECTION_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//---------------------------------------------------------------------------// 
// Fill histograms with two-cluster invariant mass                           //
// using calibration coefficients of the previous iteration.                 //
//---------------------------------------------------------------------------//


#include "IlcAnalysisTaskSE.h"
#include "IlcPVBARGeometry.h"
#include "IlcPVBARCalibData.h"
#include "IlcESDCaloCluster.h"
#include "IlcESDCaloCells.h"
#include "TH1.h"

class IlcAnalysisTaskPi0CalibSelection : public IlcAnalysisTaskSE
{
public:

  IlcAnalysisTaskPi0CalibSelection();
  IlcAnalysisTaskPi0CalibSelection(const char* name);
  virtual ~IlcAnalysisTaskPi0CalibSelection();

  // Implementation of interface methods
  virtual void UserCreateOutputObjects();
  virtual void UserExec(Option_t * opt);
  
  void SetClusterMinEnergy(Float_t emin) {fEmin=emin;}
  void SetLogWeight(Float_t weight) {fLogWeight=weight;}
  void SetCalibCorrections(IlcPVBARCalibData* cdata);
  
private:

  IlcAnalysisTaskPi0CalibSelection(const IlcAnalysisTaskPi0CalibSelection&); 
  IlcAnalysisTaskPi0CalibSelection& operator=(const IlcAnalysisTaskPi0CalibSelection&); 

  void MaxEnergyCellPos(IlcESDCaloCells *cells, IlcESDCaloCluster* clu, Int_t& maxId);

private:

  TList* fOutputContainer;
  TH1F*  fHmpi0[5][64][56];// two-cluster inv. mass assigned to each cell.
  
  IlcPVBARGeometry * fPVBARGeo;   // PVBAR geometry
  IlcPVBARCalibData* fCalibData; // corrections to CC from the previous iteration

  TH1F* fHmgg; //two-cluster inv.mass
  Float_t fEmin; // min. cluster energy
  Float_t fLogWeight; // log weight used in cluster recalibration

  ClassDef(IlcAnalysisTaskPi0CalibSelection,1);

};

#endif
