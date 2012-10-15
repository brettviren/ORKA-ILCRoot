/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcSelectorRL.h 50615 2011-07-16 23:19:19Z hristov $ */

#ifndef ILCSELECTORRL_H
#define ILCSELECTORRL_H

// Use this selector, if you have IlcROOT installed (at the moment only ESD, STEER + deps)

#include "IlcSelector.h"

class IlcRunLoader;
class IlcHeader;
class IlcStack;

class IlcSelectorRL : public IlcSelector {
  public:
    IlcSelectorRL();
    virtual ~IlcSelectorRL();

    virtual Bool_t  Notify();
    virtual Bool_t  Process(Long64_t entry);
    virtual void    SlaveTerminate();

 protected:
    IlcRunLoader* GetRunLoader();
    IlcHeader* GetHeader();
    IlcStack* GetStack();

 private:
    void DeleteRunLoader();

    IlcRunLoader* fRunLoader;    //! pointer to the RunLoader if gilc.root was opened
    Bool_t fKinematicsLoaded;    // determines if the stack is properly loaded (IlcRunLoader::LoadKinematics() succeeded), this is needed because the GetStack returnes a invalid stack object when the function failed
    Bool_t fHeaderLoaded;        // determines if the header is properly loaded

    IlcSelectorRL(const IlcSelectorRL&);
    IlcSelectorRL& operator=(const IlcSelectorRL&);

    ClassDef(IlcSelectorRL,0);
};

#endif
