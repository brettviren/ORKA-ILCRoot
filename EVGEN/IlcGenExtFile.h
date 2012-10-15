#ifndef ILCGENEXTFILE_H
#define ILCGENEXTFILE_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcGenExtFile.h 48105 2011-03-05 22:04:00Z morsch $ */


// Event generator that can read events from a files.
// The reading is performed by a realisation of IlcGenReader specific to the file format.
// Author: andreas.morsch@cern.ch

#include "IlcGenMC.h"
class IlcGenReader;


class TTree;

class IlcGenExtFile : public IlcGenMC
{
 public:
    IlcGenExtFile();
    IlcGenExtFile(Int_t npart);
     virtual ~IlcGenExtFile();
    // Initialise 
    virtual void Init();
    // generate event
    virtual void Generate();
    void SetReader(IlcGenReader* reader) {fReader = reader;}
 protected:
    void CdEventFile();
    const Text_t     *fFileName;      //! File to read from
    IlcGenReader     *fReader;        //! Reader to read the file

 private:
    IlcGenExtFile(const IlcGenExtFile &ext);
    IlcGenExtFile & operator=(const IlcGenExtFile & rhs);
    
  ClassDef(IlcGenExtFile,1) //Generate particles from external file
};
#endif






