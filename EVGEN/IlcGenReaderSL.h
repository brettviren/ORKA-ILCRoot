#ifndef ILCGENREADERSL_H
#define ILCGENREADERSL_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

// Realisations of the IlcGenReader interface to be used with IlcGenExFile.
// NextEvent() loops over events 
// and NextParticle() loops over particles. 
// This implementation reads various StarLight output formats 
// Author: andreas.morsch@cern.ch

#include "IlcGenReader.h"

class TParticle;

class IlcGenReaderSL : public IlcGenReader
{
 public:
    IlcGenReaderSL():fFile(0), fNParticles(0), fFormat(0) {;}
    IlcGenReaderSL(const IlcGenReaderSL &reader)
	:IlcGenReader(reader), fFile(0), fNParticles(0), fFormat(0)  {reader.Copy(*this);}
    virtual ~IlcGenReaderSL(){;}
    virtual void Init();
    virtual Int_t NextEvent();
    virtual TParticle* NextParticle();
    virtual void RewindEvent();
    virtual void SetFormat(Int_t format) {fFormat = format;}
    IlcGenReaderSL & operator=(const IlcGenReaderSL & rhs);

 protected:
    FILE *fFile;          // pointer to the file
    Int_t fNParticles;    // Number of particles
    Int_t fFormat;        // File format
 private:
    void Copy(TObject&) const;
    
    ClassDef(IlcGenReaderSL, 1) //Generate particles from external file
};
#endif






