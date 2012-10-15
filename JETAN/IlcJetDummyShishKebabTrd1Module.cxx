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

/* $Id: IlcJetDummyShishKebabTrd1Module.cxx 34196 2009-08-10 15:53:11Z morsch $ */

//_________________________________________________________________________
// Main class for TRD1 geometry of Shish-Kebab case.
// Author: Aleksei Pavlinov(WSU).
// Sep 20004 - Nov 2006
// See web page with description of Shish-Kebab geometries:
// http://pdsfweb01.nersc.gov/~pavlinov/ILC/SHISHKEBAB/RES/shishkebabILC.html
// Nov 9,2006 - added cas of 3X3
//_________________________________________________________________________

//
// Class modified and implemented in JETAN 
//
// M. Estienne
// 

#include "IlcLog.h"
#include "IlcJetDummyShishKebabTrd1Module.h"
#include "IlcJetDummyGeo.h"

#include <Riostream.h>

ClassImp(IlcJetDummyShishKebabTrd1Module)

  IlcJetDummyGeo *IlcJetDummyShishKebabTrd1Module::fgGeometry=0; 
  Double_t IlcJetDummyShishKebabTrd1Module::fga=0.; 
  Double_t IlcJetDummyShishKebabTrd1Module::fga2=0.; 
  Double_t IlcJetDummyShishKebabTrd1Module::fgb=0.; 
  Double_t IlcJetDummyShishKebabTrd1Module::fgr=0.; 
  Double_t IlcJetDummyShishKebabTrd1Module::fgangle=0.;   // around one degree 
  Double_t IlcJetDummyShishKebabTrd1Module::fgtanBetta=0; //

//_____________________________________________________________________________
IlcJetDummyShishKebabTrd1Module::IlcJetDummyShishKebabTrd1Module(Double_t theta, IlcJetDummyGeo *g) 
  : TNamed(),
    fOK(),
    fA(0.),
    fB(0.),
    fThetaA(0.),
    fTheta(theta),
    fOK1(),
    fOK2(),
    fOB(),
    fOB1(),
    fOB2(),
    fOK3X3(),
    fDebug(0)
{ 
  // theta in radians ; first object shold be with theta=pi/2.
  if(fgGeometry==0) {
    fTheta = TMath::PiOver2();
    fgGeometry = g;
    if(GetParameters()) {
      DefineFirstModule();
    }
  } else Warning("IlcJetDummyShishKebabTrd1Module(theta)","You should call this constractor just once !!");
  DefineName(fTheta);
  IlcInfo(Form("IlcJetDummyShishKebabTrd1Module - first module:  theta %1.4f geometry %s",fTheta,g->GetName()));  
}

//_____________________________________________________________________________
IlcJetDummyShishKebabTrd1Module::IlcJetDummyShishKebabTrd1Module(IlcJetDummyShishKebabTrd1Module &leftNeighbor) 
  : TNamed(),
    fOK(),
    fA(0.),
    fB(0.),
    fThetaA(0.),
    fTheta(0.),
    fOK1(),
    fOK2(),
    fOB(),
    fOB1(),
    fOB2(),
    fOK3X3(),
    fDebug(0)
{ 
  //  printf("** Left Neighbor : %s **\n", leftNeighbor.GetName());
  fTheta  = leftNeighbor.GetTheta() - fgangle; 

  TObject::SetUniqueID(leftNeighbor.GetUniqueID()+1);

  Init(leftNeighbor.GetA(),leftNeighbor.GetB());
}

//________________________________________________________________
IlcJetDummyShishKebabTrd1Module::IlcJetDummyShishKebabTrd1Module(const IlcJetDummyShishKebabTrd1Module& mod) 
  : TNamed(mod.GetName(),mod.GetTitle()),
    fOK(mod.fOK),
    fA(mod.fA),
    fB(mod.fB),
    fThetaA(mod.fThetaA),
    fTheta(mod.fTheta),
    fOK1(mod.fOK1),
    fOK2(mod.fOK2),
    fOB(mod.fOB),
    fOB1(mod.fOB1),
    fOB2(mod.fOB2),
    fDebug(mod.fDebug)
{
  //copy ctor
  for (Int_t i=0; i<3; i++) fOK3X3[i] = mod.fOK3X3[i];
}

