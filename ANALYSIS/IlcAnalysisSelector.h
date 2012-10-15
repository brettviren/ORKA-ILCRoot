#ifndef ILCANALYSISSELECTOR_H
#define ILCANALYSISSELECTOR_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

// Author: Andrei Gheata, 31/05/2006

//==============================================================================
//   IlcAnalysisSelector - Transparent selector class instantiated by an
// analysis manager object.
//==============================================================================

#ifndef ROOT_TSelector
#include "TSelector.h"
#endif

class IlcAnalysisManager;

class IlcAnalysisSelector : public TSelector {

protected:
   Bool_t              fInitialized; // Flag that initialization was done
   IlcAnalysisManager *fAnalysis;    // Analysis manager to be processed
   
private:
   IlcAnalysisSelector(const IlcAnalysisSelector&);            // not implemented
   IlcAnalysisSelector& operator=(const IlcAnalysisSelector&); // not implemented
   void                RestoreAnalysisManager();

public:
   IlcAnalysisSelector();
   IlcAnalysisSelector(IlcAnalysisManager *mgr);
   virtual ~IlcAnalysisSelector();
   
   virtual int         Version() const {return 2;}
   virtual void        Init(TTree *tree);
   virtual void        Begin(TTree *);
   virtual void        SlaveBegin(TTree *tree);
   virtual Bool_t      Notify();
   virtual Bool_t      Process(Long64_t entry);
   virtual void        SlaveTerminate();
   virtual void        Terminate();

   ClassDef(IlcAnalysisSelector,0)  //A class for processing jobs using IlcAnalysisManager
};

#endif
