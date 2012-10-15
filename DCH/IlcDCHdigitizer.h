#ifndef ILCDCHDIGITIZER_H
#define ILCDCHDIGITIZER_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDCHdigitizer.h,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */

#include "IlcDigitizer.h"

class TFile;
class TF1;

class IlcRunDigitizer;
class IlcRunLoader;

class IlcDCH;
class IlcDCHdigitsManager;
class IlcDCHgeometry;

///////////////////////////////////////////////////////
//  Produces digits from the hits information        //
///////////////////////////////////////////////////////

class IlcDCHdigitizer : public IlcDigitizer {

 public:

  IlcDCHdigitizer();
  IlcDCHdigitizer(const Text_t* name, const Text_t* title);
  IlcDCHdigitizer(IlcRunDigitizer *manager, const Text_t* name, const Text_t* title);
  IlcDCHdigitizer(IlcRunDigitizer *manager);
  IlcDCHdigitizer(const IlcDCHdigitizer &d);
  virtual ~IlcDCHdigitizer();
  IlcDCHdigitizer &operator=(const IlcDCHdigitizer &d);

  virtual void         Copy(TObject &d) const;
  virtual Bool_t       InitDetector();
  virtual void         Exec(Option_t* option = 0);  
  virtual Bool_t       Open(const Char_t *file, Int_t nEvent = 0);
  virtual Bool_t       MakeBranch(TTree* tree) const;
  virtual Bool_t       MakeDigits();
  virtual void         AddSDigitsManager(IlcDCHdigitsManager *manager);
  virtual void         DeleteSDigitsManager();
  virtual Bool_t       ConvertSDigits();
  virtual Bool_t       MergeSDigits();
  virtual Bool_t       SDigits2Digits();
  virtual Bool_t       WriteDigits() const;

          void         InitOutput(Int_t iEvent);
 
  virtual void         SetCompress(Int_t c = 1)             { fCompress        = c;   };
  virtual void         SetDebug(Int_t v = 1)                { fDebug           = v;   };
  virtual void         SetSDigits(Int_t v = 1)              { fSDigits         = v;   };
  virtual void         SetSDigitsScale(Float_t s)           { fSDigitsScale    = s;   };
  virtual void         SetEvent(Int_t v = 0)                { fEvent           = v;   };
  virtual void         SetManager(IlcDCHdigitsManager *man) { fDigitsManager   = man; };
  virtual void         SetGeometry(IlcDCHgeometry *geo)     { fGeo             = geo; };
  virtual void         SetMergeSignalOnly(Bool_t m = kTRUE) { fMergeSignalOnly = m;   };

  IlcDCHdigitsManager *Digits()                       const { return fDigitsManager; };

  Bool_t               GetCompress()                  const { return fCompress;      };
  Bool_t               GetSDigits()                   const { return fSDigits;       };
  Float_t              GetSDigitsScale()              const { return fSDigitsScale;  };

  virtual Double_t     TimeStruct(Float_t vdrift, Double_t time, Double_t z);

          Float_t      GetDiffusionT(Float_t vdrift);
          Float_t      GetDiffusionL(Float_t vdrift);
  virtual Int_t        Diffusion(Float_t vdrift, Double_t driftlength, Double_t *xyz);

          Float_t      GetLorentzFactor(Float_t vdrift);
  virtual Int_t        ExB(Float_t vdrift, Double_t driftlength, Double_t *xyz);  
  
 protected:

  IlcRunLoader        *fRunLoader;          //! Local pointer
  IlcDCHdigitsManager *fDigitsManager;      //! Manager for the output digits
  IlcDCHdigitsManager *fSDigitsManager;     //! Manager for the summed input s-digits
  TList               *fSDigitsManagerList; //! List of managers of input s-digits
  IlcDCH              *fDCH;                //! DCH detector class
  IlcDCHgeometry      *fGeo;                //! DCH geometry
  Int_t                fEvent;              //! Event number
  Int_t               *fMasks;              //! Masks for the merging
  Bool_t               fCompress;           //  Switch to keep only compressed data in memory
  Int_t                fDebug;              //  Sets the debug level
  Bool_t               fSDigits;            //  Switch for the summable digits
  Float_t              fSDigitsScale;       //  Scale factor for the summable digits 
  Bool_t               fMergeSignalOnly;    //  Merge only detectors that contain a signal
  Bool_t               fFixedGeometry;      //  Switch for the fixed geometry, i.e. without alignment

          void         SampleTimeStruct(Float_t vdrift);
          void         RecalcDiffusion(Float_t vdrift);
  
  struct
  {
    Float_t fLastVdrift;                                      //  the structures are vilcd for fLastVdrift (caching)
    Float_t fDiffusionT;
    Float_t fDiffusionL;
    Float_t              fOmegaTau;                           //  Tangens of the Lorentz angle
    Float_t              fLorentzFactor;                      //  Factor due to Lorentz force
  } fDiffusionInfo;
    
  struct 
  {
    Float_t fLastVdrift;                                      //  the structures are vilcd for fLastVdrift (caching)
    Float_t             *fTimeStruct1;                        //! Time Structure of Drift Cells
    Float_t             *fTimeStruct2;                        //! Time Structure of Drift Cells
    Float_t              fVDlo;                               //  Lower drift velocity, for interpolation
    Float_t              fVDhi;                               //  Higher drift velocity, for interpolation
  } fTimeStructInfo;
  
 private:
    
  virtual Bool_t       Init();
    
  virtual Bool_t       CheckDetector(Int_t plane, Int_t chamber, Int_t sector);

  ClassDef(IlcDCHdigitizer, 10)               //  Produces DCH-Digits

};

#endif
