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

/* $Id: IlcGenHMPIDlib.cxx 50533 2011-07-11 16:10:12Z hristov $ */

// Library class for particle pt and y distributions used for 
// HMPID simulations.
// To be used with IlcGenParam.
// The following particle typed can be simulated:
// phi, lambda, k
//
// Author: Annalisa Mastroserio <Annalisa.Mastroserio@ba.infn.it>
//
//

#include <TPDGCode.h>
#include <TMath.h>
#include <TRandom.h>
#include <TString.h>

#include "IlcGenHMPIDlib.h"
#include "IlcLog.h"

ClassImp(IlcGenHMPIDlib)

  //---------------------------------------
  //    Pi Plus
  //---------------------------------------
Int_t IlcGenHMPIDlib::IpPiPlus(TRandom *)
{
//PDG code
  return 211;     
}

Double_t IlcGenHMPIDlib::PtPiPlusFlat( const Double_t *, const Double_t *)
{
  //PiPlus FLAT pt-distribution
  return 1; 
}

Double_t IlcGenHMPIDlib::PtPiPlusExp( const Double_t *x, const Double_t *)
{
  //PiPlus     EXP  pt-distribution
  return x[0]*TMath::Exp(-x[0]/0.17);   
}

Double_t IlcGenHMPIDlib::YPiPlusFlat(const Double_t *,const Double_t *)
{
  //PiPlus            y-distribution 
 return 1;
}


//---------------------------------------
//        Pi Minus
//---------------------------------------
Int_t IlcGenHMPIDlib::IpPiMinus(TRandom *)
{
//PDG code
  return -211;     
}

Double_t IlcGenHMPIDlib::PtPiMinusFlat( const Double_t *, const Double_t *)
{
// PiMinus FLAT pt-distribution
  return 1; 
}

Double_t IlcGenHMPIDlib::PtPiMinusExp( const Double_t *x, const Double_t *)
{
//PiMinus     EXP  pt-distribution
  return x[0]*TMath::Exp(-x[0]/0.17);   
}

Double_t IlcGenHMPIDlib::YPiMinusFlat(const Double_t *,const Double_t *)
{
//PiMinus          y-distribution 
  return 1;
}


//--------------------------------------------
//        K Plus
//--------------------------------------------
Int_t IlcGenHMPIDlib::IpKPlus(TRandom *)
{
//PDG code
  return 321;
}

Double_t IlcGenHMPIDlib::PtKPlusFlat( const Double_t *, const Double_t *)
{
// K+ FLAT pt-distribution
  return 1;
}

Double_t IlcGenHMPIDlib::PtKPlusExp( const Double_t *x, const Double_t *)
{
// K+   EXP  pt-distribution
  return x[0]*TMath::Exp(-x[0]/0.17);
}

Double_t IlcGenHMPIDlib::YKPlusFlat(const Double_t *,const Double_t *)
{
// K+             y-distribution
  return 1;
}


//-----------------------------------------------
//         K Minus
//-----------------------------------------------
Int_t IlcGenHMPIDlib::IpKMinus(TRandom *)
{
//PDG code
  return -321;
}

Double_t IlcGenHMPIDlib::PtKMinusFlat( const Double_t *, const Double_t *)
{
// K- FLAT pt-distribution
  return 1;
}

Double_t IlcGenHMPIDlib::PtKMinusExp( const Double_t *x, const Double_t *)
{
// K-   EXP  pt-distribution
  return x[0]*TMath::Exp(-x[0]/0.17);
}

Double_t IlcGenHMPIDlib::YKMinusFlat(const Double_t *,const Double_t *)
{
// K-             y-distribution
  return 1;
}


//-----------------------------------------------
//       K0 short
//-----------------------------------------------
Int_t IlcGenHMPIDlib::IpK0s(TRandom *)
{
//PDG code
  return 310;
}

Double_t IlcGenHMPIDlib::PtK0sFlat( const Double_t *, const Double_t *)
{
// K0s FLAT pt-distribution
  return 1;
}

Double_t IlcGenHMPIDlib::PtK0sExp( const Double_t *x, const Double_t *)
{
// K0s   EXP  pt-distribution
  return x[0]*TMath::Exp(-x[0]/0.17);
}

Double_t IlcGenHMPIDlib::YK0sFlat(const Double_t *,const Double_t *)
{
// K0s             y-distribution
  return 1;
}


