#ifndef ILCPVBARESDQA_H
#define ILCPVBARESDQA_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice     */
//______________________________________________________________________________
// An analysis task to check the PVBAR photon ESD data 
//
//*-- Yves Schutz 
//////////////////////////////////////////////////////////////////////////////

#include "IlcAnalysisTask.h"  

class IlcESD ; 
class TH1D ; 
class TH1I ; 

class IlcPVBARESDQA : public IlcAnalysisTask {

public:
  IlcPVBARESDQA(const char *name) ;
  virtual ~IlcPVBARESDQA() ;
   
  virtual void Exec(Option_t * opt = "") ;
  virtual void ConnectInputData(Option_t *);
  virtual void CreateOutputObjects();
  virtual void Terminate(Option_t * opt = "") ;

private:
  TTree   * fChain ;            //!pointer to the analyzed TTree or TChain
  IlcESD  * fESD ;              //! Declaration of leave types

  TObjArray * fOutputContainer ; //! output data container

  // Histograms
  TNtuple * fhPVBARPos ;
  TNtuple * fhPVBAR ;
  TH1D    * fhPVBAREnergy ;
  TH1I    * fhPVBARDigits ;
  TH1D    * fhPVBARRecParticles ;
  TH1I    * fhPVBARPhotons ;
  TH1D    * fhPVBARInvariantMass ;
  TH1I    * fhPVBARDigitsEvent ;
   
  ClassDef(IlcPVBARESDQA, 0); // a PVBAR photon analysis task 
};
#endif // ILCPVBARESDQA_H
