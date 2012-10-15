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
  
/* $Id: IlcGenGSIlib.cxx 46990 2011-02-02 10:48:50Z morsch $ */

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Implementation of IlcGenlib to collect parametrisations used for        //
// GSI simulations.                                                        //
// It is an extension of IlcMUONLib providing in addition the option       //
// for different parametrisations of pt, y and ip for every particle type  //
//                                                                         //
// Responsible: Andres.Sandoval@cern.ch                                    //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#include "TMath.h"
#include "TRandom.h"
#include "TString.h"
#include "IlcGenGSIlib.h"


ClassImp(IlcGenGSIlib)

//==========================================================================
//
//              Definition of Particle Distributions
//                    
//==========================================================================
//
//                         Upsilon 
//
//--------------------------------------------------------------------------
//
//                  upsilon particle composition
//
//--------------------------------------------------------------------------
Int_t IlcGenGSIlib::IpUpsilon(TRandom *)
{
// Return upsilon pdg code

  return 553;     

}
Double_t IlcGenGSIlib::PtUpsilonFlat( const Double_t *px, const Double_t */*dummy*/ )
{
//--------------------------------------------------------------------------
//
//                   upsilon pt-distribution FLAT
//
//____________________________________________________________--------------
    
  const Double_t kptmin = 0.0;
  const Double_t kptmax  = 15.0;
  Double_t x=*px;
  Double_t weight = 0.;

  if ((x > kptmin) &&  (x < kptmax)) weight = 1.;

  return weight;
   
}
Double_t IlcGenGSIlib::YUpsilonFlat(const Double_t *py, const Double_t */*dummy*/)
{
//--------------------------------------------------------------------------
//
//                    upsilon y-distribution FLAT
//
//--------------------------------------------------------------------------

  const Double_t ky0 = 1.5;
  const Double_t kb=1.;
  Double_t yu;
  Double_t y=TMath::Abs(*py);

  if (y < ky0)
    yu=kb;
  else
    yu = 0.;

  return yu;

}
Double_t IlcGenGSIlib::PtUpsilonRitman( const Double_t *px, const Double_t */*dummy*/ )
{
//--------------------------------------------------------------------------
//
//                 upsilon pt-distribution RITMAN
//
//--------------------------------------------------------------------------

  const Double_t kpt0 = 4.7;
  const Double_t kxn  = 3.5;
  Double_t x=*px;

  Double_t pass1 = 1.+((x*x)/(kpt0*kpt0));

  return x/TMath::Power(pass1,kxn);
   
}
Double_t IlcGenGSIlib::YUpsilonRitman(const Double_t *py, const Double_t */*dummy*/)
{
//--------------------------------------------------------------------------
//
//                  upsilon y-distribution RITMAN
//
//--------------------------------------------------------------------------

  const Double_t ky0 = 3.;
  const Double_t kb=1.;
  Double_t yu;
  Double_t y=TMath::Abs(*py);

  if (y < ky0)
    yu=kb;
  else
    yu=kb*TMath::Exp(-(y-ky0)*(y-ky0)/2);

  return yu;
   
}
Double_t IlcGenGSIlib::PtUpsilonKarel( const Double_t */*px*/, const Double_t */*dummy*/ )
{
//--------------------------------------------------------------------------
//
//                 upsilon pt-distribution kAREL
//
//--------------------------------------------------------------------------
// to implement

  return 0.1;   

}
Double_t IlcGenGSIlib::YUpsilonKarel(const Double_t */*py*/, const Double_t */*dummy*/)
{
//--------------------------------------------------------------------------
//
//                  upsilon y-distribution KAREL
//
//--------------------------------------------------------------------------
  
  //to implement

  return 0.2;  

}
Double_t IlcGenGSIlib::PtUpsilonMUON( const Double_t *px, const Double_t */*dummy*/ )
{
//--------------------------------------------------------------------------
//
//                 upsilon pt-distribution MUONlib
//
//--------------------------------------------------------------------------

  const Double_t kpt0 = 5.3;
  const Double_t kxn  = 2.5;
  Double_t x=*px;

  Double_t pass1 = 1.+(x/kpt0)*(x/kpt0);

  return x/TMath::Power(pass1,kxn);

}
Double_t IlcGenGSIlib::YUpsilonMUON(const Double_t *py, const Double_t */*dummy*/)
{
//--------------------------------------------------------------------------
//
//                   upsilon y-distribution MUONlib
//
//--------------------------------------------------------------------------

  const Double_t ky0 = 3.;
  const Double_t kb=1.;
  Double_t yu;
  Double_t y=TMath::Abs(*py);

  if (y < ky0)
    yu=kb;
  else
    yu=kb*TMath::Exp(-(y-ky0)*(y-ky0)/2);

  return yu;

}
//--------------------------------------------------------------------------
//
//                             J/Psi
//
Int_t IlcGenGSIlib::IpJpsi(TRandom *)
{
//--------------------------------------------------------------------------
//
//                    J/Psi particle composition
//
//--------------------------------------------------------------------------

  return 443;     

}
Double_t IlcGenGSIlib::PtJpsiFlat( const Double_t *px, const Double_t */*dummy*/ )
{
//--------------------------------------------------------------------------
//
//                   J/Psi pt-distribution FLAT
//
//--------------------------------------------------------------------------

  const Double_t kptmin = 0.0;
  const Double_t kptmax  = 15.0;
  Double_t x=*px;
  Double_t weight = 0.;

  if ((x > kptmin) && (x < kptmax)) weight = 1.;

  return weight;
   
}
Double_t IlcGenGSIlib::YJpsiFlat(const Double_t *py, const Double_t */*dummy*/)
{
//--------------------------------------------------------------------------
//
//                    J/Psi y-distribution FLAT
//
//--------------------------------------------------------------------------

  const Double_t ky0 = 1.5;
  const Double_t kb=1.;
  Double_t yu;
  Double_t y=TMath::Abs(*py);

  if (y < ky0)
    yu=kb;
  else
    yu = 0.;

  return yu;

}
Double_t IlcGenGSIlib::PtJpsiMUON( const Double_t *px, const Double_t */*dummy*/ )
{
//--------------------------------------------------------------------------
//
//                   J/Psi pt-distribution MUONlib
//
//--------------------------------------------------------------------------

  const Double_t kpt0 = 4.;
  const Double_t kxn  = 3.6;
  Double_t x=*px;

  Double_t pass1 = 1.+(x/kpt0)*(x/kpt0);
  return x/TMath::Power(pass1,kxn);
   
}
Double_t IlcGenGSIlib::PtJpsiRitman( const Double_t *px, const Double_t */*dummy*/ )
{
//--------------------------------------------------------------------------
//
//                   J/Psi pt-distribution Ritman
//
//--------------------------------------------------------------------------

  const Double_t kpt0 = 2.3;
  const Double_t kxn  = 3.5;
  Double_t x=*px;

  Double_t pass1 = 1.+(x/kpt0)*(x/kpt0);

  return x/TMath::Power(pass1,kxn);
   
}
Double_t IlcGenGSIlib::YJpsiMUON(const Double_t *py, const Double_t */*dummy*/)
{
//--------------------------------------------------------------------------
//
//                    J/Psi y-distribution MUONlib
//
//--------------------------------------------------------------------------

  const Double_t ky0 = 4.;
  const Double_t kb=1.;
  Double_t yj;
  Double_t y=TMath::Abs(*py);

  if (y < ky0)
    yj=kb;
  else
    yj=kb*TMath::Exp(-(y-ky0)*(y-ky0)/2);
  return yj;

}
//--------------------------------------------------------------------------
//
//                  J/Psi pt-distribution by Sergei
//
//--------------------------------------------------------------------------
//Double_t IlcGenGSIlib::PtJpsi( Double_t *px, Double_t */*dummy*/ )
//{

