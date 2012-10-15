#ifndef ILCPVECESDQA_H
#define ILCPVECESDQA_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice     */
//______________________________________________________________________________
// An analysis task to check the PVEC photon ESD data 
//
//*-- Yves Schutz 
//////////////////////////////////////////////////////////////////////////////

#include "IlcAnalysisTask.h"  

class IlcESD ; 
class TH1D ; 
class TH1I ; 

class IlcPVECESDQA : public IlcAnalysisTask {

public:
  IlcPVECESDQA(const char *name) ;
  virtual ~IlcPVECESDQA() ;
   
  virtual void Exec(Option_t * opt = "") ;
  virtual void ConnectInputData(Option_t *);
  virtual void CreateOutputObjects();
  virtual void Terminate(Option_t * opt = "") ;

private:
  TTree   * fChain ;            //!pointer to the analyzed TTree or TChain
  IlcESD  * fESD ;              //! Declaration of leave types

  TObjArray * fOutputContainer ; //! output data container

  // Histograms
  TNtuple * fhPVECPos ;
  TNtuple * fhPVEC ;
  TH1D    * fhPVECEnergy ;
  TH1I    * fhPVECDigits ;
  TH1D    * fhPVECRecParticles ;
  TH1I    * fhPVECPhotons ;
  TH1D    * fhPVECInvariantMass ;
  TH1I    * fhPVECDigitsEvent ;
   
  ClassDef(IlcPVECESDQA, 0); // a PVEC photon analysis task 
};
#endif // ILCPVECESDQA_H
