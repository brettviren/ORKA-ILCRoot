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

//-----------------------------------------------------------------------
// Author : R. Vernet, Consorzio Cometa - Catania (it)
//-----------------------------------------------------------------------

#ifndef ILCCFRSNTASK_H
#define ILCCFRSNTASK_H

#include "IlcAnalysisTaskSE.h"

class TH1I;
class TParticle ;
class IlcCFManager;

class IlcCFRsnTask : public IlcAnalysisTaskSE {
  public:

  enum {
    kStepGenerated       = 0,
    kStepReconstructible = 1,
    kStepReconstructed   = 2,
    kStepSelected        = 3
  };

  IlcCFRsnTask();
  IlcCFRsnTask(const Char_t* name);
  IlcCFRsnTask& operator= (const IlcCFRsnTask& c);
  IlcCFRsnTask(const IlcCFRsnTask& c);
  virtual ~IlcCFRsnTask();

  // ANALYSIS FRAMEWORK STUFF to loop on data and fill output objects
  void     UserCreateOutputObjects();
  void     UserExec(Option_t *option);
  void     Terminate(Option_t *);
  
  // CORRECTION FRAMEWORK RELATED FUNCTIONS
  void           SetCFManager(IlcCFManager* io) {fCFManager = io;}   // global correction manager
  IlcCFManager * GetCFManager()                 {return fCFManager;} // get corr manager

  void     SetRsnPDG(Int_t code)             {fRsnPDG = code; }            // defines the PDG code of searched resonance
  static   Double_t GetRapidity(Double_t, Double_t) ;  // returns the rapidity of the Resonance (assuming PDG code)


 protected:

  Int_t           fRsnPDG;       //  PDG code of searched resonance
  IlcCFManager   *fCFManager  ;  // pointer to the CF manager

  // Histograms
  //Number of events
  TH1I *fHistEventsProcessed; //! simple histo for monitoring the number of events processed
  
  ClassDef(IlcCFRsnTask,1);
};

#endif
