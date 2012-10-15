/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcSelector.h 50615 2011-07-16 23:19:19Z hristov $ */

#ifndef ILCSELECTOR_H
#define ILCSELECTOR_H

// This selector is only dependent on the ESD library, if you need the whole of IlcROOT use IlcSelectorRL

#include <TSelector.h>

class TTree;
class IlcESD;

class IlcSelector : public TSelector {
  public:
    IlcSelector();
    virtual ~IlcSelector();

    virtual Int_t   Version() const {return 1;}
    virtual void    Begin(TTree*);
    virtual void    SlaveBegin(TTree* tree);
    virtual void    Init(TTree *tree);
    virtual Bool_t  Notify();
    virtual Bool_t  Process(Long64_t entry);
    virtual void    SlaveTerminate();
    virtual void    Terminate();

 protected:
    void CheckOptions();

    TTree          *fTree;     //! pointer to the TTree containing the events
    IlcESD*          fESD;     //! "ESD" branch in fChain
    Int_t fCountFiles;         // number of processed file

 private:
    IlcSelector(const IlcSelector&);
    IlcSelector& operator=(const IlcSelector&);

  ClassDef(IlcSelector,0);
};

#endif
