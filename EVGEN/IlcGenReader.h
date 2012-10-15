#ifndef ILCGENREADER_H
#define ILCGENREADER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcGenReader.h 14712 2006-08-01 13:15:07Z hristov $ */

// Interface for reading events from files.
// Realisations of this interface have to be used with IlcGenExFile.
// Author: andreas.morsch@cern.ch

#include "TObject.h"

class TParticle;
class IlcRunLoader;

class IlcGenReader : public TObject
{
 public:
    IlcGenReader():fFileName(NULL),fCode(kPDG){;}
    IlcGenReader(const IlcGenReader &reader)
	:TObject(reader), fFileName(NULL), fCode(kPDG){reader.Copy(*this);}
    virtual ~IlcGenReader(){;}
    virtual void SetFileName(const Text_t *filname) {fFileName=filname;}
    virtual IlcRunLoader * GetRunLoader() const {return 0x0;}
    virtual void Init()                                                    = 0;
    virtual Int_t NextEvent()                                              = 0;
    virtual TParticle* NextParticle()                                      = 0;
    virtual void RewindEvent()                                             = 0;
    typedef enum {kPDG, kGEANT3} Code_t;
    void SetParticleCode(Code_t code) {fCode = code;}
    IlcGenReader & operator=(const IlcGenReader & rhs);

 protected:
    const Text_t *fFileName;      // Name of file to read from
    Code_t        fCode;          // Particle code type
 private:
    void Copy(TObject&) const;
    
    ClassDef(IlcGenReader,1) //Generate particles from external file
};
#endif






