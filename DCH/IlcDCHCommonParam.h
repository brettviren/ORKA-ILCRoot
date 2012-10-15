#ifndef IlcDCHCOMMONPARAM_H
#define IlcDCHCOMMONPARAM_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// Class containing constant common parameters                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

/* $Id: IlcDCHCommonParam.h,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */

#include "TObject.h"

class IlcDCHpadPlane;

class IlcDCHCommonParam : public TObject
{
  public:
    static IlcDCHCommonParam* Instance();
    static void Terminate();
  
    enum { kNplan = 4, kNcham = 3, kNsect =12 , kNdet = 144, kNdets = 5}; 
    
    IlcDCHCommonParam(const IlcDCHCommonParam &p);   
    IlcDCHCommonParam &operator=(const IlcDCHCommonParam &p); 
    virtual void Copy(TObject &p) const;
    
    void SetField(Float_t field)                        { fField          = field;    };
    
    void     SetExB(Int_t exbOn = 1)                        { fExBOn          = exbOn;    };
    
    Float_t  GetField()                               const { return fField; };
    
    Bool_t   ExBOn()                                  const { return fExBOn;         };
    
    IlcDCHpadPlane *GetPadPlane(Int_t p, Int_t c) const;
    Int_t    GetRowMax(Int_t p, Int_t c, Int_t /*s*/) const;
    Int_t    GetColMax(Int_t p) const;
    Double_t GetRow0(Int_t p, Int_t c, Int_t /*s*/) const;
    Double_t GetCol0(Int_t p) const;
  
  protected:
    static IlcDCHCommonParam* fgInstance;     // Instance of this class (singleton implementation)
    static Bool_t fgTerminated;               // Defines if this class has already been terminated and therefore does not return instances in GetInstance anymore
    
    void Init();
    
    Float_t              fField;                              //  Magnetic field
    
    Int_t                fExBOn;                              //  Switch for the ExB effects
  
    TObjArray  *fPadPlaneArray;                               //!  Array of pad plane objects
  
  private:
    // this is a singleton, constructor is private!  
    IlcDCHCommonParam();
    virtual ~IlcDCHCommonParam();
  
    ClassDef(IlcDCHCommonParam, 1)
};

#endif
