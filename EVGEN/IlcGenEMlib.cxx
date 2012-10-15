/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/
  
/* $Id: IlcGenEMlib.cxx 30052 2008-11-25 14:54:18Z morsch $ */

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Implementation of IlcGenEMlib for electron, di-electron, and photon     //
// cocktail calculations.                                                  //
// It is based on IlcGenGSIlib.                                            //
//                                                                         //
// Responsible: R.Averbeck@gsi.de                                          //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////


#include "TMath.h"
#include "TRandom.h"
#include "TString.h"
#include "IlcGenEMlib.h"


ClassImp(IlcGenEMlib)

//==========================================================================
//
//              Definition of Particle Distributions
//                    
//==========================================================================

//--------------------------------------------------------------------------
//
//                              Pizero
//
//--------------------------------------------------------------------------
Int_t IlcGenEMlib::IpPizero(TRandom *)
{
// Return pizero pdg code
  return 111;     
}

Double_t IlcGenEMlib::PtPizero( const Double_t *px, const Double_t */*dummy*/ )
{
// Generate pizero pT distribution from modified Hagedorn parameterization
// taken from fit to unidentified hadrons in pp at 7 TeV
  const Double_t kc=0.000565;
  const Double_t kp0=0.2472;
  const Double_t kp1=4.354;
  const Double_t kn=7.007;
  Double_t invYield;
  Double_t x=*px;

  invYield = kc/TMath::Power(kp0+x/kp1,kn);

  return invYield*(2*TMath::Pi()*x);
   
}

Double_t IlcGenEMlib::YPizero( const Double_t *py, const Double_t */*dummy*/ )
{
  return YFlat(*py);

}

//--------------------------------------------------------------------------
//
//                              Eta
//
//--------------------------------------------------------------------------
Int_t IlcGenEMlib::IpEta(TRandom *)
{
// Return eta pdg code
  return 221;     
}

Double_t IlcGenEMlib::PtEta( const Double_t *px, const Double_t */*dummy*/ )
{
// Eta pT
  return MtScal(*px,1);
}

Double_t IlcGenEMlib::YEta( const Double_t *py, const Double_t */*dummy*/ )
{
  return YFlat(*py);

}

//--------------------------------------------------------------------------
//
//                              Rho
//
//--------------------------------------------------------------------------
Int_t IlcGenEMlib::IpRho(TRandom *)
{
// Return rho pdg code
  return 113;     
}

Double_t IlcGenEMlib::PtRho( const Double_t *px, const Double_t */*dummy*/ )
{
// Rho pT
  return MtScal(*px,2);
}

Double_t IlcGenEMlib::YRho( const Double_t *py, const Double_t */*dummy*/ )
{
  return YFlat(*py);

}

//--------------------------------------------------------------------------
//
//                              Omega
//
//--------------------------------------------------------------------------
Int_t IlcGenEMlib::IpOmega(TRandom *)
{
// Return omega pdg code
  return 223;     
}

Double_t IlcGenEMlib::PtOmega( const Double_t *px, const Double_t */*dummy*/ )
{
// Omega pT
  return MtScal(*px,3);
}

Double_t IlcGenEMlib::YOmega( const Double_t *py, const Double_t */*dummy*/ )
{
  return YFlat(*py);

}

//--------------------------------------------------------------------------
//
//                              Etaprime
//
//--------------------------------------------------------------------------
Int_t IlcGenEMlib::IpEtaprime(TRandom *)
{
// Return etaprime pdg code
  return 331;     
}

Double_t IlcGenEMlib::PtEtaprime( const Double_t *px, const Double_t */*dummy*/ )
{
// Eta pT
  return MtScal(*px,4);
}

Double_t IlcGenEMlib::YEtaprime( const Double_t *py, const Double_t */*dummy*/ )
{
  return YFlat(*py);

}

//--------------------------------------------------------------------------
//
//                              Phi
//
//--------------------------------------------------------------------------
Int_t IlcGenEMlib::IpPhi(TRandom *)
{
// Return phi pdg code
  return 333;     
}