//---------------------------------------------
//         Phi(1020)
//---------------------------------------------
Int_t IlcGenHMPIDlib::IpPhi(TRandom *)
{
//PDG code
  return 333;     
}

Double_t IlcGenHMPIDlib::PtPhiFlat( const Double_t *, const Double_t *)
{
// Phi FLAT pt-distribution
  return 1; 
}

Double_t IlcGenHMPIDlib::PtPhiExp( const Double_t *x, const Double_t *)
{
//phi     EXP  pt-distribution
  return x[0]*TMath::Exp(-x[0]/0.17);   
}

Double_t IlcGenHMPIDlib::YPhiFlat(const Double_t *,const Double_t *)
{
//phi             y-distribution 
  return 1;
}


//-------------------------------------------------------
//                    PROTON
//-------------------------------------------------------
Int_t IlcGenHMPIDlib::IpProton(TRandom *)
{
//PDG code
  return 2122;     
}

Double_t IlcGenHMPIDlib::PtProtonFlat( const Double_t *, const Double_t *)
{
// ProtonFLAT pt-distribution

  return 1; 
}

Double_t IlcGenHMPIDlib::PtProtonExp( const Double_t *x, const Double_t *)
{
//Proton    EXP  pt-distribution
  return x[0]*TMath::Exp(-x[0]/0.17);   
}

Double_t IlcGenHMPIDlib::YProtonFlat(const Double_t *,const Double_t *)
{
  //Proton            y-distribution 
  return 1;
}


//-------------------------------------------------------
//                    PROTON-BAR
//-------------------------------------------------------
Int_t IlcGenHMPIDlib::IpProtonBar(TRandom *)
{
//PDG code
  return -2122;     
}

Double_t IlcGenHMPIDlib::PtProtonBarFlat( const Double_t *, const Double_t *)
{
// ProtonBar FLAT pt-distribution

  return 1; 
}

Double_t IlcGenHMPIDlib::PtProtonBarExp( const Double_t *x, const Double_t *)
{
//ProtonBar     EXP  pt-distribution
  return x[0]*TMath::Exp(-x[0]/0.17);   
}

Double_t IlcGenHMPIDlib::YProtonBarFlat(const Double_t *,const Double_t *)
{
  //ProtonBar             y-distribution 
  return 1;
}


//-------------------------------------------------------
//                    LAMBDA
//-------------------------------------------------------
Int_t IlcGenHMPIDlib::IpLambda(TRandom *)
{
//PDG code
  return 3122;     
}

Double_t IlcGenHMPIDlib::PtLambdaFlat( const Double_t *, const Double_t *)
{
// Lambda FLAT pt-distribution

  return 1; 
}

Double_t IlcGenHMPIDlib::PtLambdaExp( const Double_t *x, const Double_t *)
{
//Lambda     EXP  pt-distribution
  return x[0]*TMath::Exp(-x[0]/0.17);   
}

Double_t IlcGenHMPIDlib::YLambdaFlat(const Double_t *,const Double_t *)
{
  //Lambda             y-distribution 
  return 1;
}


//-------------------------------------------------------
//                    LAMBDA-BAR
//-------------------------------------------------------
Int_t IlcGenHMPIDlib::IpLambdaBar(TRandom *)
{
//PDG code
  return -3122;     
}

Double_t IlcGenHMPIDlib::PtLambdaBarFlat( const Double_t *, const Double_t *)
{
// LambdaBar FLAT pt-distribution

  return 1; 
}

Double_t IlcGenHMPIDlib::PtLambdaBarExp( const Double_t *x, const Double_t *)
{
//LambdaBar     EXP  pt-distribution
  return x[0]*TMath::Exp(-x[0]/0.17);   
}

Double_t IlcGenHMPIDlib::YLambdaBarFlat(const Double_t *,const Double_t *)
{
  //LambdaBar             y-distribution 
  return 1;
}





typedef Double_t (*GenFunc)   (const Double_t*,  const Double_t*);
typedef Int_t    (*GenFuncIp) (TRandom *);

