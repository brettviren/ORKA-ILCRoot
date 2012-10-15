#ifndef ILCGENHYDJETEVENTHEADER_H
#define ILCGENHYDJETEVENTHEADER_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcGenHydjetEventHeader.h 18049 2007-04-20 06:38:54Z hristov $ */

// Event Header for Hydjet generator
// Output generator parameters are accessable
// for the user through this interface.
// Author: Rafael Diaz Valdes
// (rafael.diaz.valdes@cern.ch)

#include "IlcGenEventHeader.h"
#include "IlcCollisionGeometry.h"

class IlcGenHydjetEventHeader : public IlcGenEventHeader, public IlcCollisionGeometry
{
 public:

  IlcGenHydjetEventHeader(const char* name);
  IlcGenHydjetEventHeader();
  virtual ~IlcGenHydjetEventHeader() {}

  // Setters
  virtual void   SetNJets(Int_t njet) {fNjet=njet;}
  virtual void   SetImpactParameter(Float_t bgen) {fImpactParam=bgen;}
  virtual void   SetNbcol(Float_t nbcol){fNbcol=nbcol;}
  virtual void   SetNpart(Float_t npart){fNpart=npart;}
  virtual void   SetNpyt(Float_t npyt){fNpyt=npyt;}
  virtual void   SetNhyd(Float_t nhyd){fNhyd=nhyd;}

protected:

  Float_t fNjet;         //number of hard parton-parton scatterings with pt>ptmin in event.
  Float_t fImpactParam;  //generated value of impact parameter in units of nucleus radius RA
                         //(i.e the value in [fm] will be bgen*RA).
  Float_t fNbcol;        //mean number of nucleon-nucleon binary sub-collisions at given 'bgen'.
  Float_t fNpart;        //mean number of nucleon participants at given 'bgen'.
  Float_t fNpyt;         //multiplicity of hard PYTHIA/PYQUEN-induced particles in event
                         //(including full parton story).
  Float_t fNhyd;         //multiplicity of soft HYDRO-induced hadrons in event.

  ClassDef(IlcGenHydjetEventHeader,1) // Event header for Hydjet event
};

#endif
