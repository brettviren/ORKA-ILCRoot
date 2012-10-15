#ifndef ILCGENPMDLIB_H
#define ILCGENPMDLIB_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcGenPMDlib.h 30052 2008-11-25 14:54:18Z morsch $ */

//
// Library of generators for PMD
// providing y and pt parametrisation
// for generated tracks
//

class TRandom;

#include "IlcGenLib.h"

class IlcGenPMDlib :public IlcGenLib
{
 public:
    enum constants{kPion, kEta};
    GenFunc   GetPt(Int_t param, const char* tname=0) const;
    GenFunc   GetY (Int_t param, const char* tname=0) const;
    GenFuncIp GetIp(Int_t param, const char* tname=0) const;    
 private:
// Neutral pions
    static Double_t PtPi0(const Double_t *px, const Double_t *dummy);
    static Double_t PtScal(Double_t pt, Int_t np);
    static Double_t YPi0( const Double_t *py, const Double_t *dummy);
    static Int_t    IpPi0(TRandom *ran);
// Etas
    static Double_t PtEta(const Double_t *px, const Double_t *dummy);
    static Double_t YEta( const Double_t *py, const Double_t *dummy);
    static Int_t    IpEta(TRandom *ran);
//
    ClassDef(IlcGenPMDlib,0) // Library providing y and pT parameterisations
};
#endif







