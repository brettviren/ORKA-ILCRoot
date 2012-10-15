#ifndef ILCGENHIJINGPARABA_H
#define ILCGENHIJINGPARABA_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcGenHIJINGparaBa.h 16640 2007-01-31 09:18:26Z hristov $ */

// Parameterisation of pi, K, n and p eta and pt distributions
// eta: according to HIJING (shadowing + quenching)              
// pT : according to CDF measurement at 1.8 TeV                  
// Author: andreas.morsch@cern.ch

#include "IlcGenHIJINGpara.h"
class TF1;

class IlcGenHIJINGparaBa : public IlcGenHIJINGpara
{
 public:
    IlcGenHIJINGparaBa();
    IlcGenHIJINGparaBa(Int_t npart);
    virtual ~IlcGenHIJINGparaBa();
    virtual void Generate();
    virtual void Init();
 protected:
    TF1* fPtba;          //! Parametrised pt distribution for baryons
    TF1* fETAba;         //! Parametrised eta distribution for baryons

 private:
    IlcGenHIJINGparaBa(const IlcGenHIJINGparaBa &HIJINGpara);
    IlcGenHIJINGparaBa& operator = (const IlcGenHIJINGparaBa &para);

    ClassDef(IlcGenHIJINGparaBa,1) // Hijing parametrisation generator with baryons
};
#endif