//  return x = gRandom->Rndm()*10.;

//}
//--------------------------------------------------------------------------
//
//                  J/Psi y-distribution by Sergei
//
//--------------------------------------------------------------------------
/*Double_t IlcGenGSIlib::YJpsi(Double_t *py, Double_t *dummy)
{

  const Double_t ky0 = 4.;
  const Double_t kb=1.;
  Double_t yj;
  Double_t y=TMath::Abs(*py);
  //
  if (y < ky0)
    yj=kb;
  else
    yj=kb*TMath::Exp(-(y-ky0)*(y-ky0)/2);
  return yj;

}
*/
//--------------------------------------------------------------------------
//
//                        Charm
//
//--------------------------------------------------------------------------
Int_t IlcGenGSIlib::IpCharm(TRandom *ran)
{
//
//                    charm particle composition
//
//--------------------------------------------------------------------------
  
    Float_t random;
    Int_t ip;
    // 411,421,431,4122
    random = ran->Rndm();
    if (random < 0.5) {
	ip=411;
    } else if (random < 0.75) {
	ip=421;
    } else if (random < 0.90) {
	ip=431;
    } else {
	ip=4122;
    }
    if (ran->Rndm() < 0.5) {ip=-ip;}
    
    return ip;

}
Double_t IlcGenGSIlib::PtCharmFlat( const Double_t *px, const Double_t */*dummy*/)
{
//--------------------------------------------------------------------------
//
//                    charm pt-distribution, FLAT
//
//--------------------------------------------------------------------------

  Double_t x=*px;

  if (x>10.)  x = 0.;
  else x=1.;
  return x ;

}
Double_t IlcGenGSIlib::PtCharmGSI( const Double_t *px, const Double_t */*dummy*/)
{
//--------------------------------------------------------------------------
//
//                    charm pt-distribution, from Dariuzs Miskowiec
//
//--------------------------------------------------------------------------

  //Taken from PYTHIA with MRS D-' (3031 from PDFLIB), K=3.0
  const Double_t kp1 = 1.3;
  const Double_t kp2  = 0.39;
  const Double_t kp3  = 0.018;
  const Double_t kp4  = 0.91;
  Double_t x=*px;

  Double_t pass1 =TMath::Exp(-x/kp2)  ;
  Double_t pass2 =TMath::Exp(-x/kp4)  ;
  return TMath::Power(x,kp1) * (pass1 + kp3 * pass2);

}
Double_t IlcGenGSIlib::PtCharmMUON( const Double_t *px, const Double_t */*dummy*/)
{
//--------------------------------------------------------------------------
//
//                    charm pt-distribution, from MUONlib
//
//--------------------------------------------------------------------------

  const Double_t kpt0 = 4.08;
  const Double_t kxn  = 9.40;
  Double_t x=*px;

  Double_t pass1 = 1.+(x/kpt0)*(x/kpt0);

  return x/TMath::Power(pass1,kxn);

}
Double_t IlcGenGSIlib::YCharm( const Double_t *px, const Double_t */*dummy*/)
{
//--------------------------------------------------------------------------
//
//                    charm y-distribution
//
//--------------------------------------------------------------------------

    Double_t *dum=0;

    return YJpsiMUON(px,dum);

}
//--------------------------------------------------------------------------
//
//                        Beauty
//
//--------------------------------------------------------------------------
Int_t IlcGenGSIlib::IpBeauty(TRandom *ran)
{  
//
//                    beauty particle composition
//
//--------------------------------------------------------------------------

    Float_t random;
    Int_t ip;
    random = ran->Rndm();
    if (random < 0.5) {
	ip=511;
    } else if (random < 0.75) {
	ip=521;
    } else if (random < 0.90) {
	ip=531;
    } else {
	ip=5122;
    }
    if (ran->Rndm() < 0.5) {ip=-ip;}
    
    return ip;
}
Double_t IlcGenGSIlib::PtBeautyFlat( const Double_t *px, const Double_t */*dummy*/)
{
//--------------------------------------------------------------------------
//
//                    beauty pt-distribution, FLAT
//
//--------------------------------------------------------------------------

  Double_t x=*px;

  if (x>10.) x=0.;
  else x = 1.;
  return x ;

}
Double_t IlcGenGSIlib::PtBeautyGSI( const Double_t *px, const Double_t */*dummy*/)
{
//--------------------------------------------------------------------------
//
//
//                    beauty pt-distribution, from D. Miskowiec
//
//--------------------------------------------------------------------------

  //Taken from PYTHIA with MRS D-' (3031 from PDFLIB), K=3.0
  const Double_t kp1 = 1.3;
  const Double_t kp2  = 1.78;
  const Double_t kp3  = 0.0096;
  const Double_t kp4  = 4.16;
  Double_t x=*px;

  Double_t pass1 =TMath::Exp(-x/kp2)  ;
  Double_t pass2 =TMath::Exp(-x/kp4)  ;

  return TMath::Power(x,kp1) * (pass1 + kp3 * pass2);

}
Double_t IlcGenGSIlib::PtBeautyMUON( const Double_t *px, const Double_t */*dummy*/)
{
//--------------------------------------------------------------------------
//
//                    beauty pt-distribution, from MUONlib
//
//--------------------------------------------------------------------------

  const Double_t kpt0 = 4.;
  const Double_t kxn  = 3.6;
  Double_t x=*px;

  Double_t pass1 = 1.+(x/kpt0)*(x/kpt0);

  return x/TMath::Power(pass1,kxn);

}
Double_t IlcGenGSIlib::YBeauty( const Double_t *px, const Double_t */*dummy*/)
{
//--------------------------------------------------------------------------
//
//                    beauty y-distribution
//
//--------------------------------------------------------------------------

    Double_t *dum=0;

    return YJpsiMUON(px,dum);

}
//--------------------------------------------------------------------------
//
//                          Eta
//
//--------------------------------------------------------------------------
Int_t IlcGenGSIlib::IpEta(TRandom *)
{
//
//                 eta particle composition 
//
//--------------------------------------------------------------------------

  return 221;     

}
Double_t IlcGenGSIlib::PtEtaPHOS( const Double_t *px, const Double_t */*dummy*/ )
{
//--------------------------------------------------------------------------
//
//                  eta pt-distribution
//
//____________________________________________________________--------------

  return PtScal(*px,2);  //  2==> Eta in the PtScal function
   
}
Double_t IlcGenGSIlib::YEtaPHOS(const Double_t *py, const Double_t */*dummy*/)
{
//--------------------------------------------------------------------------
//
//                   eta y-distribution 
//
//--------------------------------------------------------------------------

  const Double_t ka    = 1000.;
  const Double_t kdy   = 4.;

  Double_t y=TMath::Abs(*py);

  Double_t ex = y*y/(2*kdy*kdy);

  return ka*TMath::Exp(-ex);

}
//--------------------------------------------------------------------------
//
//                       Etaprime
//
//--------------------------------------------------------------------------
Int_t IlcGenGSIlib::IpEtaprime(TRandom *)
{
//
//                 etaprime particle composition 
//
//--------------------------------------------------------------------------

  return 331;     

}
Double_t IlcGenGSIlib::PtEtaprimePHOS( const Double_t *px, const Double_t */*dummy*/ )
{
//--------------------------------------------------------------------------
//
//                 etaprime pt-distribution
//
//____________________________________________________________--------------

  return PtScal(*px,4);  //  4==> Etaprime in the PtScal function
   
}
Double_t IlcGenGSIlib::YEtaprimePHOS(const Double_t *py, const Double_t */*dummy*/)
{
//--------------------------------------------------------------------------
//
//                  etaprime y-distribution 
//
//--------------------------------------------------------------------------

  const Double_t ka    = 1000.;
  const Double_t kdy   = 4.;

  Double_t y=TMath::Abs(*py);

  Double_t ex = y*y/(2*kdy*kdy);

  return ka*TMath::Exp(-ex);

}
//--------------------------------------------------------------------------
//
//                       omega 
//
//--------------------------------------------------------------------------
Int_t IlcGenGSIlib::IpOmega(TRandom *)
{
//
//                 omega particle composition 
//
//--------------------------------------------------------------------------

  return 223;     

}
Double_t IlcGenGSIlib::PtOmega( const Double_t *px, const Double_t */*dummy*/ )
{
//--------------------------------------------------------------------------
//
//                  omega pt-distribution
//
//____________________________________________________________--------------

  return PtScal(*px,3);  //  3==> Omega in the PtScal function
   
}
Double_t IlcGenGSIlib::YOmega(const Double_t *py, const Double_t */*dummy*/)
{
//--------------------------------------------------------------------------
//
//                   omega y-distribution 
//
//--------------------------------------------------------------------------

  const Double_t ka    = 1000.;
  const Double_t kdy   = 4.;


  Double_t y=TMath::Abs(*py);

  Double_t ex = y*y/(2*kdy*kdy);

  return ka*TMath::Exp(-ex);

}
//--------------------------------------------------------------------------
//
//                       Rho 
//
//--------------------------------------------------------------------------

