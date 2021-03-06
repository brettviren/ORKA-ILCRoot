#ifndef ILCAODTZERO_H
#define ILCAODTZERO_H

//-------------------------------------------------------------------------
//     Container class for AOD TZERO data
//     Author: Filip Krizek
//     filip.krizek@cern.ch 23/02/2012
//-------------------------------------------------------------------------

#include <TObject.h>

class IlcAODTZERO : public TObject 
{
public:
  IlcAODTZERO();
  IlcAODTZERO(const IlcAODTZERO& source);
  IlcAODTZERO &operator=(const IlcAODTZERO& source);

  virtual ~IlcAODTZERO() {};

  // Getters 
  //1st
  Double32_t GetT0TOF(Int_t i)  const {return fT0TOF[i];}
  const Double32_t * GetT0TOF() const {return fT0TOF;}
  //best
  Double32_t GetT0TOFbest(Int_t i)  const {return fT0TOFbest[i];}
  const Double32_t * GetT0TOFbest() const {return fT0TOFbest;}
 
  Bool_t GetBackgroundFlag() const {return fBackground;}
  Bool_t GetPileupFlag()     const {return fPileup;}
  Bool_t GetSatellite()      const {return fSattelite;}
  
  Float_t GetT0VertexRaw()      const {return fT0VertexRaw;}
  
  //Setters
  void SetT0TOF(Int_t icase, Double32_t time) { fT0TOF[icase] = time;}
  void SetT0TOFbest(Int_t icase, Double32_t time) { fT0TOFbest[icase] = time;}
   
  void SetBackgroundFlag(Bool_t back = false) {fBackground = back;}
  void SetPileupFlag(Bool_t back = false) {fPileup  = back;}
  void SetSatelliteFlag(Bool_t sat = false) { fSattelite = sat;}
  
  void SetT0VertexRaw(Float_t vtx) { fT0VertexRaw = vtx;}
       
  
protected:
  Double32_t   fT0TOF[3];    // interaction time in ps with 1st time( A&C, A, C)
  Bool_t       fPileup;      // pile-up flag
  Bool_t       fSattelite;   // sattelite flag
  Bool_t       fBackground;  // sattelite flag
  Double32_t   fT0TOFbest[3];// interaction time in ps ( A&C, A, C) with best time
  Float_t      fT0VertexRaw; // raw T0 vertex without any cuts 

  ClassDef(IlcAODTZERO,2)
};

#endif
