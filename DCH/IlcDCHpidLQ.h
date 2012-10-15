#ifndef ILCMUDPIDLQ_H
#define ILCMUDPIDLQ_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDCHpidLQ.h,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */


///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//   The MUD particle identification class                                   //
//                                                                           //
//   Its main purposes are:                                                  //
//      - Creation and bookkeeping of the propability distributions          //
//      - Assignment of a e/pi propability to a given track based on         //
//        the LQ method                                                      //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "IlcDCHpid.h"

class IlcDCHpidLQ : public IlcDCHpid {

 public:

  IlcDCHpidLQ();
  IlcDCHpidLQ(const char* name, const char* title);
  IlcDCHpidLQ(const IlcDCHpidLQ &p);
  virtual ~IlcDCHpidLQ();
  IlcDCHpidLQ &operator=(const IlcDCHpidLQ &p);

  virtual void          Copy(TObject &p) const;
  virtual Bool_t        Init();
  virtual Bool_t        AssignLikelihood()            { return 0; };
  virtual Bool_t        AssignLikelihood(TObjArray *) { return 0; };
  virtual Bool_t        AssignLikelihood(IlcDCHtrack *t);
  virtual Bool_t        CreateHistograms(Int_t nmom, Float_t minmom, Float_t maxmom);
  virtual Bool_t        FillSpectra()                 { return 0; };
  virtual Bool_t        FillSpectra(TObjArray*)       { return 0; };
  virtual Bool_t        FillSpectra(const IlcDCHtrack *t);

          Int_t         GetIndex(const IlcDCHtrack *t);
          Int_t         GetIndex(Int_t imom, Int_t ipid);
          Int_t         GetIndex(Float_t mom, Int_t ipid);

          TObjArray*    GetHist() const                     { return fHist;        };

          Float_t       GetChargeMin() const                { return fChargeMin;   };
          Int_t         GetNClusterMin() const              { return fNClusterMin; };

          Int_t         GetNLh() const                      { return fNLh;         };
          Float_t       GetMinLh() const                    { return fMinLh;       };
          Float_t       GetMaxLh() const                    { return fMaxLh;       };

          void          SetChargeMin(Float_t min)     { fChargeMin   = min;  };
          void          SetNClusterMin(Int_t min)     { fNClusterMin = min;  };

          void          SetNLh(Int_t n)               { fNLh         = n;    };
          void          SetMinLh(Float_t min)         { fMinLh       = min;  };
          void          SetMaxLh(Float_t max)         { fMaxLh       = max;  };

 protected:

  Int_t           fNMom;             //  Number of momentum bins
  Float_t         fMinMom;           //  Lower momentum
  Float_t         fMaxMom;           //  Upper momentum
  Float_t         fWidMom;           //  Width of the momentum bins
  TObjArray      *fHist;             //  Array of histograms

  Int_t           fNLh;              //  Number of bins of the likelihood spectra
  Float_t         fMinLh;            //  Lower range of the likelihood spectra
  Float_t         fMaxLh;            //  Upper range of the likelihood spectra

  Float_t         fChargeMin;        //  Minimum charge required in one plane
  Int_t           fNClusterMin;      //  Minimum number of clusters required in one plane

  ClassDef(IlcDCHpidLQ,1)            //  Assigns e/pi propability to the tracks based on LQ method 

};
#endif
