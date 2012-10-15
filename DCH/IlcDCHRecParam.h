#ifndef IlcDCHRECPARAM_H
#define IlcDCHRECPARAM_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// Class containing constant reconstruction parameters                       //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

/* $Id: IlcDCHRecParam.h,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */

#include "TObject.h"

class IlcDCHRecParam : public TObject
{
  public:
    static IlcDCHRecParam* Instance();
    static void Terminate();
  
    enum { kNplan = 6, kNcham = 5, kNsect = 18, kNdet = 540 };
    
    IlcDCHRecParam(const IlcDCHRecParam &p);   
    IlcDCHRecParam &operator=(const IlcDCHRecParam &p); 
    virtual void Copy(TObject &p) const;
  
    virtual void     SetLUT(Int_t lutOn = 1)                        { fLUTOn          = lutOn;    };
    virtual void     SetClusMaxThresh(Float_t thresh)                 { fClusMaxThresh  = thresh;   };
    virtual void     SetClusSigThresh(Float_t thresh)                 { fClusSigThresh  = thresh;   };
    
            void SetTailCancelation(Int_t tcOn = 1)                 { fTCOn           = tcOn;     };
            void SetNexponential(Int_t nexp)                        { fTCnexp         = nexp;     };
  
    Bool_t   LUTOn()                                          const { return fLUTOn;         };
    virtual Float_t    GetClusMaxThresh()                       const { return fClusMaxThresh; };
    virtual Float_t    GetClusSigThresh()                       const { return fClusSigThresh; };
    
    virtual Double_t  LUTposition(Int_t iplane, Double_t ampL, Double_t ampC, Double_t ampR) const;
  
            Bool_t   TCOn()                                   const { return fTCOn;          };
            Int_t    GetTCnexp()                              const { return fTCnexp;            };
  
  protected:
    static IlcDCHRecParam* fgInstance;     // Instance of this class (singleton implementation)
    static Bool_t fgTerminated;               // Defines if this class has already been terminated and therefore does not return instances in GetInstance anymore
    
    void Init();
  
    // Clusterization parameter
    Float_t              fClusMaxThresh;                      //  Threshold value for cluster maximum
    Float_t              fClusSigThresh;                      //  Threshold value for cluster signal
    
    Int_t                fLUTOn;                              //  Switch for the lookup table method
    Int_t                fLUTbin;                             //  Number of bins of the LUT
    Float_t             *fLUT;                                //! The lookup table
  
    Int_t                fTCOn;                               //  Switch for the tail cancelation
    Int_t                fTCnexp;                             //  Number of exponentials, digital filter
    virtual void         FillLUT();
  
  private:
    // this is a singleton, constructor is private!  
    IlcDCHRecParam();
    ~IlcDCHRecParam();
  
    ClassDef(IlcDCHRecParam, 1)
};

#endif
