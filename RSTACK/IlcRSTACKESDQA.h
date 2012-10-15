#ifndef ILCRSTACKESDQA_H
#define ILCRSTACKESDQA_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice     */
//______________________________________________________________________________
// An analysis task to check the RSTACK photon ESD data 
//
//*-- Yves Schutz 
//////////////////////////////////////////////////////////////////////////////

#include "IlcAnalysisTask.h"  

class IlcESD ; 
class TH1D ; 
class TH1I ; 

class IlcRSTACKESDQA : public IlcAnalysisTask {

public:
  IlcRSTACKESDQA(const char *name) ;
  virtual ~IlcRSTACKESDQA() ;
   
  virtual void Exec(Option_t * opt = "") ;
  virtual void ConnectInputData(Option_t *);
  virtual void CreateOutputObjects();
  virtual void Terminate(Option_t * opt = "") ;

private:
  TTree   * fChain ;            //!pointer to the analyzed TTree or TChain
  IlcESD  * fESD ;              //! Declaration of leave types

  TObjArray * fOutputContainer ; //! output data container

  // Histograms
  TNtuple * fhRSTACKPos ;
  TNtuple * fhRSTACK ;
  TH1D    * fhRSTACKEnergy ;
  TH1I    * fhRSTACKDigits ;
  TH1D    * fhRSTACKRecParticles ;
  TH1I    * fhRSTACKPhotons ;
  TH1D    * fhRSTACKInvariantMass ;
  TH1I    * fhRSTACKDigitsEvent ;
   
  ClassDef(IlcRSTACKESDQA, 0); // a RSTACK photon analysis task 
};
#endif // ILCRSTACKESDQA_H
