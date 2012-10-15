#ifndef ILCTARGETRESPONSE_H
#define ILCTARGETRESPONSE_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcTARGETresponse.h,v 1.1.1.1 2008/03/11 14:52:50 vitomeg Exp $ */

#include <TObject.h>
#include <TString.h>

class IlcTARGETsegmentation;
class TF1;
class IlcTARGETgeom;

////////////////////////////////////////////////////
//                                                //
// TARGET base response virtual base class           //
//                                                //
////////////////////////////////////////////////////
class IlcTARGETresponse : public TObject {
 public:
 
    IlcTARGETresponse();
    virtual ~IlcTARGETresponse() {;}
    
    virtual void  SetDiffCoeff(Float_t p1, Float_t p2) {
      fDiffCoeff=p1; fDiffCoeff1=p2;}
    virtual void  DiffCoeff(Float_t &diff,Float_t &diff1) const {
      diff=fDiffCoeff; diff1=fDiffCoeff1;}
    virtual void  SetParamOptions(const char*,const char*) = 0;
   
    virtual void  ParamOptions(char *,char*) const = 0;
    virtual void  SetFilenames(const char *f1="",const char *f2="",
                              const char *f3=""){
	fFileName1=f1; fFileName2=f2; fFileName3=f3;}

    virtual void   Filenames(char* input,char* baseline,char* param) {
        strcpy(input,fFileName1.Data());  strcpy(baseline,fFileName2.Data());  
        strcpy(param,fFileName3.Data());}

    virtual void    SetOutputOption(Bool_t write=kFALSE) {fWrite = write;}
	
    virtual Bool_t  OutputOption() const {return fWrite;}

 protected:

    void NotImplemented(const char *method) const {if(gDebug>0)
         Warning(method,"This method is not implemented for this sub-class");}
   
 private:

    TString  fFileName1; // input keys : run, module #
    TString  fFileName2; // baseline & noise val or output code
                                // signal or monitored bgr.
    TString  fFileName3; // param values or output coded signal
    Bool_t   fWrite;     // Write option for the compression algorithms
    Float_t  fDiffCoeff;      // Diffusion Coefficient (scaling the time)
    Float_t  fDiffCoeff1;     // Diffusion Coefficient (constant term)
    

    ClassDef(IlcTARGETresponse,4) // Detector type response virtual base class 
};

#endif
