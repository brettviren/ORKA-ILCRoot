#ifndef ILCTRIGGERINPUT_H
#define ILCTRIGGERINPUT_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcTriggerInput.h 50636 2011-07-18 11:27:31Z hristov $ */

///////////////////////////////////////////////////////////////////////////////
//
//  Class to define a Trigger Input from an specific detector                                                                                           //
//
//
//                        name         description     id mask
//    Ej:
//      IlcTriggerInput( "V0_MB_L0", "VO minimum bias", 0x01 );
//      IlcTriggerInput( "V0_SC_L0", "VO semi central", 0x02 );
//      IlcTriggerInput( "V0_C_L0",  "VO central",      0x04 );

//    The name must be globaly unique. Spaces are not allowed.
//    As convention should start with detector name then an id
//    and the trigger level (L0, L1, L2)
//
//    A maximun of 60 inputs trigger are allow.
//    So, the id mask should set only bit from the position 1 to 60.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TNamed
#include <TNamed.h>
#endif

#include "IlcDAQ.h"

class IlcTriggerInput : public TNamed {

public:
                          IlcTriggerInput(): TNamed(),
			    fMask( 0 ),
			    fValue( 0 ),
			    fSignature( -1),
			    fLevel(0),
			    fDetectorId(-1),
			    fIsActive(kFALSE)
			    {}
                          IlcTriggerInput( TString name, TString det, UChar_t level, Int_t signature = -1, Char_t number = -1);
                          IlcTriggerInput( IlcTriggerInput & inp )
			    : TNamed( inp ),
			    fMask( inp.fMask ),
			    fValue( inp.fValue ),
			    fSignature( inp.fSignature ),
			    fLevel( inp.fLevel),
			    fDetectorId( inp.fDetectorId),
			    fIsActive(kFALSE)
			    {}
               virtual   ~IlcTriggerInput() {}

  //  Setters
                  void    Set()   { if (fIsActive) fValue = fMask; }
                  void    Reset() { fValue = 0; }
         	  void    Enable() { fIsActive = kTRUE; }

  //  Getters
                Bool_t    Status() const   { return (Bool_t)fValue; }
             ULong64_t    GetValue() const { return fValue; }
             ULong64_t    GetMask() const  { return fMask; }
	        Int_t     GetSignature() const { return fSignature; }
	       TString    GetInputName() const { return GetName(); }
               TString    GetDetector() const { return GetTitle(); }
	       TString    GetModule() const;
	        Char_t    GetDetectorId() const { return fDetectorId; }
	       UChar_t    GetLevel() const { return fLevel; }
	        Bool_t    IsActive() const { return fIsActive; }

           virtual void   Print( const Option_t* opt ="" ) const;

	   static Bool_t  fgkIsTriggerDetector[IlcDAQ::kNDetectors]; // List of trigger detectors
   static  const char*    fgkCTPDetectorName[IlcDAQ::kNDetectors];
protected:
             ULong64_t    fMask;                //  Trigger ID mask (1 bit)
             ULong64_t    fValue;               //  Trigger Signal (0 = false, > 1 = true = fMask )
		Int_t     fSignature;           //  8 bit signature (internal CTP inputs can have longer signature)
		UChar_t   fLevel;               //  L0, L1 or L2
		Char_t    fDetectorId;          //  Ilc-wide detector id, see IlcDAQ class for details
	        Bool_t    fIsActive;            //  Is trigger input active (during simulation)
//                  void    fDectParameterTable;  //-> link to detector parameter table????

   ClassDef( IlcTriggerInput, 4 )  // Define a Trigger Input
};


#endif
