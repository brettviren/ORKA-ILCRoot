#ifndef ILCGENREADERECALHIJING_H
#define ILCGENREADERECALHIJING_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcGenReaderEcalHijing.h 14845 2006-08-15 12:25:37Z morsch $ */

//
// Realisation of IlcGenReader to be used with IlcGenExtFile
// It reads Hijing events from a ntuple like event structure.
// Author: andreas.morsch@cern.ch
//
#include "IlcGenReader.h"


class IlcGenReaderEcalHijing : public IlcGenReader
{
 public:
    IlcGenReaderEcalHijing();
    
    IlcGenReaderEcalHijing(const IlcGenReaderEcalHijing &reader);
    virtual ~IlcGenReaderEcalHijing(){;}
    // Initialise 
    virtual void Init();
    // Read
    virtual Int_t NextEvent();
    virtual TParticle*  NextParticle();
    virtual void RewindEvent(){;}
    IlcGenReaderEcalHijing & operator=(const IlcGenReaderEcalHijing & rhs);

 protected:
    Int_t             fNcurrent;      // points to the next entry
    Int_t             fNparticle;     // number of particles
    
    TTree            *fTreeNtuple;    // pointer to the TTree
    //Declaration of leaves types
    Int_t           fNjatt;           // Number of particles
    Int_t           fNahij;           // Number of particles in ilc accept. 
    Int_t           fNphij;           // ?
    Int_t           fKhij[10000];     // particle code
    Float_t         fPxhij[10000];    // px
    Float_t         fPyhij[10000];    // py
    Float_t         fPzhij[10000];    // pz
    Float_t         fEhij[10000];     // energy
 private:
    void Copy(TObject&) const;
    
    ClassDef(IlcGenReaderEcalHijing,1) // Read particles from cwn-ntuple
};
#endif






