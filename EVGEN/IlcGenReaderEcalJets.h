#ifndef ILCGENREADERECALJETS_H
#define ILCGENREADERECALJETS_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcGenReaderEcalJets.h 14845 2006-08-15 12:25:37Z morsch $ */
//
// Realisation of IlcGenReader to be used with IlcGenExtFile
// It reads Hijing events from a ntuple like event structure.
// Author: andreas.morsch@cern.ch
//
#include "IlcGenReader.h"


class IlcGenReaderEcalJets : public IlcGenReader
{
 public:
    IlcGenReaderEcalJets();
    IlcGenReaderEcalJets(const IlcGenReaderEcalJets &reader);
    virtual ~IlcGenReaderEcalJets(){;}
    // Initialise 
    virtual void Init();
    // Read
    virtual Int_t NextEvent();
    virtual TParticle*  NextParticle();
     virtual void RewindEvent(){;}
    IlcGenReaderEcalJets & operator=(const IlcGenReaderEcalJets & rhs);
 
 protected:
    Int_t           fNcurrent;      // points to the next event
    Int_t           fNparticle;     // points to the next particle 
    Int_t           fNev;           // event number
    Float_t         fX[2];          // vertex ?
    Int_t           fXtyp[2];       // parton type
    Int_t           fNpart;         // number of particles  
    Float_t         fXpt[200];      // pt of particle
    Float_t         fXeta[200];     // eta of particle
    Float_t         fXphi[200];     // phi of particle
    Int_t           fXid[200];      // id of particle
    Int_t           fNjet;          // number of jets 
    Float_t         fJet[10];       // E_t of jet
    Float_t         fJeta[10];      // eta of jet
    Float_t         fJphi[10];      // phi of jet
    Int_t           fNsjet;         // number of clusters
    Float_t         fJset[10];      // E_t of cluster 
    Float_t         fJseta[10];     // eta of cluster
    Float_t         fJsphi[10];     // phi of cluster
    Int_t           fNpjet;         // Jet p
    Float_t         fJpet[10];      // Jet Et
    Float_t         fJpeta[10];     // Jet eta
    Float_t         fJpphi[10];     // Jet phi
    TTree            *fTreeNtuple;  // pointer to the TTree

 private:
    void Copy(TObject&) const;
    ClassDef(IlcGenReaderEcalJets,1) // Read particles from cwn-ntuple
};
#endif






