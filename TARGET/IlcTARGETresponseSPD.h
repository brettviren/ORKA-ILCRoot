#ifndef ILCTARGETRESPONSESPD_H
#define ILCTARGETRESPONSESPD_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */


#include "IlcTARGETresponse.h"
///////////////////////////////////////////
//                                       //
// TARGET response class for SPD            //
///////////////////////////////////////////
  
class IlcTARGETresponseSPD :  public IlcTARGETresponse {
 public:
    IlcTARGETresponseSPD(); // default constructor
    virtual ~IlcTARGETresponseSPD() {;} // destructror

    virtual  void   SetCouplingParam(Double_t col, Double_t row)
        {fCouplCol=col; fCouplRow=row;}   
    virtual  void   GetCouplingParam(Double_t &col, Double_t &row) const 
        {col=fCouplCol; row=fCouplRow;}
    virtual void SetCouplingOption(const char *opt) {   // Options: "old" or "new"
        fCouplOpt=opt;}
    virtual void CouplingOption(char *opt) const {
        strcpy(opt,fCouplOpt.Data());}
    virtual  void   SetSigmaDiffusionAsymmetry(Double_t ecc)
        {fEccDiff=ecc;}   
    virtual  void   GetSigmaDiffusionAsymmetry(Double_t &ecc) const 
        {ecc=fEccDiff;}
    virtual void   SetParamOptions(const char* /* a */,const char* /* b */)
      {NotImplemented("SetParamOptions");}
    virtual void   ParamOptions(char *,char*) const
      {NotImplemented("ParamOptions");} 

 protected:

    static const Float_t fgkDiffCoeffDefault;  //default for fDiffCoeff
    static const TString fgkCouplingOptDefault;  // type of pixel Coupling (old or new)
    static const Float_t fgkCouplingColDefault;//default for fCouplCol 
    static const Float_t fgkCouplingRowDefault;//default for fCouplRow 
    static const Float_t fgkEccentricityDiffDefault;//default for fCouplRow 

    Float_t fCouplCol;        // Coupling probability along a column
    Float_t fCouplRow;        // Coupling probability along a row
    TString fCouplOpt;        // Coupling Option
    Float_t fEccDiff;         // Eccentricity (i.e. asymmetry parameter) in the Gaussian Diffusion


    ClassDef(IlcTARGETresponseSPD,5) // SPD base response class
};

#endif
