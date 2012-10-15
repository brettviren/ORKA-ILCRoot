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
// Container for TRD thresholds stored in the OADB
//
#ifndef ILCTRDPIDPARAMS_H
#define ILCTRDPIDPARAMS_H

#ifndef ROOT_TNamed
#include <TNamed.h>
#endif

class TSortedList;

class IlcTRDPIDParams : public TNamed{
  public:
    IlcTRDPIDParams();
    IlcTRDPIDParams(const char *name);
    IlcTRDPIDParams(const IlcTRDPIDParams &);
    virtual ~IlcTRDPIDParams();
    virtual void Print(Option_t *) const;

    Bool_t GetThresholdParameters(Int_t ntracklets, Double_t efficiency, Double_t *params) const;
    void SetThresholdParameters(Int_t ntracklets, Double_t effMin, Double_t effMax, Double_t *params);

  private:
    class IlcTRDPIDThresholds : public TObject{
    public:
	IlcTRDPIDThresholds();
	IlcTRDPIDThresholds(Int_t nTracklets, Double_t effMin, Double_t effMax, Double_t *params = NULL);
	IlcTRDPIDThresholds(Int_t nTracklets, Double_t eff, Double_t *params = NULL);
	IlcTRDPIDThresholds(const IlcTRDPIDThresholds &);
	IlcTRDPIDThresholds &operator=(const IlcTRDPIDThresholds &);
	virtual ~IlcTRDPIDThresholds() {}

        Int_t GetNTracklets() const { return fNTracklets; }
	Double_t GetElectronEfficiency(Int_t step = 0) const { if(step == 0) return fEfficiency[0]; else return fEfficiency[1]; }
	const Double_t *GetThresholdParams() const { return fParams; }

	virtual Bool_t IsSortable() const { return kTRUE; }
	virtual Int_t Compare(const TObject *ref) const;
        virtual Bool_t IsEqual(const TObject *ref) const;

    private:
	Int_t fNTracklets;          //
	Double_t fEfficiency[2];    //
	Double_t fParams[4];        //

	ClassDef(IlcTRDPIDThresholds, 1);
    };

    IlcTRDPIDParams &operator=(const IlcTRDPIDParams &);
    static const Double_t kVerySmall;
    TSortedList *fEntries; //

    ClassDef(IlcTRDPIDParams, 1);
};
#endif

