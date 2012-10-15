#ifndef ILCDETECTOR_H
#define ILCDETECTOR_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDetector.h 50615 2011-07-16 23:19:19Z hristov $ */

// 
// This is the basic detector class from which
// all ILC detector derive.
// This class is the base for the implementation of all detectors
// in ILC
//

#include "IlcModule.h"

class IlcHit;
class TTree;
class TBranch;
class IlcLoader;

class IlcDetector : public IlcModule {

public:

  // Creators - distructors
  IlcDetector(const char* name, const char *title);
  IlcDetector();
  virtual ~IlcDetector();

  // Inline functions
  virtual int   GetNdigits() const {return fNdigits;}
  virtual int   GetNhits()   const {return fNhits;}
  TClonesArray *Digits() const {return fDigits;}
  TClonesArray *Hits()   const {return fHits;}
  virtual  Bool_t        IsModule() const {return kFALSE;}
  virtual  Bool_t        IsDetector() const {return kTRUE;}

  Int_t         GetIshunt() const {return fIshunt;}
  void          SetIshunt(Int_t ishunt) {fIshunt=ishunt;}
  
  // Other methods
  virtual void        Publish(const char *dir, void *c, const char *name=0) const;
  virtual void        Browse(TBrowser *b);
  virtual void        FinishRun();
  virtual void        MakeBranch(Option_t *opt=" ");
  virtual void        ResetDigits();
  virtual void        ResetHits();
  virtual void        AddAlignableVolumes() const;

  virtual void        SetTreeAddress();
  virtual void        SetTimeGate(Float_t gate) {fTimeGate=gate;}
  virtual Float_t     GetTimeGate() const {return fTimeGate;}
  virtual void        StepManager() {}
  virtual void        DrawModule() const {}
  virtual IlcHit*     FirstHit(Int_t track);
  virtual IlcHit*     NextHit();
  virtual void        SetBufferSize(Int_t bufsize=8000) {fBufferSize = bufsize;}  
  virtual TBranch*    MakeBranchInTree(TTree *tree, const char* cname, void* address, Int_t size=32000, const char *file=0);
  virtual TBranch*    MakeBranchInTree(TTree *tree, const char* cname, const char* name, void* address, Int_t size=32000, Int_t splitlevel=99, const char *file=0);
  
  void MakeTree(Option_t *option); //skowron
  virtual void        RemapTrackHitIDs(Int_t *) {}
  
  virtual IlcLoader* MakeLoader(const char* topfoldername); //builds standard getter (IlcLoader type)
  void    SetLoader(IlcLoader* loader){fLoader = loader;}
  IlcLoader* GetLoader() const {return fLoader;} //skowron
    // Data members
protected:      
  
  Float_t       fTimeGate;    //Time gate in seconds

  Int_t         fIshunt;      //1 if the hit is attached to the primary
  Int_t         fNhits;       //!Number of hits
  Int_t         fNdigits;     //!Number of digits
  Int_t         fBufferSize;  //!buffer size for Tree detector branches
  Int_t         fMaxIterHit;  //!Limit for the hit iterator
  Int_t         fCurIterHit;  //!Counter for the hit iterator
  TClonesArray *fHits;        //!List of hits for one track only
  TClonesArray *fDigits;      //!List of digits for this detector

  IlcLoader*  fLoader;//! pointer to getter for this module skowron

 private:
  IlcDetector(const IlcDetector &det);
  IlcDetector &operator=(const IlcDetector &det);

  ClassDef(IlcDetector,5)  //Base class for ILC detectors
};
#endif
