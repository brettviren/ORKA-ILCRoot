#ifndef ILCTRIGGERBCMASK_H
#define ILCTRIGGERBCMASK_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// This class represents the CTP bunch-crossing mask                         //
//                                                                           //
// The Mask contains name and 3564 bits for each bunch-crossing in an orbit  //
//                                                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <TNamed.h>
#include <TBits.h>

class IlcTriggerBCMask : public TNamed {

public:
                          IlcTriggerBCMask();
                          IlcTriggerBCMask( TString & name);
                          IlcTriggerBCMask( TString & name, TString & mask );
                          IlcTriggerBCMask( const char* name, const char* mask );
                          IlcTriggerBCMask( const IlcTriggerBCMask& mask );
               virtual   ~IlcTriggerBCMask();
  IlcTriggerBCMask&   operator=(const IlcTriggerBCMask& mask);

           const UChar_t* GetFullMask () const {return fBCMask; }
		  Bool_t  GetMask(UShort_t index) const;
		    void  Print( const Option_t* opt) const;
		UShort_t  GetNUnmaskedBCs() const;
		  Bool_t  SetMask (const char *mask);
  
  enum {kNBytesPerBCMask = 446,  // Number of bytes to store the 3564 bits of BC mask
	kNBits = 3564            // BC mask contains 3564 bits
  };

private:
                Bool_t    CreateMask(TString &mask);
		Bool_t    Bcm2Bits(TObjArray *tokens, Int_t &index, TBits &bits, Int_t &ibit, Int_t &level) const;

                UChar_t   fBCMask[kNBytesPerBCMask];         // Bunch cross mask (3564 bits)

   ClassDef( IlcTriggerBCMask, 2 )  // Define a trigger bunch-crossing mask
};

#endif
