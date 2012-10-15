#ifndef ILCTARGETDIGITIZER_H
#define ILCTARGETDIGITIZER_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/*
  $Id: IlcTARGETDigitizer.h,v 1.1.1.1 2008/03/11 14:52:51 vitomeg Exp $
 */
//////////////////////////////////////////////////////////////////
// Digitizer class for TARGET                                      //
//////////////////////////////////////////////////////////////////
class TObjArray;
class TTree;

class IlcDigitizationInput;

#include "IlcDigitizationInput.h"
#include "IlcDigitizer.h" // Base class from which this one is derived
#include "IlcTARGET.h"   // TARGET class functions used in inline functions.
class IlcTARGETmodule;

class IlcTARGETDigitizer : public IlcDigitizer{
 public:
    IlcTARGETDigitizer();
    IlcTARGETDigitizer(IlcDigitizationInput *manager);
    IlcTARGETDigitizer(const IlcTARGETDigitizer& dig);
    IlcTARGETDigitizer& operator=(const IlcTARGETDigitizer &source);

    virtual ~IlcTARGETDigitizer();
    // Standard routines.
    virtual Bool_t Init();
    // Perform SDigits to Digits, with or without merging, depending on the
    // number of files.
    virtual void Digitize(Option_t* opt=0);
    // Sets a particular module active
    virtual void SetModuleActive(Int_t i){if(fModActive) fModActive[i] = kTRUE;}
    // Sets a particular module inactive
    virtual void SetModuleInActive(Int_t i){if(fModActive) fModActive[i] = kFALSE;}
    // Sets Region of Interst Flag. if fRiof=0 then no Region of Interest
    // cut applyed
    virtual void SetByRegionOfInterestFlag(Int_t i=0){fRoif = i;};
    // Sets the SDigits file number to  be used to define the region of 
    // interest. Default is file=-1, assumed that a region of interest
    // cut will be applied. A value of 0 means no cut to be applyed. Other
    // values have yet to be defined.
    virtual void SetByRegionOfFileNumber(Int_t i=-1){fRoiifile = i;};
    // Clears the region of interest flag. Calling this implies that a
    // Region of interest cut will not be made.
    virtual void ClearByRegionOfInterestFlag(){fRoif = 0;};
 private:
    // Routines used internaly
    // Returns a pointer to the TObjecArray of Modules.
    TObjArray* GetModules(){return fTARGET->GetModules();}
    // Returns a pointer to a  specific module.
    IlcTARGETmodule* GetModule(Int_t i){return fTARGET->GetModule(i);}
    // Returns a pointer to the manager
    IlcDigitizationInput* GetManager(){return fDigInput;}
    // Sets the region of Interest based on which module have SDigits
    // Defined (non-noise SDigits).
    virtual void SetByRegionOfInterest(TTree *ts);
 private:
    IlcTARGET *fTARGET;      //! local pointer to TARGET
    Bool_t *fModActive;//! flag to indicate which module to digitize.
    Bool_t  fInit;     //! flag to indecate Initilization when well.
    Int_t   fRoif;     //! Region of interest flag.
    Int_t   fRoiifile; //! The file number with which to determing the region
                       // of interest from.
    Bool_t fFlagFirstEv; //! Flag to control calibration access

    ClassDef(IlcTARGETDigitizer,2) // Task to Digitize TARGET from summable hits.
};
#endif
