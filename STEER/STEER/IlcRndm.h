#ifndef ILCRNDM_H
#define ILCRNDM_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcRndm.h 50615 2011-07-16 23:19:19Z hristov $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//   Random Number Interface                                                 //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <TRandom.h>

class IlcRndm 
{
public:
  IlcRndm();
  IlcRndm(const IlcRndm &rn);
  virtual ~IlcRndm() {fRandom=0;}
  IlcRndm & operator=(const IlcRndm& rn) 
    {rn.Copy(*this); return (*this);}
  
  // Random number generator bit
  virtual void SetRandom(TRandom *ran=0)
  {if(ran) fRandom=ran;
  else fRandom=gRandom;}

  virtual TRandom* GetRandom() const {return fRandom;}
  virtual void Rndm(Float_t* array, Int_t size) const; 
#ifdef CKNONE
  virtual Float_t Rndm() const {return fRandom->Rndm();}
#else
  virtual Float_t Rndm() const {
    Float_t r;
    do r=fRandom->Rndm(); while(0>=r || r>=1); return r;}
#endif
  virtual void WriteRandom(const char *filename) const;
  virtual void ReadRandom(const char *filename);

protected:
  TRandom *fRandom;       // Pointer to the random number generator

private:
  void Copy(IlcRndm &rn) const;

  ClassDef(IlcRndm,1)  //Random Number generator wrapper
};

#endif 

