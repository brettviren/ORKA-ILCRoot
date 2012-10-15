#ifndef ILCTARGETPARAM_H
#define ILCTARGETPARAM_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcTARGETParam.h,v 1.5 2008/11/06 23:59:40 cgatto Exp $ */

/////////////////////////////////////////////////////////////////////////
//  TARGET Parameter manipulation routines.
//  Created April 15 1999.
//  version: 0.0.0
//  By: Daniele Barbareschi/Matteo Rucco
//
/////////////////////////////////////////////////////////////////////////
#include <Riostream.h>
#include <TObject.h>
#include <TObjArray.h>
#include <TVector.h>
#include <TString.h>
#include <TArrayI.h>
#include <TArrayD.h>

//_______________________________________________________________________

class IlcTARGETParam : public TObject {

 public:
    IlcTARGETParam(Int_t detVersion);                      // Default constructor

    //    IlcTARGETParam(const IlcTARGETParam &source);    // Copy constructor
    //    IlcTARGETParam& operator=(const IlcTARGETParam &source);// = operator

    virtual ~IlcTARGETParam();             // Default destructor
    void Init(Int_t detVersion);
    // Zero and reinitilizes this class.
    Int_t GetNdetectors(Int_t lay) const {return fNdet[lay];}
    //     This function returns the number of ladders for a give layer. In
    // particular it returns fNlad[layer-1].
    Int_t GetNladders(Int_t lay)   const {return fNlad[lay];};
    const Int_t GetNlayers()                   const {return fNlayers;}
    const Int_t GetNlayersStrip()              const {return fNlayersStrip;}
    const Int_t GetNendcaps()                  const {return fNendcaps;}
    const Int_t GetNendcapsStrip()             const {return fNendcapsStrip;}
    const Int_t GetNmodulesSPD()               const {return fNmodulesSPD;}
    const Int_t GetNmodulesSSD()               const {return fNmodulesSSD;}

    Int_t GetTotLayers()                             {return fTotLayers;}
   
    Double_t GetScale()                        const {return fScale;}//Scale's Factor
    //Double_t GetScaleSiD()                   const {return fScaleSiD;}//Scale's Factor for SiD version
    Double_t GetCThickness()                   const {return fCThick;}//Half CarbonFiber Thickness
    Double_t GetSiThickness()                  const {return fSiThick;}//Half Silicon Thickness
    
    Double_t GetSigmaRPhi(Int_t lay)           const {return fSigmaRPhi.At(lay);}
    Double_t GetSigmaZ(Int_t lay)              const {return fSigmaZ.At(lay);}
    Double_t GetSigmaDe(Int_t lay)             const {return fSigmaDe.At(lay);}
    Double_t GetThrDe(Int_t lay)               const {return fThrDe.At(lay);}
    Double_t GetBiasVoltage()                  const {return fbiasVoltage;}
    Float_t  GetSingleHitStripEfficiency()     const {return fSingleHitStripEfficiency;}
    Float_t  GetSingleHitPixelEfficiency()     const {return fSingleHitPixelEfficiency;}
    
    Float_t  GetStereoP(Int_t lay)             const {return fStereoP[lay];}
    Float_t  GetStereoN(Int_t lay)             const {return fStereoN[lay];}
    Int_t    GetSensor(Int_t lay)              const {return fSensor[lay];}
    
    
    void SetBiasVoltage(Double_t biasVoltage=18.182)                   {fbiasVoltage=biasVoltage;}
    void SetNModuleSPD(Int_t moduleSPD)                   {fNmodulesSPD=moduleSPD;}
    void SetTotLayers(Int_t totLayers)                   {fTotLayers=totLayers;}
    
    void SetNdetectors(Int_t lay,Int_t det)  {fNdet[lay]=det;}
    //     This function returns the number of ladders for a give layer. In
    // particular it returns fNlad[layer-1].
    void SetNladders(Int_t lay,Int_t lad)   {fNlad[lay]=lad;};
/*    void SetNlayers(Int_t lay)                    {fNlayers=lay;}
    void SetNendcaps(Int_t endcap)                    {fNendcaps=endcap;} */
    void SetSigmaRPhi(Int_t lay,Double_t sigmaRPhi)                    {fSigmaRPhi[lay]=sigmaRPhi;} 
    void SetSigmaZ(Int_t lay,Double_t sigmaZ)                    {fSigmaZ[lay]=sigmaZ;} 
    void SetSigmaDe(Int_t lay,Double_t sigmaDe)                    {fSigmaDe[lay]=sigmaDe;} 
    void SetThrDe(Int_t lay,Double_t ThrDe)                    {fThrDe[lay]=ThrDe;} 

    Int_t GetRangeY() {return fRangeCluY;}
    Int_t GetRangeZ() {return fRangeCluZ;}
    Double_t GetFda() {return fFda;}
    Double_t GetCouplCol() {return fCouplCol;}
    Double_t GetCouplRow() {return fCouplRow;}

    Double_t GetLayerThick()          { return fLayerThick ;}
    Double_t GetTargetThick()         { return fTargetThick ;}
    Double_t GetTargetLength()        { return fTargetLength ;}
    Double_t GetTargetLowerZ()        { return fTargetLowerZ ;}
    Int_t    GetNLayers()       const { return (Int_t)(fTargetThick/fLayerThick) ;}
    Int_t    GetNLadders()      const { return fNLadders ;}


 private:
//Resize Variable
Double_t fScale;
Double_t fCThick;
Double_t fSiThick;
//Double_t fScaleSiD;
Double_t fOldMagField;
Double_t fNewMagField;
//End Resize Variable

    
    Double_t   fLayerThick;
    Double_t   fTargetThick;
    Double_t   fTargetLength;
    Double_t   fTargetLowerZ;
    Int_t      fNLadders;
    Int_t      fNendcaps;// The number of pixel endcaps;
    Int_t      fNendcapsStrip;// The number of strip endcaps;
    Int_t      fNlayersStrip;// The number of strip layers;
    Int_t      fNmodulesSPD;// The total number of SPD modules
    Int_t      fNmodulesSSD;// The total number of SSD modules
    Int_t      fTotLayers;
    Int_t      fNlayers; // The number of pixel layers.
    TArrayI    fNlad;    // Array of the number of ladders/layer(layer)
    TArrayI    fNdet;    // Array of the number of detector/ladder(layer)
    TArrayD    fSigmaRPhi;
    TArrayD      fSigmaZ;
    TArrayD      fSigmaDe;
    TArrayD      fThrDe;
    Float_t     fSingleHitStripEfficiency;
    Float_t     fSingleHitPixelEfficiency;
    Double_t       fbiasVoltage;
    Int_t       fRangeCluY;
    Int_t       fRangeCluZ;
    Double_t    fFda;
    Double_t    fCouplCol;
    Double_t    fCouplRow;
    Float_t     *fStereoP;
    Float_t     *fStereoN;
    Int_t       *fSensor;

    ClassDef(IlcTARGETParam,5) // TARGET parameter class
};

#endif
