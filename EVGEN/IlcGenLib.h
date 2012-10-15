#ifndef ILCGENLIB_H
#define ILCGENLIB_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcGenLib.h 30052 2008-11-25 14:54:18Z morsch $ */

#include <TObject.h>

class TRandom;

class IlcGenLib :
  public TObject
{
 public:
//
    virtual ~IlcGenLib(){}
    typedef Double_t (*GenFunc)  (const Double_t *, const Double_t *);
    typedef Int_t    (*GenFuncIp)(TRandom *);    
    virtual GenFunc   GetPt(Int_t param, const char *tname) const   = 0;
    virtual GenFunc   GetY (Int_t param, const char *tname) const   = 0;
    virtual GenFuncIp GetIp(Int_t param, const char *tname) const   = 0;    
    ClassDef(IlcGenLib,0) // Library providing y and pT parameterisations
};
#endif







