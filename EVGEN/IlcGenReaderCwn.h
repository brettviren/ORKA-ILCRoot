#ifndef ILCGENREADERCWN_H
#define ILCGENREADERCWN_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcGenReaderCwn.h 14845 2006-08-15 12:25:37Z morsch $ */

//
// Realisation of IlcGenReader to be used with IlcGenExtFile
// It reads events from a ntuple like event structure.
// Author: andreas.morsch@cern.ch
//
#include "IlcGenReader.h"
#include <Rtypes.h>


class IlcGenReaderCwn : public IlcGenReader
{
 public:
    IlcGenReaderCwn();
    IlcGenReaderCwn(const IlcGenReaderCwn &reader);
    virtual ~IlcGenReaderCwn();
        // Initialise 
    virtual void Init();
    // Read
    virtual Int_t NextEvent();
    virtual TParticle*  NextParticle();
    virtual void RewindEvent(){;}
    IlcGenReaderCwn & operator=(const IlcGenReaderCwn & rhs);
    
 protected:
    Int_t             fNcurrent;      // points to the next entry
    Int_t             fNparticle;     // particle number in event
    Int_t             fNparticleMax;  // number of particles in event    
    TTree            *fTreeNtuple;    // pointer to the TTree
    //Declaration of leaves types
    Int_t           fNihead;          // Number of entries in integer header  
    Int_t           fIhead[12];       // Integer header
    Int_t           fNrhead;          // Number of entries in float header
    Float_t         fRhead[6];        // Float header
    UInt_t          fIdpart;          // Particle type
    Float_t         fTheta;           // Theta 
    Float_t         fPhi;             // Phi
    Float_t         fP;               // Total momentum
    Float_t         fE;               // Total energy
 private:
    void Copy(TObject&) const;
    ClassDef(IlcGenReaderCwn,1) // Read particles from cwn-ntuple
};
#endif






