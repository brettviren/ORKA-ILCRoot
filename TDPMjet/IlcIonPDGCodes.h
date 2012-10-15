#ifndef IlcIonPDGCodes_h
#define IlcIonPDGCodes_h 
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

#include <TArrayI.h>
#include <TParticlePDG.h>
#include <TParticleClassPDG.h>

class TArrayI;
class TParticle;
class TDatabasePDG;

class IlcIonPDGCodes : public TObject
{

public:
  
  IlcIonPDGCodes();
  virtual ~IlcIonPDGCodes() {;}
  IlcIonPDGCodes(const IlcIonPDGCodes &PDGCodes);
  IlcIonPDGCodes& operator=(const IlcIonPDGCodes& pdg);
  virtual void AddParticlesToPdgDataBase();
//  virtual void MapPDGGEant3Codes();
  virtual void SetPDGCode(Int_t i, Int_t val) {fPDGCode[i]=val;}
  Int_t IdFromPDG(Int_t pdg) const;
  Int_t PDGFromId(Int_t pdg) const;
  
protected:
  
  Int_t fNIon;
  Int_t fPDGCode[200]; 

  ClassDef(IlcIonPDGCodes,1) 

};
#endif
