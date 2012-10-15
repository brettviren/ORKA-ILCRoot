#ifndef ILCFASTMUONTRIGGEREFF_H
#define ILCFASTMUONTRIGGEREFF_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcFastMuonTriggerEff.h 53089 2011-11-22 22:19:31Z morsch $ */
// Class for the fast simulation of the muon trigger efficiency
// The trigger parametrization is computed for background levels 0., 0.5 and 1.
// In order to set a background level different from 0 it is necessary to 
// explicitly force it with:
// ForceBkgLevel(BkgLevel).
// For intermediate background levels, the trigger response is linearly 
// interpolated between these values.
// There is increased granularity in the pT region below 3 GeV. Although
// it does not seem to be necessary it is also possible to interpolate
// between pT bins using SetInt().
// Author: Pietro Cortese (Universita' del Piemonte Orientale - Alessandria 
// and INFN of Torino)

#include <IlcFastResponse.h>
#include <TString.h>

class TH3F;
class TAxis;

// Debugging flag
//#define MYTRIGDEBUG

enum CutTupe {kLow, kHigh, kAny};

class IlcFastMuonTriggerEff : public IlcFastResponse {
    
 public:
    IlcFastMuonTriggerEff();
    IlcFastMuonTriggerEff(const char* /*Name*/, const char* /*Title*/);
    IlcFastMuonTriggerEff(const IlcFastMuonTriggerEff& eff);
    virtual ~IlcFastMuonTriggerEff(){;}
    virtual void    Init();
    virtual void    Evaluate(Float_t charge, Float_t pt, Float_t theta, Float_t phi,
			     Float_t& effLow, Float_t& effHigh, Float_t& effAny);
    virtual void    Evaluate(Float_t pt, Float_t theta, Float_t phi,
			     Float_t& effLow, Float_t& effHigh, Float_t& effAny)
	{Evaluate(1., pt, theta, phi, effLow, effHigh, effAny);}
    virtual Float_t Evaluate(Float_t charge, Float_t pt, Float_t theta, Float_t phi);
    
    virtual void    SetCut(Int_t cut = kLow);
    virtual Float_t Cut() const {return fCut;}
    virtual Int_t   SetBkgLevel(Float_t Bkg=0.);
    virtual Int_t   ForceBkgLevel(Float_t Bkg=0.);
    virtual Float_t GetBkgLevel() const {return fBkg;}
    Int_t LoadTables(const Char_t *namet);  // Load trigger response tables
    void SetInt() {fInt=1;}
    void UnsetInt() {fInt=0;}
    Int_t GetInt() const {return fInt;}
    // Copy
    IlcFastMuonTriggerEff& operator=(const IlcFastMuonTriggerEff& rhs);
  protected:
    Double_t fPtMin;               // Minimun pt
    Double_t fPtMax;               // Maximum pt
    Double_t fDpt;                 // Delta_pt
    Int_t    fnptb;                // Number of bins
    Double_t fPhiMin;              // Lower limit for phi 
    Double_t fPhiMax;              // Upper limit for phi
    Double_t fDphi;                // Delta_phi
    Int_t    fnphib;               // Number of bins   
    Double_t fThetaMin;            // lower limit for theta
    Double_t fThetaMax;            // upper limit for theta
    Double_t fDtheta;              // Delta_theta
    Int_t    fnthetab;             // Number of bins 
    Int_t   fCut;                  // Cut type (low/high)
    Int_t   fZones;                // Total number of zones
    TH3F*   fhEffAPt;              // Trig. prob. for Any Pt
    TH3F*   fhEffLPt;              // Trig. prob. for Low Pt
    TH3F*   fhEffHPt;              // Trig. prob. for High Pt
    TAxis*  fhLX;                  // x-axis 
    TAxis*  fhLY;                  // y-axis
    TAxis*  fhLZ;                  // z-axis
    Float_t fBkg;                  // Background level
    TString fTableTitle;           // Title of the LUT
    TString fDescription;          // Description of the table content
    Int_t fInt;                    // Interpolation flag (1 to interpolate)
  private:
    Int_t fibx;                    // index x 
    Int_t fiby;                    // index y
    Int_t fibz;                    // index z
    ClassDef(IlcFastMuonTriggerEff,1)    // Fast Muon Trigger response
};

#endif 



