#ifndef ILCGENEVENTHEADER_H
#define ILCGENEVENTHEADER_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcGenEventHeader.h 50615 2011-07-16 23:19:19Z hristov $ */

//---------------------------------------------------------------------
// Event header base class for generator. 
// Stores generated event information
// Author: andreas.morsch@cern.ch
//---------------------------------------------------------------------

#include <TNamed.h>
#include <TArrayF.h>

class IlcGenEventHeader : public TNamed
{
 public:

  IlcGenEventHeader(const char* name);
  IlcGenEventHeader();
  virtual ~IlcGenEventHeader() {}
  // Getters
  virtual Int_t           NProduced()       const  {return fNProduced;}
  virtual Float_t         InteractionTime() const  {return fInteractionTime;}
  virtual Float_t         EventWeight()     const  {return fEventWeight;}
  virtual void            PrimaryVertex(TArrayF &o) const;
  // Setters
  virtual void   SetNProduced(Int_t nprod)         {fNProduced = nprod;}
  virtual void   SetPrimaryVertex(const TArrayF &o);
  virtual void   SetInteractionTime(Float_t t)     {fInteractionTime = t;}
  virtual void   SetEventWeight(Float_t w)         {fEventWeight = w;}
        
	  
protected:
  Int_t     fNProduced;                 // Number stable or undecayed particles
  TArrayF   fVertex;                    // Primary Vertex Position
  Float_t   fInteractionTime;           // Time of the interaction
  Float_t   fEventWeight;               // Event weight
  ClassDef(IlcGenEventHeader, 4)        // Event header for primary event
};

#endif