GenFunc IlcGenHMPIDlib::GetPt(Int_t iPID, const char * sForm) const
{
// Return pointer to Pt parameterisation
  IlcDebug(1,Form("PID: %i, form: %s",iPID,sForm));	
   TString type(sForm);

   switch(iPID) {

   case kPiPlus:  
      if     (type=="FLAT")                                         return PtPiPlusFlat;
      else if(type=="EXP")                                          return PtPiPlusExp;
      else {
        IlcFatal(Form("Unknown Pt distribution form: %s",sForm));   return 0;
       }

   case kPiMinus:  
      if     (type=="FLAT")                                         return PtPiMinusFlat;
      else if(type=="EXP")                                          return PtPiMinusExp;
      else {
        IlcFatal(Form("Unknown Pt distribution form: %s",sForm));   return 0;
       }

  case kKPlus:
      if     (type=="FLAT")                                         return PtKPlusFlat;
      else if(type=="EXP")                                          return PtKPlusExp;
      else {
        IlcFatal(Form("Unknown Pt distribution form: %s",sForm));   return 0;
       }

   case kKMinus:
      if     (type=="FLAT")                                         return PtKMinusFlat;
      else if(type=="EXP")                                          return PtKMinusExp;
      else {
        IlcFatal(Form("Unknown Pt distribution form: %s",sForm));   return 0;
       }


  case kK0Short:  
      if     (type=="FLAT")                                         return PtK0sFlat;
      else if(type=="EXP")                                          return PtK0sExp;
      else {
        IlcFatal(Form("Unknown Pt distribution form: %s",sForm));   return 0;
       }


   case kPhi:  
      if     (type=="FLAT")                                         return PtPhiFlat;
      else if(type=="EXP")                                          return PtPhiExp;
      else {
        IlcFatal(Form("Unknown Pt distribution form: %s",sForm));   return 0;
       }

   case kProton:  
      if     (type=="FLAT")                                         return PtProtonFlat;
      else if(type=="EXP")                                          return PtProtonExp;
      else {
        IlcFatal(Form("Unknown Pt distribution form: %s",sForm));   return 0;
       }  

   case kProtonBar:  
      if     (type=="FLAT")                                         return PtProtonBarFlat;
      else if(type=="EXP")                                          return PtProtonBarExp;
      else {
        IlcFatal(Form("Unknown Pt distribution form: %s",sForm));   return 0;
       }  

   case kLambda0:  
      if     (type=="FLAT")                                         return PtLambdaFlat;
      else if(type=="EXP")                                          return PtLambdaExp;
      else {
        IlcFatal(Form("Unknown Pt distribution form: %s",sForm));   return 0;
       }  

   case kLambda0Bar:  
      if     (type=="FLAT")                                         return PtLambdaBarFlat;
      else if(type=="EXP")                                          return PtLambdaBarExp;
      else {
        IlcFatal(Form("Unknown Pt distribution form: %s",sForm));   return 0;
       }  

   default : IlcFatal(Form("Unknown particle type: %i",iPID));      return 0;
   }//switch
}

GenFunc IlcGenHMPIDlib::GetY(Int_t iPID, const char *sForm) const
{
  IlcDebug(1,Form("PID: %i, form: %s",iPID,sForm));	

   switch (iPID) {

   case kPiPlus:                                                   return YPiPlusFlat;
   case kPiMinus:                                                  return YPiMinusFlat;
   case kKPlus:                                                    return YKPlusFlat;
   case kKMinus:                                                   return YKMinusFlat;
   case kK0Short:                                                  return YK0sFlat;
   case kPhi:                                                      return YPhiFlat;
   case kProton:                                                   return YProtonFlat;
   case kProtonBar:                                                return YProtonBarFlat; 
   case kLambda0:                                                  return YLambdaFlat;
   case kLambda0Bar:                                               return YLambdaBarFlat;

  default  : IlcFatal(Form("Unknown particle type: %i",iPID));     return 0;

   }//switch
}

GenFuncIp IlcGenHMPIDlib::GetIp(Int_t iPID, const char *sForm) const
{
// Return pointer to particle type parameterisation
  IlcDebug(1,Form("PID: %i, form: %s",iPID,sForm));   //////////	

  switch (iPID){

    case kPiPlus:                                                return IpPiPlus;
    case kPiMinus:                                               return IpPiMinus;
    case kKPlus:                                                 return IpKPlus;
    case kKMinus:                                                return IpKMinus;
    case kK0Short:                                               return IpK0s;
    case kPhi:                                                   return IpPhi;
    case kProton:                                                return IpProton; 
    case kProtonBar:                                             return IpProtonBar;
    case kLambda0:                                               return IpLambda;
    case kLambda0Bar:                                            return IpLambdaBar; 

  default  : IlcFatal(Form("Unknown particle type: %i",iPID));  return 0;
  }
}