Int_t IlcGenGSIlib::IpRho(TRandom *)
{
//
//                 rho particle composition 
//
//--------------------------------------------------------------------------

  return 113;     

}
Double_t IlcGenGSIlib::PtRho( const Double_t *px, const Double_t */*dummy*/ )
{
//--------------------------------------------------------------------------
//
//                  rho pt-distribution
//
//____________________________________________________________--------------

  return PtScal(*px,10);  //  10==> Rho in the PtScal function
   
}
Double_t IlcGenGSIlib::YRho(const Double_t *py, const Double_t */*dummy*/)
{
//--------------------------------------------------------------------------
//
//                   rho y-distribution 
//
//--------------------------------------------------------------------------

  const Double_t ka    = 1000.;
  const Double_t kdy   = 4.;


  Double_t y=TMath::Abs(*py);

  Double_t ex = y*y/(2*kdy*kdy);

  return ka*TMath::Exp(-ex);

}
//--------------------------------------------------------------------------
//
//                              Pion
//
//--------------------------------------------------------------------------
Int_t IlcGenGSIlib::IpPionPHOS(TRandom *ran)
{
//
//                 particle composition  pi+, pi0, pi-
//
//--------------------------------------------------------------------------

    Float_t random = ran->Rndm();

    if ( (3.*random)  < 1. ) 
    {
          return 211 ;
    } 
    else
    {  
      if ( (3.*random) >= 2.)
      {
         return -211 ;
      }
      else 
      {
        return 111  ;
      }
    }
}
Double_t IlcGenGSIlib::PtPion( const Double_t *px, const Double_t */*dummy*/ )
{
//--------------------------------------------------------------------------
//
//                  pion pt-distribution
//
//       Pion transverse momentum distribtuion as in IlcGenMUONlib class, 
//       version 3.01 of ilcroot:
//       PT-PARAMETERIZATION CDF, PRL 61(88) 1819
//       POWER LAW FOR PT > 500 MEV
//       MT SCALING BELOW (T=160 MEV)
//
//____________________________________________________________--------------

  const Double_t kp0 = 1.3;
  const Double_t kxn = 8.28;
  const Double_t kxlim=0.5;
  const Double_t kt=0.160;
  const Double_t kxmpi=0.139;
  const Double_t kb=1.;
  Double_t y, y1, kxmpi2, ynorm, a;
  Double_t x=*px;

  y1=TMath::Power(kp0/(kp0+kxlim),kxn);
  kxmpi2=kxmpi*kxmpi;
  ynorm=kb*(TMath::Exp(-sqrt(kxlim*kxlim+kxmpi2)/kt));
  a=ynorm/y1;
  if (x > kxlim)
    y=a*TMath::Power(kp0/(kp0+x),kxn);
  else
    y=kb*TMath::Exp(-sqrt(x*x+kxmpi2)/kt);
  return y*x;
   
}
Double_t IlcGenGSIlib::YPion(const Double_t *py, const Double_t */*dummy*/)
{
//--------------------------------------------------------------------------
//
//                    pion y-distribution 
//
//--------------------------------------------------------------------------

  const Double_t ka    = 7000.;   
  const Double_t kdy   = 4.;

  Double_t y=TMath::Abs(*py);

  Double_t ex = y*y/(2*kdy*kdy);

  return ka*TMath::Exp(-ex);

}
Int_t IlcGenGSIlib::IpKaonPHOS(TRandom *ran)
{
//--------------------------------------------------------------------------
//
//
//                        Kaon
//--------------------------------------------------------------------------
//
//                kaon particle composition K+, K-, Ko_short, Ko_long
//
//--------------------------------------------------------------------------

    Float_t random = ran->Rndm();
    Float_t random2 = ran->Rndm();
    if (random2 < 0.5) 
    {
      if (random < 0.5) {       
        return  321;   //   K+
      } else {
        return -321;   // K-
      }
    }
    else
    {  
      if (random < 0.5) {       
        return  130;   // K^0 short
      } else {  
        return  310;   // K^0 long
      }
    }
}
Double_t IlcGenGSIlib::PtKaonPHOS( const Double_t *px, const Double_t */*dummy*/ )
{
//--------------------------------------------------------------------------
//
//                   kaon pt-distribution
//
//____________________________________________________________--------------

  return PtScal(*px,1);  //  1==> Kaon in the PtScal function
   
}
Double_t IlcGenGSIlib::YKaonPHOS(const Double_t *py, const Double_t */*dummy*/)
{
//--------------------------------------------------------------------------
//
//                    kaon y-distribution 
//
//--------------------------------------------------------------------------

  const Double_t ka    = 1000.;
  const Double_t kdy   = 4.;

  Double_t y=TMath::Abs(*py);

  Double_t ex = y*y/(2*kdy*kdy);

  return ka*TMath::Exp(-ex);

}
//--------------------------------------------------------------------------
//
//                        Phi  
//
Int_t IlcGenGSIlib::IpPhi(TRandom *)
{
//--------------------------------------------------------------------------
//
//                 particle composition 
//
//--------------------------------------------------------------------------

  return 333;     

}
Double_t IlcGenGSIlib::PtPhiPHOS( const Double_t *px, const Double_t */*dummy*/ )
{
//--------------------------------------------------------------------------
//
//                   phi pt-distribution
//
//____________________________________________________________--------------

  return PtScal(*px,5);  //  5==> Phi in the PtScal function
   
}
Double_t IlcGenGSIlib::YPhiPHOS(const Double_t *py, const Double_t */*dummy*/)
{
//--------------------------------------------------------------------------
//
//                    phi y-distribution 
//
//--------------------------------------------------------------------------

  const Double_t ka    = 1000.;
  const Double_t kdy   = 4.;


  Double_t y=TMath::Abs(*py);
 
  Double_t ex = y*y/(2*kdy*kdy);

  return ka*TMath::Exp(-ex);

}
Int_t IlcGenGSIlib::IpBaryons(TRandom *ran)
{
//--------------------------------------------------------------------------
//
//                          Baryons
//
//--------------------------------------------------------------------------
//
//                 baryons particle composition p, pbar, n, nbar
//
//--------------------------------------------------------------------------

    Float_t random = ran->Rndm();
    Float_t random2 = ran->Rndm();
    if (random2 < 0.5) 
    {
      if (random < 0.5) {       
        return  2212;   //   p
      } else {
        return -2212;   // pbar
      }
    }
    else
    {  
      if (random < 0.5) {       
        return  2112;   // n
      } else {  
        return -2112;   // n bar
      }
    }
}
Double_t IlcGenGSIlib::PtBaryons( const Double_t *px, const Double_t */*dummy*/ )
{
//--------------------------------------------------------------------------
//
//                  baryons pt-distribution
//
//____________________________________________________________--------------

  return PtScal(*px,6);  //  6==> Baryon in the PtScal function
   
}
Double_t IlcGenGSIlib::YBaryons(const Double_t *py, const Double_t */*dummy*/)
{
//--------------------------------------------------------------------------
//
//                   baryons y-distribution 
//
//--------------------------------------------------------------------------

  const Double_t ka    = 1000.;
  const Double_t kdy   = 4.;

  Double_t y=TMath::Abs(*py);

  Double_t ex = y*y/(2*kdy*kdy);

  return ka*TMath::Exp(-ex);

}
//============================================================= 
//
//                    Mt-scaling as in IlcGenPHOSlib  
//
//============================================================= 
//
 Double_t IlcGenGSIlib::PtScal(Double_t pt, Int_t np)
{
// Function for the calculation of the Pt distribution for a 
// given particle np, from the pion Pt distribution using the 
// mt scaling. 

// It was taken from IlcGenPHOSlib ilcroot version 3.04, which 
// is an update of the one in IlcGenMUONlib ilcroot version 3.01
// with an extension for Baryons but supressing Rhos
// np = 1=>Pions 2=>Kaons 3=>Etas 4=>Omegas 5=>ETA' 6=>PHI
//      7=>BARYONS-BARYONBARS

// The present adds the Rhos

// MASS   0=>PI, 1=>K, 2=>ETA, 3=>OMEGA, 4=>ETA', 5=>PHI 
//        6=>BARYON-BARYONBAR, 10==>RHO

  const Double_t khm[11] = {0.1396, 0.494,  0.547,    0.782,   0.957,   1.02, 
                                         0.938, 0. , 0., 0., 0.769};

  //     VALUE MESON/PI AT 5 GEV

  const Double_t kfmax[11]={1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.};

  Double_t f5=TMath::Power(((sqrt(100.018215)+2.)/(sqrt(100.+khm[np]*khm[np])+2.0)),12.3);
  Double_t kfmax2=f5/kfmax[np];
  // PIONS
  Double_t ptpion=100.*PtPion(&pt, (Double_t*) 0);
  Double_t fmtscal=TMath::Power(((sqrt(pt*pt+0.018215)+2.)/
                                 (sqrt(pt*pt+khm[np]*khm[np])+2.0)),12.3)/ kfmax2;
  return fmtscal*ptpion;
}

