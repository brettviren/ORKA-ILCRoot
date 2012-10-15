#ifndef IlcESDACORDE_H
#define IlcESDACORDE_H

#include <TObject.h>
// Send comments to:
// Mario Rodriguez <mrodrigu@mail.cern.ch>
// Luciano Diaz <luciano.diaz@nucleares.unam.mx>
// Last update: October 2th 2009 

class IlcESDACORDE : public TObject
{

 public:
  IlcESDACORDE();
  IlcESDACORDE(const IlcESDACORDE&);
  IlcESDACORDE(Bool_t *ACORDEBitPattern); 
  virtual ~IlcESDACORDE() {};
  virtual void Copy(TObject &) const;

 // We define the "setters" for ACORDE

  void SetACORDEBitPattern(Bool_t ACORDEBitPattern[60]){for (Int_t i=0;i<60;i++){fACORDEBitPattern[i]=ACORDEBitPattern[i];}}

  
  
 // Getters  	
  Bool_t GetHitChannel(Int_t i) const;
  IlcESDACORDE &operator=(const IlcESDACORDE& source);
  
 protected:

  Bool_t	fACORDEBitPattern[60];  // Array with the ACORDE's bitpattern

  ClassDef(IlcESDACORDE, 3)

};

#endif