//________________________________________________________________
void IlcJetDummyShishKebabTrd1Module::Init(Double_t A, Double_t B)
{ 
  // Define parameter module from parameters A,B from previos.
  Double_t yl = (fgb/2)*TMath::Sin(fTheta) + (fga/2)*TMath::Cos(fTheta) + fgr, y = yl;
  Double_t xl = (yl - B) / A;     // y=A*x+B

  if(fDebug>1){
    Double_t xp1 = (fga/2. + fgb/2.*fgtanBetta)/(TMath::Sin(fTheta) + fgtanBetta*TMath::Cos(fTheta));
    printf(" xp1 %9.3f \n ", xp1);
  }
  // xp1 == xp => both methods give the same results - 3-feb-05
  Double_t alpha = TMath::Pi()/2. + fgangle/2;
  Double_t xt = (fga+fga2)*TMath::Tan(fTheta)*TMath::Tan(alpha)/(4.*(1.-TMath::Tan(fTheta)*TMath::Tan(alpha)));
  Double_t yt = xt / TMath::Tan(fTheta), xp = TMath::Sqrt(xt*xt + yt*yt);
  Double_t x  = xl + xp;
  fOK.Set(x, y);
  if(fDebug>1) printf(" yl %9.3f | xl %9.3f | xp %9.3f \n", yl, xl, xp);

  // have to define A and B; 
  Double_t yCprev = fgr + fga*TMath::Cos(fTheta);
  Double_t xCprev = (yCprev - B) / A;
  Double_t xA     = xCprev + fga*TMath::Sin(fTheta), yA = fgr;

  fThetaA = fTheta - fgangle/2.;
  fA = TMath::Tan(fThetaA); // !!
  fB = yA - fA*xA;

  DefineAllStaff();
}

//_____________________________________________________________________________
void IlcJetDummyShishKebabTrd1Module::DefineAllStaff()
{
//
// Standard definitions
  DefineName(fTheta);
  // Centers of module - 2X2 case
  Double_t kk1 = (fga+fga2)/(2.*4.); // kk1=kk2 

  Double_t xk1 = fOK.X() - kk1*TMath::Sin(fTheta);
  Double_t yk1 = fOK.Y() + kk1*TMath::Cos(fTheta) - fgr;
  fOK1.Set(xk1,yk1);

  Double_t xk2 = fOK.X() + kk1*TMath::Sin(fTheta);
  Double_t yk2 = fOK.Y() - kk1*TMath::Cos(fTheta) - fgr;
  fOK2.Set(xk2,yk2);

  // Centers of module - 3X3 case; Nov 9,2006
  fOK3X3[1].Set(fOK.X(), fOK.Y()-fgr); // coincide with module center

  kk1 = ((fga+fga2)/4. + fga/6.)/2.; 

  xk1 = fOK.X() - kk1*TMath::Sin(fTheta);
  yk1 = fOK.Y() + kk1*TMath::Cos(fTheta) - fgr;
  fOK3X3[0].Set(xk1,yk1);

  xk2 = fOK.X() + kk1*TMath::Sin(fTheta);
  yk2 = fOK.Y() - kk1*TMath::Cos(fTheta) - fgr;
  fOK3X3[2].Set(xk2,yk2);

  // May 15, 2006; position of cell face of cells 
  fOB.Set(fOK.X()-fgb/2.*TMath::Cos(fTheta),  fOK.Y()-fgb/2.*TMath::Sin(fTheta)-fgr);
  fOB1.Set(fOB.X()-fga/4.*TMath::Sin(fTheta), fOB.Y()+fga/4.*TMath::Cos(fTheta));
  fOB2.Set(fOB.X()+fga/4.*TMath::Sin(fTheta), fOB.Y()-fga/4.*TMath::Cos(fTheta));

}

//_____________________________________________________________________________
void IlcJetDummyShishKebabTrd1Module::DefineFirstModule()
{
  // Define first module
  fOK.Set(fga2/2., fgr + fgb/2.); // position the center of module vs o

  // parameters of right line : y = A*z + B in system where zero point is IP.
  fThetaA = fTheta - fgangle/2.;
  fA      = TMath::Tan(fThetaA);
  Double_t xA = fga/2. + fga2/2., yA = fgr;
  fB = yA - fA*xA;

  TObject::SetUniqueID(1); //

  DefineAllStaff();
}

//_____________________________________________________________________________
void IlcJetDummyShishKebabTrd1Module::DefineName(Double_t theta)
{
  // Define name of object
  SetName(Form("%2i(%5.2f)", TObject::GetUniqueID(), theta*TMath::RadToDeg()));
}

