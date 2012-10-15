#ifndef ILCDIGITIZER_H
#define ILCDIGITIZER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDigitizer.h 52261 2011-10-23 15:46:57Z hristov $*/

////////////////////////////////////////////////////////////////////////
//
//  Base Class for Detector specific Merging/Digitization   
//  Detector specific digitization classes derive from this
//  Author: Jiri Chudoba (CERN)
//
////////////////////////////////////////////////////////////////////////

#include "TNamed.h"
#include "IlcDigitizationInput.h"

class IlcDigitizer: public TNamed {

 public:
// ctor with name and title
    IlcDigitizer(const Text_t* name="IlcDigitizer",
                const Text_t* title="IlcDigitizer");
// ctor to be used with name and title
    IlcDigitizer(IlcDigitizationInput *manager,
                 const Text_t* name="IlcDigitizer",
                 const Text_t* title="IlcDigitizer");
// Copy ctor needed because there is a pointer
    IlcDigitizer(const IlcDigitizer &dig);
    IlcDigitizer& operator=(const IlcDigitizer &dig)
      {dig.Copy(*this);return *this;}
      
    virtual ~IlcDigitizer();
    virtual Bool_t Init() {return kTRUE;}
    virtual void Digitize(Option_t* option) = 0;
    Bool_t GetRegionOfInterest() const {return fDigInput ? fDigInput->GetRegionOfInterest() : kFALSE;}

protected:
    Int_t GetNInputStreams() const;
    void Copy(TObject &dig) const;

    IlcDigitizationInput *fDigInput;   //! Pointer to the Digitizer input
    
    ClassDef(IlcDigitizer,3) // Base class for detector digitizers
};

#endif // ILCDIGITIZER_H

