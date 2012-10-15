#ifndef ILCVERTEXGENFILE_H
#define ILCVERTEXGENFILE_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcVertexGenFile.h 50615 2011-07-16 23:19:19Z hristov $ */

// Generator for vertices taken from a file
// The file name of the gilc file is passed as argument
// to the constructor.

#include "IlcVertexGenerator.h"

class TFile;
class TTree;
class IlcHeader;


class IlcVertexGenFile: public IlcVertexGenerator {
 public:
  IlcVertexGenFile();
  IlcVertexGenFile(const char* fileName, Int_t eventsPerEntry = 1);
  virtual ~IlcVertexGenFile();

  virtual TVector3 GetVertex();

 private:
  IlcVertexGenFile(const IlcVertexGenFile &vgf);
  //:     IlcVertexGenerator(vgf)    {Fatal("copy ctor","Not implemented\n");}
  IlcVertexGenFile & operator=(const IlcVertexGenFile &);
  //    {Fatal("= operator","Not implemented\n"); return *this;}
  TFile*           fFile;           //! gilc file with vertices
  TTree*           fTree;           //! tree with headers
  IlcHeader*       fHeader;         //! event header
  Int_t            fEventsPerEntry; // number of events with same vertex
  Int_t            fEvent;          //! current event number

  ClassDef(IlcVertexGenFile, 1)     // generator for vertices taken from a file
};

#endif