//==========================================================================
//
//                     Set Getters 
//    
//==========================================================================

typedef Double_t (*GenFunc) (const Double_t*,  const Double_t*);

typedef Int_t (*GenFuncIp) (TRandom *);

GenFunc IlcGenGSIlib::GetPt(Int_t param, const char * tname) const
{
// Return pointer to pT parameterisation
   GenFunc func=0;
   TString sname(tname);

   switch (param) 
    {
    case kUpsilon:
      if (sname=="FLAT"){
        func= PtUpsilonFlat;
        break;
      }
      if (sname=="MUON"){
        func= PtUpsilonMUON;
        break;
      }
      if (sname=="RITMAN"){
        func=PtUpsilonRitman;
        break;
      }
      if (sname=="KAREL"){
        func=PtUpsilonKarel;
        break;
      }
      func=0;
      printf("<IlcGenGSIlib::GetPt> unknown parametrisation\n");
      break;

    case kJPsi:
      if (sname=="FLAT"){
        func= PtJpsiFlat;
        break;
      }
      if (sname=="MUON"){
        func= PtJpsiMUON;
        break;
      }
      //      if (sname=="SERGEI"){
      //	func= PtJpsi;
      //	break;
      //     }
      func=0;
      printf("<IlcGenGSIlib::GetPt> unknown parametrisation\n");
      break;

    case kCharm: 
      if (sname=="FLAT"){
        func= PtCharmFlat;
        break;
      }

      if (sname=="MUON"){
        func= PtCharmMUON;
        break;
      }

      if (sname=="GSI"){
        func= PtCharmGSI;
        break;
      }
      func=0;
      printf("<IlcGenGSIlib::GetPt> unknown parametrisation\n");
      break;

    case kBeauty: 
      if (sname=="FLAT"){
        func= PtBeautyFlat;
        break;
      }
      if (sname=="MUON"){
        func= PtBeautyMUON;
        break;
      }
      if (sname=="GSI"){
        func= PtBeautyGSI;
        break;
      }
      func=0;
      printf("<IlcGenGSIlib::GetPt> unknown parametrisation\n");
      break;


    case kEta:
      func=PtEtaPHOS;
      break;

    case kEtaprime:
      func=PtEtaprimePHOS;
      break;

    case kOmega:
      func=PtOmega;
      break;

    case kRho:
      func=PtRho;
      break;

    case kKaon:
      func=PtKaonPHOS;
      break;

    case kPion:
      func=PtPion;
      break;

    case kPhi:
      func=PtPhiPHOS;
      break;

	 //    case kLambda:
	 //         func=PtLambda;
	 //         break;

    case kBaryons:
      func=PtBaryons;
      break;

    default:
      func=0;
      printf("<IlcGenGSIlib::GetPt> unknown parametrisation\n");
    }
   return func;
}

