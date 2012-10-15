#ifndef ILCDECAYEREVTGEN_H
#define ILCDECAYEREVTGEN_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */
///////////////////////////////////////////////////////////////////////////
//  Implementation of IlcDecayer using EvtGen package. It inherits       //
//  from IlcDecayer.                                                     //
//                                                                       //
//  Contact: Giuseppe.Bruno@ba.infn.it  &  Fiorella.Fionda@ba.infn.it    //
///////////////////////////////////////////////////////////////////////////


#include "IlcDecayer.h"

class EvtGen;
class EvtRandomEngine;
class EvtStdHep;
class TLorentzVector;
class TClonesArray;

class IlcDecayerEvtGen : public IlcDecayer
{
 public:
  IlcDecayerEvtGen();
  IlcDecayerEvtGen(const IlcDecayerEvtGen &decayer);
  virtual ~IlcDecayerEvtGen();
  virtual void  Init();
  virtual void  Decay(Int_t ipart, TLorentzVector *p);
  virtual Int_t ImportParticles(TClonesArray *particles);
  virtual void    SetForceDecay(Decay_t decay) {fDecay=decay;}
  virtual void    SetForceDecay(Int_t decay){SetForceDecay((Decay_t) decay);}
  virtual void    ForceDecay();
  virtual Float_t GetPartialBranchingRatio(Int_t ipart);
  virtual Float_t GetLifetime(Int_t kf);
  Char_t*           GetDecayTablePath() {return fDecayTablePath;} 
  virtual void    ReadDecayTable();
  Bool_t SetDecayTablePath(Char_t *path);
   
  private:
  void  Copy(TObject &decayer) const;
  IlcDecayerEvtGen &operator=(const IlcDecayerEvtGen &decayer) 
      {decayer.Copy(*this);return(*this);}

  protected:
  EvtRandomEngine *fRandomEngine;  //!pointer to EvtRandomEngine to generate random number   
  EvtGen *fGenerator;              //!pointer to EvtGen class interface 
  EvtStdHep *fEvtstdhep;           //!pointer to EvtGen common block
  Char_t *fDecayTablePath;         //!pointer to decay table path
  Char_t *fParticleTablePath;      //!pointer to particle table path
  Decay_t     fDecay;  	           // Forced decay case		

  ClassDef(IlcDecayerEvtGen,0)  //IlcDecayer implementation using EvtGen 
};

//converts from mm/c to s
const Double_t kconv=0.001/2.999792458e8;

#endif

 
