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
// Author : R. Vernet, INFN - Catania (it)
//-----------------------------------------------------------------------

#ifndef ILCCFTASKFORUNFOLDING_H
#define ILCCFTASKFORUNFOLDING_H

#include "IlcAnalysisTaskSE.h"

class TH1I;
class TParticle ;
class TFile ;
class IlcStack ;
class IlcCFManager;
class IlcESDtrack;
class IlcVParticle;
class THnSparse;

class IlcCFTaskForUnfolding : public IlcAnalysisTaskSE {
  public:

  IlcCFTaskForUnfolding();
  IlcCFTaskForUnfolding(const Char_t* name);
  virtual ~IlcCFTaskForUnfolding();

  // ANALYSIS FRAMEWORK STUFF to loop on data and fill output objects
  void     UserCreateOutputObjects();
  void     UserExec(Option_t *option);
  void     Terminate(Option_t *);
  
  // CORRECTION FRAMEWORK RELATED FUNCTIONS
  void           SetCFManager(IlcCFManager* io) {fCFManager = io;}   // global correction manager
  IlcCFManager * GetCFManager() const {return fCFManager;}           // get corr manager
  void           SetCorrelationMatrix(THnSparse* h) {fCorrelation=h;}

 protected:
  
  IlcCFManager   *fCFManager    ;  // pointer to the CF manager

  // Histograms
  //Number of events
  TH1I       *fHistEventsProcessed; // simple histo for monitoring the number of events processed
  THnSparse  *fCorrelation;         //response matrix for unfolding  
  ClassDef(IlcCFTaskForUnfolding,0);
};

#endif
