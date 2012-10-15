#ifndef ILCJETSELECTOR_H
#define ILCJETSELECTOR_H
 
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */
 
//---------------------------------------------------------------------
// Jet finder base class
// manages the search for jets 
// Author: jgcn@mda.cinvestav.mx
//---------------------------------------------------------------------

#include <TSelector.h>

class TTree;
class IlcJetFinder;

class IlcJetSelector : public TSelector 
{
 public:
  IlcJetSelector(TTree* tree = 0);
  virtual ~IlcJetSelector();
  virtual void   Config();
  virtual Int_t  Version() const {return 1;}
  virtual void   Begin(TTree* tree) ;
  virtual void   SlaveBegin(TTree* tree);
  virtual void   Init(TTree* tree);
  virtual Bool_t Notify();
  virtual Bool_t Process(Long64_t entry);
  virtual void   SetOption(const char *option) { fOption = option; }
  virtual void   SetObject(TObject *obj) { fObject = obj; }
  virtual void   SetInputList(TList *input) {fInput = input;}
  virtual TList* GetOutputList() const { return fOutput; }
  virtual void   SlaveTerminate();
  virtual void   Terminate();
  
 protected:
  IlcJetFinder* fJetFinder; // Pointer to jet finder
  
  ClassDef(IlcJetSelector, 1)
};

#endif
