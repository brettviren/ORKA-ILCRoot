#ifndef ILCTARGETDIGITZER_H
#define ILCTARGETDIGITZER_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/*
  $Id: IlcTARGEThitDigitizer.h,v 1.1.1.1 2008/03/11 14:52:50 vitomeg Exp $
 */

class TObjArray;

#include <TClonesArray.h> // function of this class used in inline functions.

class IlcRunDigitizer;

#include "IlcDigitizer.h" // Base class from which this one is derived
#include "IlcTARGET.h"   // TARGET class functions used in inline functions.
class IlcTARGEThit;
class IlcTARGETmodule;



class IlcTARGETDigitizer : public IlcDigitizer{
 public:
    IlcTARGETDigitizer();
    IlcTARGETDigitizer(IlcRunDigitizer *manager);
    virtual ~IlcTARGETDigitizer();
    // Standard routines.
    virtual Bool_t Init();
    virtual void Exec(Option_t* opt=0);
 private:
    // Routines used internaly
    TClonesArray* GetHits(){return fTARGET->Hits();}
    IlcTARGEThit* GetHit(Int_t h){return (IlcTARGEThit*)(GetHits()->UncheckedAt(h));}
    TObjArray* GetModules(){return fTARGET->GetModules();}
    IlcTARGETmodule* GetModule(Int_t i){return fTARGET->GetModule(i);}
    IlcRunDigitizer* GetManager(){return fManager;}
 private:
    IlcTARGET *fTARGET;  //! local pointer to TARGET

    ClassDef(IlcTARGETDigitizer,1) // Task to Digitize TARGET from summable hits.
};
#endif