//_____________________________________________________________________________
Bool_t IlcJetDummyShishKebabTrd1Module::GetParameters()
{
 // Get needing module parameters from EMCAL geometry
  if(!fgGeometry) fgGeometry = IlcJetDummyGeo::GetInstance();
  TString sn(fgGeometry->GetName()); // 2-Feb-05
  sn.ToUpper();
  if(!fgGeometry) {
    Warning("GetParameters()"," No geometry ");
    return kFALSE; 
  }

  fga        = (Double_t)fgGeometry->GetEtaModuleSize();
  fgb        = (Double_t)fgGeometry->GetLongModuleSize();
  fgangle    = Double_t(fgGeometry->GetTrd1Angle())*TMath::DegToRad();
  fgtanBetta = TMath::Tan(fgangle/2.);
  fgr        = (Double_t)fgGeometry->GetIPDistance();

  if(!sn.Contains("TRD2")) fgr += fgGeometry->GetSteelFrontThickness();

  fga2       = Double_t(fgGeometry->Get2Trd1Dx2());
  //PH  PrintShish(0);
  return kTRUE;
}

// service methods
//_____________________________________________________________________________
void IlcJetDummyShishKebabTrd1Module::PrintShish(Int_t pri) const
{
  // service method
  if(pri>=0) {
    printf("PrintShish() \n a %7.3f:%7.3f | b %7.2f | r %7.2f \n TRD1 angle %7.6f(%5.2f) | tanBetta %7.6f", 
    fga, fga2, fgb, fgr, fgangle, fgangle*TMath::RadToDeg(), fgtanBetta);
    printf(" fTheta %f : %5.2f : cos(theta) %f\n", 
    fTheta, GetThetaInDegree(),TMath::Cos(fTheta)); 
    if(pri>=1) {
      printf(" %i |%s| theta %f :  fOK.Phi = %f(%5.2f)\n", 
      GetUniqueID(), GetName(), fTheta, fOK.Phi(),fOK.Phi()*TMath::RadToDeg());
      printf(" A %f B %f | fThetaA %7.6f(%5.2f)\n", fA,fB, fThetaA,fThetaA*TMath::RadToDeg());
      printf(" fOK  : X %9.4f: Y %9.4f : eta  %5.3f\n",  fOK.X(), fOK.Y(), GetEtaOfCenterOfModule());
      printf(" fOK1 : X %9.4f: Y %9.4f :   (local, ieta=2)\n", fOK1.X(), fOK1.Y());
      printf(" fOK2 : X %9.4f: Y %9.4f :   (local, ieta=1)\n\n", fOK2.X(), fOK2.Y());
      printf(" fOB  : X %9.4f: Y %9.4f \n", fOB.X(), fOB.Y());
      printf(" fOB1 : X %9.4f: Y %9.4f (local, ieta=2)\n", fOB1.X(), fOB1.Y());
      printf(" fOB2 : X %9.4f: Y %9.4f (local, ieta=1)\n", fOB2.X(), fOB2.Y());
      // 3X3 
      printf(" 3X3 \n");
      for(Int_t ieta=0; ieta<3; ieta++) {
        printf(" fOK3X3[%i] : X %9.4f: Y %9.4f (local) \n", ieta, fOK3X3[ieta].X(), fOK3X3[ieta].Y());
      }
      //      fOK.Dump();
      GetMaxEtaOfModule(pri);
    }
  }
}

//_____________________________________________________________________________
Double_t  IlcJetDummyShishKebabTrd1Module::GetThetaInDegree() const 
{
  return fTheta*TMath::RadToDeg();
}

//_____________________________________________________________________________
Double_t  IlcJetDummyShishKebabTrd1Module::GetEtaOfCenterOfModule() const 
{ 
  return -TMath::Log(TMath::Tan(fOK.Phi()/2.));
}

//_____________________________________________________________________________
Double_t  IlcJetDummyShishKebabTrd1Module::GetMaxEtaOfModule(Int_t pri) const 
{ 
  // Right bottom point of module
  Double_t xBottom     = (fgr - fB) / fA;
  Double_t thetaBottom = TMath::ATan2(fgr, xBottom);
  Double_t etaBottom   = ThetaToEta(thetaBottom);
  // Right top point of module
  Double_t l = fgb / TMath::Cos(fgangle/2.); // length of lateral module side
  Double_t xTop = xBottom + l*TMath::Cos(TMath::ATan(fA));
  Double_t yTop = fA*xTop + fB;
  Double_t thetaTop = TMath::ATan2(yTop, xTop);
  Double_t etaTop   = ThetaToEta(thetaTop);

  if(pri) { 
    printf(" Right bottom point of module : eta %5.4f : theta %6.4f (%6.2f) \n", 
    etaBottom, thetaBottom, thetaBottom * TMath::RadToDeg());
    printf(" Right    top point of module : eta %5.4f : theta %6.4f (%6.2f) \n", 
    etaTop, thetaTop, thetaTop * TMath::RadToDeg());
  }
  return etaBottom>etaTop ? etaBottom : etaTop;
}
