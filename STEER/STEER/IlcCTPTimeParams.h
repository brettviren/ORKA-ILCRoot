#ifndef ILCCTPTIMEPARAMS_H
#define ILCCTPTIMEPARAMS_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
* See cxx source for full Copyright notice */
/* $Id$ */

#include <TNamed.h>
#include <TObjArray.h>
#include "IlcCTPInputTimeParams.h"

class TNamed;

class IlcCTPTimeParams : public TNamed {

public:
			IlcCTPTimeParams();
              virtual   ~IlcCTPTimeParams();
	
                        IlcCTPTimeParams(const IlcCTPTimeParams &timeparams);
      IlcCTPTimeParams& operator=(const IlcCTPTimeParams& timeparams);		

             
	static IlcCTPTimeParams* LoadCTPTimeParams(TString filename);	
	static IlcCTPTimeParams* LoadCTPTimeParamsFromString(const char* timeparams);
		Bool_t ProcessCTPTimeParamsLine(const char* line);
		  void AddInput( TString& inputName, UInt_t& inputLevel, UInt_t inputDelay, TString inputEdge, UInt_t deltamin, UInt_t deltamax );
		  void AddDelayL0L1L2(Int_t delayL1L0, UInt_t delayL2L0);
	  virtual void Print(const Option_t* opt="") const;
		//Setters

		//Getters
		Int_t   GetDelayL1L0()   const { return fDelayL1L0; }
	        UInt_t  GetDelayL2L0()   const { return fDelayL2L0; }                  const TObjArray* GetInputTimeParams() const { return &fCTPInputTimeParams; }
      IlcCTPInputTimeParams* GetTimeParamsForInput(TString inputname);
      Int_t GetDeltasforClass(TString classname,Int_t& deltamin,Int_t& deltamax);

                enum {kNMaxInputs = 60}; //CTP can manage up to 60 trigger detector inputs
private:
			Int_t  fDelayL1L0;
			UInt_t fDelayL2L0;
			TObjArray fCTPInputTimeParams;

  ClassDef( IlcCTPTimeParams, 3 ) 
};

#endif
