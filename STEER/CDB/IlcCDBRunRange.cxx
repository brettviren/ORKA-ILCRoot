/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/////////////////////////////////////////////////////////////////////
//                                                                 //
//  class IlcCDBRunRange					   //
//  defines the run validity range of the object:		   //
//  [fFirstRun, fLastRun] 					   //
//                                                                 //
/////////////////////////////////////////////////////////////////////

#include "IlcCDBRunRange.h"

#include "IlcLog.h"

ClassImp(IlcCDBRunRange)

//___________________________________________________________________________
IlcCDBRunRange::IlcCDBRunRange():
fFirstRun(-1), 
fLastRun(-1)
{
// constructor

}

//___________________________________________________________________________
IlcCDBRunRange::IlcCDBRunRange(Int_t firstRun, Int_t lastRun):
fFirstRun(firstRun), 
fLastRun(lastRun)
{
// constructor

}

//___________________________________________________________________________
IlcCDBRunRange::~IlcCDBRunRange() {
// destructor

}

//___________________________________________________________________________
Bool_t IlcCDBRunRange::Overlaps(const IlcCDBRunRange& other) const {
// check if this runRange overlaps other runRange
	
	if (!(IsValid() && other.IsValid())) {
		IlcError("Comparing invalid run ranges!");
		return kFALSE;
	}	
	
	if (IsAnyRange() || other.IsAnyRange()) {
		IlcError("Comparing unspecified ranges!");
		return kFALSE;
	}
	
	return ((fFirstRun < other.fFirstRun && other.fFirstRun <= fLastRun)
	    || (other.fFirstRun <= fFirstRun && fFirstRun <= other.fLastRun));
}

//___________________________________________________________________________
Bool_t IlcCDBRunRange::Comprises(const IlcCDBRunRange& other) const {
// check if this runRange contains other runRange

	if (!(IsValid() && other.IsValid())) {
		IlcError("Comparing invalid run ranges!");
		return kFALSE;
	}	
	
	if (IsAnyRange()) {
		return kTRUE;
	}

	return fFirstRun <= other.fFirstRun && other.fFirstRun <= fLastRun
		&& fFirstRun <= other.fLastRun && other.fLastRun <= fLastRun;
}

//___________________________________________________________________________
Bool_t IlcCDBRunRange::IsEqual(const TObject* obj) const {
// check if this runRange is equal to other runRange
	
        if (this == obj) {
                return kTRUE;
        }

        if (IlcCDBRunRange::Class() != obj->IsA()) {
                return kFALSE;
        }
        IlcCDBRunRange* other = (IlcCDBRunRange*) obj;
	return fFirstRun == other->fFirstRun && fLastRun == other->fLastRun;
}

//___________________________________________________________________________
Bool_t IlcCDBRunRange::IsValid() const {
// validity check

	if (fFirstRun < 0 && fLastRun < 0) {
		return kTRUE;
	}

	if (fFirstRun >= 0 && fLastRun >= fFirstRun) {
		return kTRUE;
	}

	return kFALSE;
}


