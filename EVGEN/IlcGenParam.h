#ifndef ILCGENPARAM_H
#define ILCGENPARAM_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcGenParam.h 50149 2011-06-16 13:44:44Z morsch $ */

//
// Class to generate particles from using parametrized pT and y distributions.
// Distributions are obtained from pointer to object of type IlcGenLib.
// (For example IlcGenMUONlib)
//
// andreas.morsch@cern.ch
//

#include "IlcGenMC.h"

class IlcPythia;
class TParticle;
class IlcGenLib;
class TF1;

typedef enum { kAnalog, kNonAnalog} Weighting_t;
//-------------------------------------------------------------
class IlcGenParam : public IlcGenMC
{
 public:
    IlcGenParam();
    IlcGenParam(Int_t npart, const IlcGenLib * Library, Int_t param,   const char*  tname = 0);
    IlcGenParam(Int_t npart, Int_t param, const char* tname = 0, const char*  name  = 0);
    IlcGenParam(Int_t npart, Int_t param,
		Double_t (*PtPara)(const Double_t*, const Double_t*),
		Double_t (*YPara )(const Double_t*, const Double_t*),
		Int_t    (*IpPara)(TRandom*)           );
     
    virtual ~IlcGenParam();
    virtual void Generate();
    virtual void Init();
    // select particle type
    virtual void SetParam(Int_t param) {fParam = param;}
    //Setting the flag for Background transportation while using SetForceDecay()
    void SetSelectAll(Bool_t selectall) {fSelectAll = selectall;}
    // force decay type
    virtual void SetWeighting(Weighting_t flag = kAnalog) {fAnalog = flag;}	
    virtual void SetDeltaPt(Float_t delta=0.01) {fDeltaPt = delta;}
    virtual void SetDecayer(IlcDecayer* decayer) {fDecayer = decayer;}
    virtual void Draw(const char * opt);
    TF1 *  GetPt() { return fPtPara;}
    TF1 *  GetY() {return fYPara;}
    Float_t GetRelativeArea(Float_t ptMin, Float_t ptMax, Float_t yMin, Float_t yMax, Float_t phiMin, Float_t phiMax);

 protected:
    Double_t (*fPtParaFunc)(const Double_t*, const Double_t*); //! Pointer to Pt parametrisation function
    Double_t (*fYParaFunc )(const Double_t*, const Double_t*); //! Pointer to Y parametrisation function
    Int_t    (*fIpParaFunc )(TRandom*);    //! Pointer to particle type parametrisation function
    TF1* fPtPara;              // Transverse momentum parameterisation
    TF1* fYPara;               // Rapidity parameterisation
    Int_t       fParam;        // Parameterisation type 
    Float_t     fdNdy0;        // central multiplicity per event
    Float_t     fYWgt;         // Y-weight
    Float_t     fPtWgt;        // Pt-weight
    Float_t     fBias;         // Biasing factor
    Int_t       fTrials;       // Number of trials
    Float_t     fDeltaPt;      // pT sampling in steps of fDeltaPt
    Bool_t      fSelectAll;    // Flag for transportation of Background while using SetForceDecay()
    IlcDecayer  *fDecayer;     // ! Pointer to pythia object for decays

 private:
    IlcGenParam(const IlcGenParam &Param);
    IlcGenParam & operator=(const IlcGenParam & rhs);

    ClassDef(IlcGenParam, 2) // Generator using parameterised pt- and y-distribution
};
#endif