GenFunc IlcGenGSIlib::GetY(Int_t param, const char * tname) const
{
// Return pointer to y- parameterisation
   GenFunc func=0;
   TString sname(tname);

   switch (param) 
    {
    case kUpsilon:
      if (sname=="FLAT"){
	func= YUpsilonFlat;
	break;
      }
      if (sname=="MUON"){
	func= YUpsilonMUON;
	break;
      }
      if (sname=="RITMAN"){
	func=YUpsilonRitman;
	break;
      }
      if (sname=="KAREL"){
	func=YUpsilonKarel;
	break;
      }
      func=0;
      printf("<IlcGenGSIlib::GetY> unknown parametrisation\n");
      break;

    case kJPsi:
      if (sname=="FLAT"){
	func= YJpsiFlat;
	break;
      }
      if (sname=="MUON"){
	func= YJpsiMUON;
	break;
      }
      //      if (sname=="SERGEI"){
      //	func= YJpsi;
      //	break;
      //      }
   
      func=0;
      printf("<IlcGenGSIlib::GetY> unknown parametrisation\n");
      break;
     
    case kCharm: 
	func= YCharm;
	break;

    case kBeauty: 
	func= YBeauty;
	break;

    case kEta:
         func=YEtaPHOS;
         break;

    case kEtaprime:
         func=YEtaprimePHOS;
         break;

    case kOmega:
         func=YOmega;
         break;

    case kRho:
	 func=YRho;
	 break;

    case kKaon:
         func=YKaonPHOS;
         break;

    case kPion:
         func=YPion;
         break;

    case kPhi:
         func=YPhiPHOS;
         break;

	 //    case kLambda:
	 //         func=YLambda;
	 //         break;

    case kBaryons:
         func=YBaryons;
         break;

    default:
        func=0;
        printf("<IlcGenGSIlib::GetY> unknown parametrisation\n");
    }
    return func;
}

GenFuncIp IlcGenGSIlib::GetIp(Int_t param, const char * tname) const
{
// Return pointer to particle type parameterisation
   GenFuncIp func=0;
   TString sname(tname);

   switch (param) 
    {
    case kUpsilon:
	func= IpUpsilon;
	break;

    case kJPsi:
	func= IpJpsi;
	break;

    case kCharm: 
	func= IpCharm;
	break;

    case kBeauty: 
	func= IpBeauty;
	break;

    case kEta:
         func=IpEta;
         break;

    case kEtaprime:
         func=IpEtaprime;
         break;

    case kOmega:
         func=IpOmega;
         break;

    case kRho:
	 func=IpRho;
	 break;

    case kKaon:
         func=IpKaonPHOS;
         break;

    case kPion:
         func=IpPionPHOS;
         break;

    case kPhi:
         func=IpPhi;
         break;

	 //    case kLambda:
	 //         func=IpLambda;
	 //         break;

    case kBaryons:
         func=IpBaryons;
         break;

    default:
        func=0;
        printf("<IlcGenGSIlib::GetIp> unknown parametrisation\n");
    }
    return func;
}












