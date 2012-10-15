#ifndef ILCTARGETFDIGITIZER_H
#define ILCTARGETFDIGITIZER_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/*
  $Id: IlcTARGETFDigitizer.h,v 1.1.1.1 2008/03/11 14:52:51 vitomeg Exp $
 */
//////////////////////////////////////////////////////////////////
// Class for fast reconstruction of recpoints                   //
//////////////////////////////////////////////////////////////////
class TObjArray;
class TTree;


class IlcRunDigitizer;

#include "IlcDigitizer.h" // Base class from which this one is derived
#include "IlcTARGET.h"   // TARGET class functions used in inline functions.
class IlcTARGETmodule;

class IlcTARGETFDigitizer : public IlcDigitizer{
 public:
    IlcTARGETFDigitizer();
    IlcTARGETFDigitizer(IlcDigitizationInput *manager);
    IlcTARGETFDigitizer(const IlcTARGETFDigitizer& rec);
    IlcTARGETFDigitizer& operator=(const IlcTARGETFDigitizer &source);    
    virtual ~IlcTARGETFDigitizer();
    // Standard routines.
    virtual Bool_t Init();
    // Perform SDigits to Digits, with or without merging, depending on the
    // number of files.
    virtual void Exec(Option_t* opt=0);
 private:
    // Routines used internaly
    // Returns a pointer to the TObjecArray of Modules.
    TObjArray* GetModules(){return fTARGET->GetModules();}
    // Returns a pointer to a  specific module.
    IlcTARGETmodule* GetModule(Int_t i){return fTARGET->GetModule(i);}
 private:
    IlcTARGET *fTARGET;      //! local pointer to TARGET
    Bool_t  fInit;     //! flag to indicate Initilization when well.


    ClassDef(IlcTARGETFDigitizer,1) // Task to Digitize TARGET from summable hits.
};
#endif