Double_t IlcGenEMlib::PtPhi( const Double_t *px, const Double_t */*dummy*/ )
{
// Phi pT
  return MtScal(*px,5);
}

Double_t IlcGenEMlib::YPhi( const Double_t *py, const Double_t */*dummy*/ )
{
  return YFlat(*py);

}

Double_t IlcGenEMlib::YFlat(Double_t /*y*/)
{
//--------------------------------------------------------------------------
//
//                    flat rapidity distribution 
//
//--------------------------------------------------------------------------

  Double_t dNdy = 1.;   

  return dNdy;

}

//============================================================= 
//
//                    Mt-scaling  
//
//============================================================= 
//
 Double_t IlcGenEMlib::MtScal(Double_t pt, Int_t np)
{
// Function for the calculation of the Pt distribution for a 
// given particle np, from the pizero Pt distribution using  
// mt scaling. 

// MASS   0=>PIZERO, 1=>ETA, 2=>RHO, 3=>OMEGA, 4=>ETAPRIME, 5=>PHI

  const Double_t khm[6] = {0.13498, 0.54751, 0.7755, 0.78265, 0.95778, 1.01946};

  Double_t scaledPt = sqrt(pt*pt + khm[np]*khm[np] - khm[0]*khm[0]);
  Double_t scaledYield = PtPizero(&scaledPt, (Double_t*) 0);

  //     VALUE MESON/PI AT 5 GEV

  Double_t normPt = 5.;
  Double_t scaledNormPt = sqrt(normPt*normPt + khm[np]*khm[np] - khm[0]*khm[0]);
  const Double_t kfmax[6]={1., 0.48, 1.0, 0.9, 0.25, 0.4};

  Double_t norm = kfmax[np] * (PtPizero(&normPt, (Double_t*) 0) / PtPizero(&scaledNormPt, (Double_t*) 0));

  return norm*(pt/scaledPt)*scaledYield;
}

//==========================================================================
//
//                     Set Getters 
//    
//==========================================================================

typedef Double_t (*GenFunc) (const Double_t*,  const Double_t*);

typedef Int_t (*GenFuncIp) (TRandom *);

GenFunc IlcGenEMlib::GetPt(Int_t param, const char * tname) const
{
// Return pointer to pT parameterisation
   GenFunc func=0;
   TString sname(tname);

   switch (param) 
    {
    case kPizero:
      func=PtPizero;
      break;
    case kEta:
      func=PtEta;
      break;
    case kRho:
      func=PtRho;
      break;
    case kOmega:
      func=PtOmega;
      break;
    case kEtaprime:
      func=PtEtaprime;
      break;
    case kPhi:
      func=PtPhi;
      break;

    default:
      func=0;
      printf("<IlcGenEMlib::GetPt> unknown parametrisation\n");
    }
   return func;
}

GenFunc IlcGenEMlib::GetY(Int_t param, const char * tname) const
{
// Return pointer to y- parameterisation
   GenFunc func=0;
   TString sname(tname);

   switch (param) 
    {
    case kPizero:
         func=YPizero;
         break;
    case kEta:
         func=YEta;
         break;
    case kRho:
         func=YRho;
         break;
    case kOmega:
         func=YOmega;
         break;
    case kEtaprime:
         func=YEtaprime;
         break;
    case kPhi:
         func=YPhi;
         break;

    default:
        func=0;
        printf("<IlcGenEMlib::GetY> unknown parametrisation\n");
    }
    return func;
}

GenFuncIp IlcGenEMlib::GetIp(Int_t param, const char * tname) const
{
// Return pointer to particle type parameterisation
   GenFuncIp func=0;
   TString sname(tname);

   switch (param) 
    {
    case kPizero:
         func=IpPizero;
         break;
    case kEta:
         func=IpEta;
         break;
    case kRho:
         func=IpRho;
         break;
    case kOmega:
         func=IpOmega;
         break;
    case kEtaprime:
         func=IpEtaprime;
         break;
    case kPhi:
         func=IpPhi;
         break;

    default:
        func=0;
        printf("<IlcGenEMlib::GetIp> unknown parametrisation\n");
    }
    return func;
}












