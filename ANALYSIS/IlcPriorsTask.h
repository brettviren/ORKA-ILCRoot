/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

#ifndef IlcPriorsTask_H
#define IlcPriorsTask_H

#include "IlcAnalysisTaskSE.h"
#include "IlcPID.h"
class TH1I;
class TH2D;

class IlcPriorsTask : public IlcAnalysisTaskSE {
  public:


  IlcPriorsTask();
  IlcPriorsTask(const Char_t* name);
  IlcPriorsTask& operator= (const IlcPriorsTask& c);
  IlcPriorsTask(const IlcPriorsTask& c);
  virtual ~IlcPriorsTask();

  // ANALYSIS FRAMEWORK STUFF to loop on data and fill output objects
  void     UserCreateOutputObjects();
  void     UserExec(Option_t *option);
  void     Terminate(Option_t *);
  
  void SetPriors(Double_t conc[IlcPID::kSPECIES])        {for(Int_t i=0; i<IlcPID::kSPECIES; i++) fPriors[i]=conc[i];}
  void GetPriors(Double_t conc[IlcPID::kSPECIES]) const  {for(Int_t i=0; i<IlcPID::kSPECIES; i++) conc[i]=fPriors[i];}

  void SetNiterations(Int_t nIter) {fNiterMax = nIter;}

  Bool_t NotifyBinChange(); // method called at the end of the event loop

 protected:
 
  // Histograms

  TH1I  *fHistEventsProcessed; // simple histo for monitoring the number of events processed
  TH2D  *fCalcPriors;          // histo monitoring priors during iterations

  Double_t fPriors[IlcPID::kSPECIES];  // Priors
  Double_t fRecId[IlcPID::kSPECIES];   // Reconstructed Id
  Double_t fMCId[IlcPID::kSPECIES];    // MC Id

  Int_t fNiterations;          //counter  
  Int_t fNiterMax;             //max number of iterations
  ClassDef(IlcPriorsTask,1);
};

#endif
