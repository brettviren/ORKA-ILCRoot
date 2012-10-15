#ifndef ILCDCH_H
#define ILCDCH_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDCH.h,v 1.3 2008/06/07 07:51:35 cgatto Exp $ */

////////////////////////////////////////////////
//  Manager and hits classes for set: DCH     //
////////////////////////////////////////////////

#include <TLorentzVector.h>
#include "IlcDetector.h"
#include <TVirtualMC.h>

//#include "IlcDCHTriggerL1.h"
#include "IlcDCHhit.h"
#include "IlcDCHParam.h"
#include "IlcDCHwireposition.h"
class TFile;
class TLorentzVector;

class IlcRun;
class IlcDigit;

class IlcDCHhit;
class IlcDCHsim;
class IlcDCHgeometry;
//class IlcDCHParam;

//_____________________________________________________________________________
class IlcDCH : public IlcDetector {

 public:

  IlcDCH();
  IlcDCH(const char *name, const char *title);
  virtual           ~IlcDCH();

  virtual void       AddHit(Int_t, Int_t*, Float_t*) { }; 
//   virtual void       AddHit(Int_t track, Int_t det, Float_t *hits, Int_t q, Bool_t inDrift, Float_t dW=0,Int_t status=0,Float_t *Pos_local=0x0);
  virtual IlcDCHhit * AddHit(Int_t track, UShort_t superlayer, UShort_t ring, UShort_t wire, ULong_t celid, UShort_t trkStat, Float_t *hits, UShort_t ClMult);  
  virtual void       BuildGeometry();
  TList *   BuildGeometry2(void) ;             // creates the geometry for the ROOT display
  virtual void       Copy(TObject &dch) const;
  virtual void       CreateGeometry();
  virtual void       CreateMaterials();
  virtual void       DrawModule() const;
  virtual Int_t      DistancetoPrimitive(Int_t px, Int_t py);
//  virtual void       LoadPoints(Int_t track);    
  virtual void       Init();
  virtual Int_t      IsVersion() const = 0;
  virtual void       MakeBranch(Option_t* option);
//  virtual void       ResetDigits();     

  void SetParam(IlcDCHParam *param){fDCHParam=param;}

  virtual void       StepManager() = 0; 
  virtual void       SetTreeAddress();

  virtual void       StepManagerErmilova()      = 0;
  virtual void       StepManagerGeant()         = 0;
  virtual void       StepManagerFixedStep()     = 0;
  virtual void       SelectStepManager(Int_t t) = 0;
  virtual void       SetStepSize(Double_t s)    = 0;

  virtual void       SetHits()             {};
  virtual void       SetDrawTR(Int_t idraw = 1)     { fDrawTR      = idraw; };
  virtual void       SetDisplayType(Int_t type = 0) { fDisplayType = type;  };

  IlcDCHgeometry    *GetGeometry() const            { return fDCHGeometry; };
  IlcDCHParam       *GetParam()                     { return fDCHParam; }

//  virtual void       SetTR(Bool_t ) = 0;

//  virtual void       SetSensChamber(Int_t ichamber)              = 0;
//  virtual void       SetSensPlane(Int_t iplane)                  = 0;
//  virtual void       SetSensSector(Int_t isector)                = 0;
//  virtual void       SetSensSector(Int_t isector, Int_t nsector) = 0;

  virtual Int_t      GetSensChamber() const     = 0;
  virtual Int_t      GetSensPlane() const       = 0;
  virtual Int_t      GetSensSector() const      = 0;
  virtual Int_t      GetSensSectorRange() const = 0; 
 
//  virtual void       Hits2Digits();
//  virtual void       Hits2SDigits();
//  virtual IlcDigitizer* CreateDigitizer(IlcRunDigitizer* manager) const; 
//  virtual void       SDigits2Digits();
//  virtual void       Digits2Raw();

  virtual Bool_t     GetTR() const = 0;

//  virtual IlcDCHTriggerL1 *CreateTriggerDetector() const { return new IlcDCHTriggerL1(); };

 private:
   IlcDCH(const IlcDCH &dch);
   IlcDCH    &operator=(const IlcDCH &dch);
   Int_t                 fVersion;

 protected:

  IlcDCHgeometry      *fDCHGeometry;        //  The DCH geometry
  IlcDCHParam         *fDCHParam;           //  pointer to DCH parameters 
  IlcDCHwireposition  *fDCHwire; //!
  Float_t              fGasDensity;         //  The density of the drift gas
  Float_t              fFoilDensity;        //  The density of the entrance window foil

  Int_t                fDrawTR;             //  Switches marking the TR photons in the display
  Int_t                fDisplayType;        //  Display type (0: normal, 1: detailed) 

  ClassDef(IlcDCH,1)                        //  Cluster Counting Drift Chamber Detector base class

};

#endif
