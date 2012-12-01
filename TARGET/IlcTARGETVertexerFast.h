#ifndef ILCTARGETVERTEXERFAST_H
#define ILCTARGETVERTEXERFAST_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */


#include <IlcTARGETVertexer.h>

/////////////////////////////////////////////////////////////////////
//                                                                 //
// Fast vertexer - True (i.e. generated) vertex coordinates        //
// are smeared with gaussians of given width                       //
//                                                                 //
/////////////////////////////////////////////////////////////////////


class IlcTARGETVertexerFast : public IlcTARGETVertexer {

 public:
  IlcTARGETVertexerFast();
  IlcTARGETVertexerFast(Double_t *smear);
  virtual ~IlcTARGETVertexerFast(); 
  virtual IlcESDVertex* FindVertexForCurrentEvent(Int_t evnumb);

#ifdef WIN32
  virtual IlcESDVertex* FindVertexForCurrentEvent(TTree*);
#else
  virtual IlcESDVertex* FindVertexForCurrentEvent(TTree*){ Fatal("FindVertexForCurrentEvent","FindVertexForCurrentEvent(TTree*) not yet implemented\n");}
#endif

  virtual void FindVertices();
  virtual void PrintStatus() const;

 protected:

  // copy constructor (NO copy allowed: the constructor is protected
  // to avoid misuse)
// // // //   IlcTARGETVertexerFast(const IlcTARGETVertexerFast& vtxr);
  // assignment operator (NO assignment allowed)
  IlcTARGETVertexerFast& operator=(const IlcTARGETVertexerFast& /* vtxr */);

  Double_t *fSmear;         // rms of gaussians used for smearing


ClassDef(IlcTARGETVertexerFast,1);
};

#endif
