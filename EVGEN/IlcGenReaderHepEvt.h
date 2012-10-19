#ifndef ILCGENREADERHEPEVT_H
#define ILCGENREADERHEPEVT_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */


//
// Realisation of IlcGenReader to be used with IlcGenExtFile
// It reads Hijing events from a ntuple like event structure.
// Author: andreas.morsch@cern.ch
//
#include "IlcGenReader.h"
#include "Riostream.h"


class IlcGenReaderHepEvt : public IlcGenReader
{
 public:
    IlcGenReaderHepEvt();
    
    IlcGenReaderHepEvt(const IlcGenReaderHepEvt &reader):IlcGenReader(reader)
	{reader.Copy(*this);}
    virtual ~IlcGenReaderHepEvt(){ extfile.close();}
    // Initialise 
    virtual void Init();
    // Read
    virtual Int_t NextEvent();
    virtual TParticle*  NextParticle();
    virtual void RewindEvent(){ extfile.seekg(point_Ev); fNparticle = 0; }
    IlcGenReaderHepEvt & operator=(const IlcGenReaderHepEvt & rhs);

    void SetFirstEvent(Int_t first){ fFirstEvent = first; }

 protected:
    Int_t             fNcurrent;      // points to the next entry
    long point_Ev;
    ifstream extfile;

    Int_t             fNparticle;     // number of particles
    Int_t             fFirstEvent;     // First Event to read
    
 private:
    void Copy(TObject&) const;
    
    ClassDef(IlcGenReaderHepEvt,1) // Read particles from root-ntuple
};
#endif






