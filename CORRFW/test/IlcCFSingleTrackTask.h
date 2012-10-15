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

#ifndef ILCCFSINGLETRACKTASK_H
#define ILCCFSINGLETRACKTASK_H

#include "IlcAnalysisTaskSE.h"

class TH1I;
class TParticle ;
class TFile ;
class IlcStack ;
class IlcCFManager;
class IlcESDtrack;
class IlcVParticle;

class IlcCFSingleTrackTask : public IlcAnalysisTaskSE {
  public:

  enum {
    kStepGenerated       = 0,
    kStepReconstructible = 1,
    kStepReconstructed   = 2,
    kStepSelected        = 3
  };

  IlcCFSingleTrackTask();
  IlcCFSingleTrackTask(const Char_t* name);
  IlcCFSingleTrackTask& operator= (const IlcCFSingleTrackTask& c);
  IlcCFSingleTrackTask(const IlcCFSingleTrackTask& c);
  virtual ~IlcCFSingleTrackTask();

  // ANALYSIS FRAMEWORK STUFF to loop on data and fill output objects
  void     UserCreateOutputObjects();
  void     UserExec(Option_t *option);
  void     Terminate(Option_t *);
  
  // CORRECTION FRAMEWORK RELATED FUNCTIONS
  void           SetCFManager(IlcCFManager* io) {fCFManager = io;}   // global correction manager
  IlcCFManager * GetCFManager() const {return fCFManager;}           // get corr manager
  void           SetQAList(TList* list) {fQAHistList = list;}

  // Data types
  Bool_t IsReadTPCTracks() const {return fReadTPCTracks;}
  Bool_t IsReadAODData()   const {return fReadAODData;}
  void   SetReadTPCTracks (Bool_t flag=kTRUE) {fReadTPCTracks=flag;}
  void   SetReadAODData   (Bool_t flag=kTRUE) {fReadAODData=flag;}

 protected:
  
  Bool_t          fReadTPCTracks ; // flag to loop on TPC tracks only (ESD mode only)
  Bool_t          fReadAODData ;   // flag for AOD/ESD input files
  IlcCFManager   *fCFManager    ;  // pointer to the CF manager
  TList          *fQAHistList   ;  // list of QA histograms

  // Histograms
  //Number of events
  TH1I  *fHistEventsProcessed; // simple histo for monitoring the number of events processed
  
  ClassDef(IlcCFSingleTrackTask,1);
};

#endif
