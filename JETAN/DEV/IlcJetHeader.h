#ifndef ILCJETHEADER_H
#define ILCJETHEADER_H
 
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id$ */ 
 
//---------------------------------------------------------------------
// Jet header base class 
// Stores a comment which describes the jet analysis
// Author: jgcn@mda.cinvestav.mx
//---------------------------------------------------------------------
 
#include <TNamed.h>
#include <TMath.h>

class TString;

class IlcJetHeader : public TNamed
{
 public:
 
  IlcJetHeader(const char* name);
  IlcJetHeader();
  virtual ~IlcJetHeader() { }

  // Getters
  virtual TString GetComment() const   {return fComment;  } 
  virtual Float_t GetJetEtaMax() const {return fJetEtaMax;}
  virtual Float_t GetJetEtaMin() const {return fJetEtaMin;}
  virtual Float_t GetJetPhiMax() const {return fJetPhiMax;}
  virtual Float_t GetJetPhiMin() const {return fJetPhiMin;}
  virtual Int_t   GetDebug() const     {return fDebug;    }
  virtual Bool_t  GetBGMode() const    {return fBGMode;   }
  virtual Float_t GetRadius() const    {return fRadius;   }

  // Setters
  virtual void    SetComment(const char* com)     {fComment=TString(com);}
  virtual void    SetJetEtaMax(Float_t eta= 0.5)  {fJetEtaMax = eta;}
  virtual void    SetJetEtaMin(Float_t eta= -0.5) {fJetEtaMin = eta;}
  virtual void    SetJetPhiMax(Float_t phi= 2*TMath::Pi()) {fJetPhiMax = phi;}
  virtual void    SetJetPhiMin(Float_t phi= 0.)   {fJetPhiMin = phi;}
  virtual void    SetDebug(Int_t debug)           {fDebug = debug;}
  virtual void    SetBGMode(Bool_t bgMode)        {fBGMode = bgMode;}
  virtual void    SetRadius(Float_t rad)          {fRadius = rad;}

  // others
  
protected:
  TString fComment;        // a comment 
  Float_t fJetEtaMax;      // maximum eta for the jet
  Float_t fJetEtaMin;      // minimum eta for the jet
  Float_t fJetPhiMax;      // maximum phi for the jet
  Float_t fJetPhiMin;      // minimum phi for the jet
  Int_t   fDebug;          // debug option
  Bool_t  fBGMode;         // background mode
  Float_t fRadius;         // jet radius param

  ClassDef(IlcJetHeader,4) // jet header base class
};
 
#endif
