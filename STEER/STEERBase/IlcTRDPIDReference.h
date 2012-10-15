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
//
// Container for the reference distributions for the TRD PID
// Provides storing of the references together with the mometum steps
// they were taken
// More information can be found in the implementation file
//
#ifndef ILCTRDPIDREFERENCE_H
#define ILCTRDPIDREFERENCE_H

#include <TNamed.h>
#include <TArrayF.h>
#include "IlcPID.h"

class TObjArray;

class IlcTRDPIDReference : public TNamed{
public:
	IlcTRDPIDReference();
	IlcTRDPIDReference(const Char_t *name);
	IlcTRDPIDReference(const IlcTRDPIDReference &ref);
	IlcTRDPIDReference &operator=(const IlcTRDPIDReference &ref);
	~IlcTRDPIDReference();

	void SetNumberOfMomentumBins(Int_t nBins, Float_t *momenta);
	void AddReference(TObject *histo, IlcPID::EParticleType spec, Int_t pbin);

	// Derive reference
	TObject *GetLowerReference(IlcPID::EParticleType spec, Float_t p, Float_t &pLower) const;
	TObject *GetUpperReference(IlcPID::EParticleType spec, Float_t p, Float_t &pUpper) const;

	Int_t GetNumberOfMomentumBins() const { return fMomentumBins.GetSize(); }
	void Print(const Option_t *) const;
private:
	enum{
		kIsOwner = BIT(14)
	};
	TObjArray *fRefContainer;     // Histogram container
	TArrayF fMomentumBins;        // Momentum Bins

	ClassDef(IlcTRDPIDReference, 1)		// Container for TRD references
};
#endif

