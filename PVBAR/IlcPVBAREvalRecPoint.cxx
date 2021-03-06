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

/* $Id: IlcPVBAREvalRecPoint.cxx 47119 2011-02-04 15:35:48Z policheh $ */

//*-- Author: Boris Polichtchouk, IHEP
//////////////////////////////////////////////////////////////////////////////
// Reconstructed point operations for the Clusterization class for ORKA reconstruction.
// Performs clusterization (collects neighbouring active cells)
// It differs from IlcPVBARClusterizerv1 in neighbour definition only

// ---- ROOT system ---
#include <TMath.h>
#include <TDirectory.h>
#include <TBranch.h>
#include <TTree.h>
#include <TROOT.h>
#include <TFolder.h>

// --- IlcRoot header files ---
#include "IlcLog.h"
#include "IlcConfig.h"
#include "IlcPVBARDigit.h" 
#include "IlcPVBARClusterizer.h"
#include "IlcPVBAREvalRecPoint.h"
#include "IlcRun.h"
#include "IlcPVBARLoader.h"
#include "IlcPVBARRecCpvManager.h"
#include "IlcPVBARRecEmcManager.h"
#include "IlcPVBARDigitizer.h"
#include "IlcPVBARGeometry.h" 

// --- Standard library ---


ClassImp(IlcPVBAREvalRecPoint)

IlcPVBAREvalRecPoint::IlcPVBAREvalRecPoint() :
  fIsEmc(kFALSE),
  fIsCpv(kTRUE),
  fParent(-333),
  fChi2Dof(-111),
  fEventFolderName(IlcConfig::GetDefaultEventFolderName())
{
  // default ctor
}

IlcPVBAREvalRecPoint::IlcPVBAREvalRecPoint(Bool_t cpv, IlcPVBAREvalRecPoint* parent) : 
  fIsEmc(kFALSE),
  fIsCpv(kFALSE),
  fParent(-333),
  fChi2Dof(-111),
  fEventFolderName("")
{
  // ctor
  fParent=-333;
  fChi2Dof=-111;

//    fParent=parent;
  TObjArray* wPool = (TObjArray*)GetWorkingPool();
  if(!wPool) {
    Fatal("ctor", "Couldn't find working pool") ; 
  }

  if(wPool)
    fParent = wPool->IndexOf((TObject*)parent);

  fChi2Dof = parent->Chi2Dof();

  if(cpv) {
    fIsEmc = kFALSE;
    fIsCpv = kTRUE;
  }
  else {
    fIsEmc = kTRUE;
    fIsCpv = kFALSE;
  }

  // Add itself to working pool
  AddToWorkingPool((TObject*)this);
  
}

IlcPVBAREvalRecPoint::IlcPVBAREvalRecPoint(Int_t i, Bool_t cpv) : 
  fIsEmc(kFALSE),
  fIsCpv(kFALSE),
  fParent(-333),
  fChi2Dof(-111),
  fEventFolderName(IlcConfig::GetDefaultEventFolderName())
{
  // ctor
  IlcPVBAREmcRecPoint* rp=0;

  IlcPVBARLoader* fLoader = IlcPVBARLoader::GetPVBARLoader(fEventFolderName);

  if(cpv) {
    rp = (IlcPVBARCpvRecPoint*)fLoader->CpvRecPoints()->At(i);
    fIsEmc = kFALSE;
    fIsCpv = kTRUE;
  }
  else {
    rp = (IlcPVBAREmcRecPoint*)fLoader->EmcRecPoints()->At(i);
    fIsEmc = kTRUE;
    fIsCpv = kFALSE;
  }

  Int_t* digits = rp->GetDigitsList();
  Float_t* energies = rp->GetEnergiesList();
  Int_t nDigits = rp->GetMultiplicity();
  
  for(Int_t iDigit=0; iDigit<nDigits; iDigit++) {
    IlcPVBARDigit* digit = (IlcPVBARDigit*)fLoader->Digits()->At( digits[iDigit] );
    Float_t eDigit = energies[iDigit];
    this->AddDigit(*digit,eDigit);
  }

  TVector3 locpos;
  rp->GetLocalPosition(locpos);
  fLocPos = locpos; 

  // Add itself to working pool
  AddToWorkingPool((TObject*)this);
   
}

IlcPVBARClusterizer* IlcPVBAREvalRecPoint::GetClusterizer()
{
  // returns clusterizer task
  TFolder* ilcF = (TFolder*)gROOT->FindObjectAny("YSIlc");
  TFolder* wPoolF = (TFolder*)ilcF->FindObject("WhiteBoard/RecPoints/PVBAR/SmP");
  IlcPVBARClusterizer* clu = (IlcPVBARClusterizer*)wPoolF->FindObject("PVBAR:clu-v1");
  if(!clu) { 
    Fatal("GetClusterizer", "Couldn't find Clusterizer") ; 
  }

  return clu;
}

Bool_t IlcPVBAREvalRecPoint::TooClose(IlcPVBARRecPoint* pt) const 
{
  // check if a rec.point pt is too close to this one
  TVector3 herPos;
  TVector3 myPos;
  pt->GetLocalPosition(herPos);
  this->GetLocalPosition(myPos);
  Float_t dx = herPos.X() - myPos.X();
  Float_t dz = herPos.Z() - myPos.Z();
  Float_t dr = TMath::Sqrt(dx*dx + dz*dz);

  if(dr<GetReconstructionManager()->MergeGammasMinDistanceCut())
    return kTRUE;
  else
    return kFALSE;

}

Bool_t IlcPVBAREvalRecPoint::NeedToSplit() const 
{
  // rec.point needs to be split
  return kFALSE;
}

void IlcPVBAREvalRecPoint::DeleteParent()
{
  // delete parent
  fParent=-333;
}

void IlcPVBAREvalRecPoint::UpdateWorkingPool()
{
  // update pool of rec.points
  Int_t i; //loop variable
  
  for(i=0; i<this->InWorkingPool(); i++) {
     IlcPVBAREvalRecPoint* parent = (IlcPVBAREvalRecPoint*)GetFromWorkingPool(i);
     TObjArray children;
     Int_t nChild = parent->HasChild(children);
     for(Int_t iChild=0; iChild<nChild; iChild++)
       {
	 ((IlcPVBAREvalRecPoint*)children.At(iChild))->DeleteParent();
       }

     if(nChild) {
       RemoveFromWorkingPool(parent);
       delete parent;
     }

  }

  for(i=0; i<this->InWorkingPool(); i++) {
    IlcPVBAREvalRecPoint* weak = (IlcPVBAREvalRecPoint*)GetFromWorkingPool(i);
    if (weak->KillWeakPoint()) delete weak;
  }

  for(i=0; i<this->InWorkingPool(); i++) {
    IlcPVBAREvalRecPoint* close = (IlcPVBAREvalRecPoint*)GetFromWorkingPool(i);
    close->MergeClosePoint();
  }

  for(i=0; i<this->InWorkingPool(); i++)
    ((IlcPVBAREvalRecPoint*)IlcPVBAREvalRecPoint::GetFromWorkingPool(i))->SetIndexInList(i);

}

Int_t IlcPVBAREvalRecPoint::HasChild(TObjArray& children)
{
  // returns the number of children
  for(Int_t iChild=0; iChild<InWorkingPool(); iChild++)
    {
      IlcPVBAREvalRecPoint* child = (IlcPVBAREvalRecPoint*)GetFromWorkingPool(iChild);
      TObject* parent = (TObject*)child->Parent();
      TObject* me = (TObject*)this;
      if(parent==me) children.Add(child);
    }

  return children.GetEntries();
}

void IlcPVBAREvalRecPoint::Init()
{
  // initialization
  IlcPVBARClusterizer* clusterizer = GetClusterizer();
  if(!clusterizer) {
    IlcFatal("Cannot get clusterizer") ;
  }

  TClonesArray* digits = IlcPVBARLoader::GetPVBARLoader(fEventFolderName)->Digits();
  Float_t logWeight=0;  

  if(this->IsEmc()) {
    logWeight = clusterizer->GetEmcLogWeight();
  }
  else {
    logWeight = clusterizer->GetCpvLogWeight();
  }
  
  TVector3 vtx(0.,0.,0.) ;
  TVector3 inc(0.,0.,0.) ;
  EvalLocalPosition(logWeight,vtx,digits,inc); // evaluate initial position
}


void IlcPVBAREvalRecPoint::MakeJob()
{
  // Reconstruction algoritm implementation.

  IlcPVBARRecManager* recMng = GetReconstructionManager();

  Init();

  UnfoldLocalMaxima();
  
  TObjArray children;
  Int_t nChild = HasChild(children);
  
  if(!nChild) {
    EvaluatePosition();
    if(Chi2Dof()>recMng->OneGamChisqCut()) SplitMergedShowers();
  }

  for(Int_t iChild=0; iChild<nChild; iChild++) {
    IlcPVBAREvalRecPoint* child = (IlcPVBAREvalRecPoint*)children.At(iChild);
    child->EvaluatePosition();

    if(child->Chi2Dof()>recMng->OneGamChisqCut())
      child->SplitMergedShowers();
  }  

} 

void IlcPVBAREvalRecPoint::InitTwoGam(Float_t* gamma1, Float_t* gamma2)
{
  //Compute start values for two gamma fit algorithm.
  // gamma1[0], gamma1[1], gamma1[2] are Energy,x,z of the reconstructed "gammas".


  TVector3 lpos; // start point choosing.
  GetLocalPosition(lpos);
  Float_t xx = lpos.Z();
  Float_t yy = lpos.X();
  Float_t e = GetEnergy();

  IlcInfo(Form("(x,z,e)[old] = (%f, %f, %f)", yy, xx, e)) ;

//    xx = XY(xx/e);
//    yy = XY(yy/e);


  Float_t eDigit ;
  IlcPVBARDigit * digit ;
  Int_t nDigits = GetMultiplicity();  
  Int_t * digits = GetDigitsList();
  Float_t * energies = GetEnergiesList();

  Float_t ix ;
  Float_t iy ;
  Int_t relid[4] ; 

  Float_t exx = 0;
  Float_t eyy = 0;
  Float_t exy = 0;
  Float_t sqr;
  Float_t cos2fi = 1.;

  IlcPVBARLoader* fLoader = IlcPVBARLoader::GetPVBARLoader(fEventFolderName);
  IlcPVBARGeometry * PVBARgeom =  IlcPVBARGeometry::GetInstance() ;

  Int_t iDigit; //loop variable

  for(iDigit = 0 ; iDigit < nDigits ; iDigit ++)
    {
      digit = (IlcPVBARDigit*)fLoader->Digits()->At( digits[iDigit] ); 
      eDigit = energies[iDigit];
      PVBARgeom->AbsToRelNumbering(digit->GetId(), relid) ;
      PVBARgeom->RelPosInModule(relid, iy, ix);
    
      Float_t dx =  ix - xx;
      Float_t dy =  iy - yy;
      exx += eDigit*dx*dx;
      eyy += eDigit*dy*dy;
      exy += eDigit*dx*dy;
      
    }

  sqr = TMath::Sqrt(4.*exy*exy + (exx-eyy)*(exx-eyy));
  Float_t euu = (exx+eyy+sqr)/2.;
  if(sqr>1.e-10) cos2fi = (exx - eyy)/sqr;
  Float_t cosfi = TMath::Sqrt((1.+cos2fi)/2.);
  Float_t sinfi = TMath::Sqrt((1.-cos2fi)/2.);
  if(exy<0) sinfi = -sinfi;

  Float_t eu3 = 0;
  for(iDigit = 0 ; iDigit < nDigits ; iDigit ++)
    {
      digit = (IlcPVBARDigit*)fLoader->Digits()->At( digits[iDigit] ); 
      eDigit = energies[iDigit];
      PVBARgeom->AbsToRelNumbering(digit->GetId(), relid) ;
      PVBARgeom->RelPosInModule(relid, iy, ix);
    
      Float_t dx =  ix - xx;
      Float_t dy =  iy - yy;
      Float_t du = dx*cosfi + dy*sinfi;
      eu3 += eDigit*du*du*du;
    }
  
  Float_t c = e*eu3*eu3/(euu*euu*euu)/2.;
  Float_t sign = 1.;
  if(eu3<0) sign = -1.;
  Float_t r = 1.+ c + sign*TMath::Sqrt(2.*c + c*c);
  Float_t u = 0;
  if(TMath::Abs(r-1.)>0.1) u = eu3/euu*(r+1.)/(r-1.);
  if(TMath::Abs(r-1.)<0.1) u = TMath::Sqrt(sqr/e/r)*(1.+r);
  
  Float_t e2c = e/(1.+r);
  Float_t e1c = e-e2c;
  Float_t u1 = -u/(1.+r);
  Float_t u2 = u+u1;
  
  Float_t x1c = xx+u1*cosfi;
  Float_t y1c = yy+u1*sinfi;
  Float_t x2c = xx+u2*cosfi;
  Float_t y2c = yy+u2*sinfi;

//    printf("e1c -> %f\n",e1c);
//    printf("x1c -> %f\n",x1c);
//    printf("y1c -> %f\n",y1c);
//    printf("e2c -> %f\n",e2c);
//    printf("x2c -> %f\n",x2c);
//    printf("y2c -> %f\n",y2c);

  gamma1[0] = e1c;
  gamma1[1] = y1c;
  gamma1[2] = x1c;

  gamma2[0] = e2c;
  gamma2[1] = y2c;
  gamma2[2] = x2c;
  
}

void IlcPVBAREvalRecPoint::TwoGam(Float_t* gamma1, Float_t* gamma2)
{
  //Fitting algorithm for the two very closed gammas 
  //that merged into the cluster with one maximum.
  // Starting points gamma1 and gamma2 must be provided before ( see InitTwoGam)
  //Set chisquare of the fit.


  Float_t st0 = GetReconstructionManager()->TwoGamInitialStep();
  Float_t chmin = GetReconstructionManager()->TwoGamChisqMin();
  Float_t emin = GetReconstructionManager()->TwoGamEmin();
  Float_t stpmin = GetReconstructionManager()->TwoGamStepMin();
  Int_t nIter = GetReconstructionManager()->TwoGamNumOfIterations(); // Number of iterations.

  Float_t chisq = 100.; //Initial chisquare.

  Int_t nadc = GetMultiplicity();
  if(nadc<3)
      fChi2Dof= -111.;
  
  Int_t dof = nadc - 5;
  if(dof<1) dof=1;
  Float_t chstop = chmin*dof;

  Float_t ch = 1.e+20;
  Float_t st = st0;
  Float_t grx1 = 0.;
  Float_t gry1 = 0.;
  Float_t grx2 = 0.;
  Float_t gry2 = 0.;
  Float_t gre = 0.;
  Float_t gr = 1.;
  Float_t ee1=0,xx1=0,yy1=0,ee2=0,xx2=0,yy2=0,cosi;

  Float_t e1c = gamma1[0];
  Float_t y1c = gamma1[1];
  Float_t x1c = gamma1[2];

  Float_t e2c = gamma2[0];
  Float_t y2c = gamma2[1];
  Float_t x2c = gamma2[2];

  Float_t e = GetEnergy();

  Float_t eDigit ;
  IlcPVBARDigit * digit ;
  Int_t nDigits = GetMultiplicity();  
  Int_t * digits = GetDigitsList();
  Float_t * energies = GetEnergiesList();

  Float_t ix ;
  Float_t iy ;
  Int_t relid[4] ; 

  IlcPVBARLoader* fLoader = IlcPVBARLoader::GetPVBARLoader(fEventFolderName);
  IlcPVBARGeometry * PVBARgeom =  IlcPVBARGeometry::GetInstance() ;

  for(Int_t iter=0; iter<nIter; iter++)
    {
      Float_t chisqc = 0.;
      Float_t grx1c = 0.;
      Float_t gry1c = 0.;
      Float_t grx2c = 0.;
      Float_t gry2c = 0.;
      Float_t grec = 0.;

      for(Int_t iDigit = 0 ; iDigit < nDigits ; iDigit ++)
	{
	  digit = (IlcPVBARDigit*)fLoader->Digits()->At( digits[iDigit] ); 
	  eDigit = energies[iDigit];
	  PVBARgeom->AbsToRelNumbering(digit->GetId(), relid) ;
	  PVBARgeom->RelPosInModule(relid, iy, ix);
	  
	  Float_t a1,gx1,gy1;
	  Float_t a2,gx2,gy2;
	  
	  Float_t dx1 =  x1c - ix;
	  Float_t dy1 =  y1c - iy;

	  GetReconstructionManager()->AG(e1c,dx1,dy1,a1,gx1,gy1);

	  Float_t dx2 =  x2c - ix;
	  Float_t dy2 =  y2c - iy;

	  GetReconstructionManager()->AG(e2c,dx2,dy2,a2,gx2,gy2);
      
	  Float_t a = a1+a2;
//  	  Float_t D = Const*a*(1. - a/e);
//  	  if(D<0) D=0;

//  //    	  D = 9.; // ????
	  
//  	  Float_t da = a - eDigit;
//  	  chisqc += da*da/D;
//  	  Float_t dd = da/D;
//  	  dd = dd*(2.-dd*Const*(1.-2.*a/e));

	  Float_t dd;
	  chisqc += GetReconstructionManager()->TwoGamChi2(a,eDigit,e,dd);
	  
	  grx1c += gx1*dd;
	  gry1c += gy1*dd;
	  grx2c += gx2*dd;
	  gry2c += gy2*dd;
	  grec += (a1/e1c - a2/e2c)*e*dd;

	}

      Float_t grc = TMath::Sqrt(grx1c*grx1c + gry1c*gry1c + grx2c*grx2c + gry2c*gry2c);
      if(grc<1.e-10) grc=1.e-10;

      Float_t sc = 1. + chisqc/ch;
      st = st/sc; 

      if(chisqc>ch) 
	goto loop20;
      cosi = (grx1*grx1c + gry1*gry1c + grx2*grx2c + gry2*gry2c + gre*grec)/gr/grc;
      st = st*sc/(1.4 - cosi);

      ee1 = e1c;
      xx1 = x1c;
      yy1 = y1c;
      ee2 = e2c;
      xx2 = x2c;
      yy2 = y2c;

      ch = chisqc;

      if(ch<chstop)
	goto loop101;

      grx1 = grx1c;
      gry1 = gry1c;
      grx2 = grx2c;
      gry2 = gry2c;
      gre = grec;
      gr = grc;
 
    loop20: ;
      Float_t step = st*gr;

      IlcInfo(Form("Iteration %d dof %d chisq/dof %f chstop/dof %f step %f stpmin %f",
	   iter, dof, ch/dof, chstop/dof, step, stpmin)) ;

      
      if(step<stpmin) 
	goto loop101;

      Float_t de = st*gre*e;
      e1c = ee1 - de;
      e2c = ee2 + de;
      
      if( (e1c>emin) && (e2c>emin) )
	goto loop25;
      st = st/2.;
      goto loop20;
      
    loop25: ;
      x1c = xx1 - st*grx1;
      y1c = yy1 - st*gry1;
      x2c = xx2 - st*grx2;
      y2c = yy2 - st*gry2;


    }

 loop101: 

//    if(ch>chisq*(nadc-2)-delch)
//      return ch/dof;  
  
  chisq = ch/dof;
  gamma1[0] = ee1;
  gamma1[1] = yy1;
  gamma1[2] = xx1;

  gamma2[0] = ee2;
  gamma2[1] = yy2;
  gamma2[2] = xx2;

  Float_t x1New = yy1;
  Float_t z1New = xx1;
  Float_t e1New = ee1;
  Float_t x2New = yy2;
  Float_t z2New = xx2;
  Float_t e2New = ee2;

  TString message ; 
  message  = "     (x,z,e)[1 fit] = (%f, %f, %f)\n" ;  
  message  = "     (x,z,e)[2 fit] = (%f, %f, %f)\n" ;  
  IlcInfo(Form(message.Data(), 
       x1New, z1New, e1New, 
       x2New, z2New, e2New)) ;

  fChi2Dof = chisq;

}

void IlcPVBAREvalRecPoint::UnfoldTwoMergedPoints(Float_t* gamma1, Float_t* gamma2)
{
  //Unfold TWO merged rec. points in the case when cluster has only one maximum, 
  //but it's fitting to the one gamma shower is too bad.
  // Use TwoGam() to estimate the positions and energies of merged points.
 
  
  Int_t nMax = 2;
  Float_t* gamma;

  Int_t* digits = GetDigitsList();
  Int_t nDigits = GetMultiplicity();
  Float_t* energies = GetEnergiesList();
  Float_t* eFit = new Float_t[nDigits];

  IlcPVBARLoader* fLoader = IlcPVBARLoader::GetPVBARLoader(fEventFolderName);
  IlcPVBARGeometry * PVBARgeom =  IlcPVBARGeometry::GetInstance() ;

  for(Int_t iDigit=0; iDigit<nDigits; iDigit++)
    {
      IlcPVBARDigit* digit = (IlcPVBARDigit*)fLoader->Digits()->At( digits[iDigit] ); 
      Int_t relid[4] ;
      PVBARgeom->AbsToRelNumbering(digit->GetId(), relid) ;
      Float_t x,z;     
      PVBARgeom->RelPosInModule(relid, x, z);

      Float_t gain = 0.;
      for(Int_t iMax=0; iMax<nMax; iMax++)
	{
	  if(iMax==0)
	    gamma = gamma1;
	  else
	    gamma = gamma2;

	  Float_t eMax = gamma[0];
	  Float_t xMax = gamma[1];
	  Float_t zMax = gamma[2];

	  Float_t dx = xMax - x;
	  Float_t dz = zMax - z;
	  Float_t amp,gx,gy;
	  GetReconstructionManager()->AG(eMax,dz,dx,amp,gx,gy); 
	  gain += amp;
	}
 
      eFit[iDigit] = gain;
    }


  for( Int_t iMax=0; iMax<nMax; iMax++)
    {      
      if(iMax==0)
	gamma = gamma1;
      else
	gamma = gamma2;

      Float_t eMax = gamma[0];
      Float_t xMax = gamma[1];
      Float_t zMax = gamma[2];

      IlcPVBAREvalRecPoint* newRP = new IlcPVBAREvalRecPoint(IsCPV(),this);
      TVector3 newpos(xMax,0,zMax);
      newRP->SetLocalPosition(newpos);

      for( Int_t iDigit = 0 ; iDigit < nDigits ; iDigit ++)
	{
	  IlcPVBARDigit* digit = (IlcPVBARDigit*)fLoader->Digits()->At( digits[iDigit] ); 
	  Float_t eDigit = energies[iDigit];
	  Int_t relid[4] ;
	  PVBARgeom->AbsToRelNumbering(digit->GetId(), relid) ;
	  Float_t ix,iz;
	  PVBARgeom->RelPosInModule(relid, ix, iz);
	  
	  Float_t dx = xMax - ix;
	  Float_t dz = zMax - iz;
	  Float_t singleShowerGain,gxMax,gyMax;
	  GetReconstructionManager()->AG(eMax,dz,dx,singleShowerGain,gxMax,gyMax);
	  Float_t totalGain = eFit[iDigit];
	  Float_t ratio = singleShowerGain/totalGain; 
	  eDigit = eDigit*ratio;
	  newRP->AddDigit(*digit,eDigit);
	}
      IlcInfo(Form("======= Split: daughter rec point %d =================", 
		   iMax)) ;
      newRP->Print();

    }

  delete[] eFit;
  

}


void IlcPVBAREvalRecPoint::EvaluatePosition() 
{
  // One gamma fit algorithm.
  // Set chisq/dof of the fit.
  // gamma1[0], gamma1[1], gamma1[2] are Energy,x,z of the reconstructed gamma, respectively.

  Int_t nDigits = GetMultiplicity();
  if(nDigits<2)
    return;

  Int_t nIter = GetReconstructionManager()->OneGamNumOfIterations(); // number of iterations
  Float_t st0 = GetReconstructionManager()->OneGamInitialStep(); // initial step
//    const Float_t stpMin = 0.005;
  Float_t stpMin = GetReconstructionManager()->OneGamStepMin();
  Float_t chmin =  GetReconstructionManager()->OneGamChisqMin();
 
  TVector3 locpos;
  IlcPVBARDigit* digit;
  Float_t eDigit;
  Int_t relid[4] ; 
  Float_t ix, iy;

  GetLocalPosition(locpos);
  Float_t e = GetEnergy();
  Float_t xc = locpos.Z();
  Float_t yc = locpos.X();
  Float_t dx,dy,gx,gy,grxc,gryc;
  Float_t st = st0;
  Float_t chisq = 1.e+20;
  Float_t gr = 1.;
  Float_t grx = 0.;
  Float_t gry = 0.;
  Int_t dof = GetMultiplicity() - 2;
  if(dof<1)
    dof = 1;
  Float_t chstop = chmin*dof;
  Float_t cosi,x1=0,y1=0;
  Float_t chisqc;

  IlcPVBARLoader* fLoader = IlcPVBARLoader::GetPVBARLoader(fEventFolderName);
  IlcPVBARGeometry * PVBARgeom =  IlcPVBARGeometry::GetInstance() ;

  for(Int_t iter=0; iter<nIter; iter++)
    {
 
      chisqc = 0.;
      grxc = 0.;
      gryc = 0.;
      for(Int_t iDigit = 0 ; iDigit < nDigits ; iDigit ++)
	{

	  Float_t* energies = GetEnergiesList();
	  Int_t* digits = GetDigitsList();
	  digit = (IlcPVBARDigit*)fLoader->Digits()->At( digits[iDigit] );
	  eDigit = energies[iDigit];
	  PVBARgeom->AbsToRelNumbering(digit->GetId(), relid) ;
	  PVBARgeom->RelPosInModule(relid, iy, ix);
      
	  dx =  xc - ix;
	  dy =  yc - iy;

	  if(!dx) dx=dy;
	  if(!dy) dy=dx;
	  	  
	  Float_t a;
	  GetReconstructionManager()->AG(e,dx,dy,a,gx,gy);
	  Float_t dd;
	  IlcInfo(Form("  (ix iy  xc yc  dx dy)   %f %f %f %f %f %f", 
		       ix, iy, xc, yc, dx, dy)) ;
	  Float_t chi2dg = GetReconstructionManager()->OneGamChi2(a,eDigit,e,dd);

  	  // Exclude digit with too large chisquare.
	  if(chi2dg > 10) {  continue; }

	  chisqc += chi2dg;
	  grxc += gx*dd;
	  gryc += gy*dd;

	}

      Float_t grc = TMath::Sqrt(grxc*grxc + gryc*gryc);
      if(grc<1.e-10) grc=1.e-10;
      Float_t sc = 1. + chisqc/chisq;
       IlcInfo(Form(" chisq: %f", chisq)) ;
      st = st/sc;
      if(chisqc>chisq)
	goto loop20;
      cosi = (grx*grxc + gry*gryc)/gr/grc;
      st = st*sc/(1.4 - cosi);
      x1 = xc;
      y1 = yc;
      grx = grxc;
      gry = gryc;
      gr = grc;
      chisq = chisqc;

      if(chisq<chstop)
	goto loop101;
  
    loop20: ;
      Float_t step = st*gr;

      IlcInfo(Form(" Iteration %d dof %d chisq/dof %f chstop/dof %f step %f stpMin %f",
	   iter, dof, chisq/dof, chstop/dof, step, stpMin)) ;
	

      if(step<stpMin)
	goto loop101;

      if(step>1.)
	st = 1./gr;
      xc = x1 - st*grx;
      yc = y1 - st*gry;
    }


 loop101:
  chisq = chisq/dof;
//    if(chisq>Chsqcut)
//      {
//  //        TwoGam();
//      }

  Float_t gamma1[3];
  gamma1[0] = e;
  gamma1[1] = y1;
  gamma1[2] = x1;

  TVector3 newpos(gamma1[1],0,gamma1[2]);
  //SetLocalPosition(newpos);
  
  fLocPos=newpos;
  fAmp=e;

//    TVector3 pos;
//    RP->GetLocalPosition(pos);

  
  
  fChi2Dof = chisq;

} 


Bool_t IlcPVBAREvalRecPoint::KillWeakPoint()
{
  // Remove this point from further procession
  // if it's energy is too small.
  
  Float_t thr0 = GetReconstructionManager()->KillGamMinEnergy();
  
  if(GetEnergy()<thr0) {
    IlcInfo(Form("+++++++ Killing this rec point ++++++++++")) ;
    RemoveFromWorkingPool(this);
    return kTRUE;
  }

  return kFALSE;
}


void IlcPVBAREvalRecPoint::SplitMergedShowers() 
{
  // Split two very close rec. points.

  if(GetMultiplicity()<2) 
    return; 

  Float_t gamma1[3];
  Float_t gamma2[3];

  InitTwoGam(gamma1,gamma2); // start points for fit
  TwoGam(gamma1,gamma2); // evaluate the positions and energies
  UnfoldTwoMergedPoints(gamma1,gamma2); // make the job
  
}


void IlcPVBAREvalRecPoint::MergeClosePoint() 
{
  // merge rec.points if they are too close
  IlcPVBARLoader* fLoader = IlcPVBARLoader::GetPVBARLoader(fEventFolderName);
//    IlcPVBARDigitizer* digitizer = fLoader->Digitizer();
//    Float_t fPedestal = digitizer->GetPedestal();  // YVK 30.09.2001
//    Float_t fSlope = digitizer->GetSlope();
  
  for (Int_t i=0;i<InWorkingPool(); i++)
    {
      TObject* obj = GetFromWorkingPool(i);
      if(!((TObject*)this)->IsEqual(obj))
	{
	  IlcPVBARRecPoint* rp = (IlcPVBARRecPoint*)obj;
	  if(GetPVBARMod() == rp->GetPVBARMod())
	    {
	      if(TooClose(rp))
		{
		  IlcInfo(Form("+++++++ Merging point 1: ++++++")) ;
		  this->Print();
		  IlcInfo(Form("+++++++ and point 2: ++++++++++")) ;
		  ((IlcPVBAREvalRecPoint*)rp)->Print();

		  //merge two rec. points
		  TVector3 lpos1;
		  TVector3 lpos2;
		  this->GetLocalPosition(lpos1);
		  rp->GetLocalPosition(lpos2);

		  Int_t* digits = rp->GetDigitsList();
		  Float_t dE = rp->GetEnergy()/(rp->GetEnergy()+this->GetEnergy());
		  Float_t newX = lpos1.X()*dE + lpos2.X()*(1.-dE); 
		  Float_t newZ = lpos1.Z()*dE + lpos2.Z()*(1.-dE);
		  Float_t newE = rp->GetEnergy()+this->GetEnergy();
		  Float_t* energies = ((IlcPVBAREmcRecPoint*)rp)->GetEnergiesList();

		  for(Int_t iDigit=0; iDigit<rp->GetDigitsMultiplicity(); iDigit++)
		    {
		      IlcPVBARDigit* digit = (IlcPVBARDigit*)fLoader->Digits()->At(digits[iDigit]);
		      Float_t eDigit = energies[iDigit];
		      this->AddDigit(*digit,eDigit);
		    }

		  TVector3 newpos(newX,0,newZ);
		  fLocPos = newpos;
		  fAmp = newE;
		  RemoveFromWorkingPool(rp);
		  delete rp;
		  
		  IlcInfo(Form("++++++ Resulting point: ++++++++")) ;
		  this->Print();

		  break;
		} 
	    }
	}
    }

}

Int_t IlcPVBAREvalRecPoint::UnfoldLocalMaxima() 
{
  // Make unfolding in the reconstruction point with several local maxima.
  // Return the number of local maxima.

  // if multiplicity less then 2 - nothing to unfold
  if(GetMultiplicity()<2) return 1; 

  IlcPVBARDigit * maxAt[1000];
  Float_t maxAtEnergy[1000];  
  Float_t locMaxCut, logWeight;
  Int_t relid[4] ; 
  Float_t xMax;
  Float_t zMax;

  IlcPVBARClusterizer* clusterizer = GetClusterizer();
  if(!clusterizer) {
    IlcFatal(Form("Cannot get clusterizer")) ;
  }

  if(this->IsEmc()) {
    locMaxCut = clusterizer->GetEmcLocalMaxCut();
    logWeight = clusterizer->GetEmcLogWeight();
  }
  else {
    locMaxCut = clusterizer->GetCpvLocalMaxCut();
    logWeight = clusterizer->GetCpvLogWeight();
  }

  IlcPVBARLoader* fLoader = IlcPVBARLoader::GetPVBARLoader(fEventFolderName);
  IlcPVBARGeometry * PVBARgeom =  IlcPVBARGeometry::GetInstance() ;
  TClonesArray* digits = fLoader->Digits();

  // if number of local maxima less then 2 - nothing to unfold
  Int_t nMax = GetNumberOfLocalMax(maxAt,maxAtEnergy,locMaxCut,digits); 
  if(nMax<2) return nMax;

  Int_t* digitsList = GetDigitsList();
  Int_t nDigits = GetMultiplicity();
  Float_t* energies = GetEnergiesList();
  Float_t* eFit = new Float_t[nDigits];

  Int_t iDigit; //loop variable

  for(iDigit=0; iDigit<nDigits; iDigit++)
    {
      eFit[iDigit] = 0.;
    }

  for(iDigit=0; iDigit<nDigits; iDigit++)
    {
      
      IlcPVBARDigit* digit = (IlcPVBARDigit*)fLoader->Digits()->At( digitsList[iDigit] );
      PVBARgeom->AbsToRelNumbering(digit->GetId(), relid) ;
      Float_t x,z;
      PVBARgeom->RelPosInModule(relid, x, z);

      for(Int_t iMax=0; iMax<nMax; iMax++)
	{
	  IlcPVBARDigit* digitMax = maxAt[iMax];
	  Float_t eMax = maxAtEnergy[iMax]; 
	  PVBARgeom->AbsToRelNumbering(digitMax->GetId(), relid) ;
	  PVBARgeom->RelPosInModule(relid, xMax, zMax);
	  Float_t dx = xMax - x;
	  Float_t dz = zMax - z;
	  Float_t amp,gx,gy;
	  GetReconstructionManager()->AG(eMax,dz,dx,amp,gx,gy); 
//        amp = amp + 0.5;
	  eFit[iDigit] += amp;
	}
    }


  for(Int_t iMax=0; iMax<nMax; iMax++) 
    {
      IlcPVBARDigit* digitMax = maxAt[iMax];
      PVBARgeom->AbsToRelNumbering(digitMax->GetId(), relid) ;
      PVBARgeom->RelPosInModule(relid, xMax, zMax);
      Float_t eMax = maxAtEnergy[iMax];

      IlcPVBAREvalRecPoint* newRP = new IlcPVBAREvalRecPoint(IsCPV(),this);    
      newRP->AddDigit(*digitMax,maxAtEnergy[iMax]);

      //Neighbous ( matrix 3x3 around the local maximum)
      for(iDigit=0; iDigit<nDigits; iDigit++)
	{     
	  IlcPVBARDigit* digit = (IlcPVBARDigit*)fLoader->Digits()->At( digitsList[iDigit] );
  	  Float_t eDigit = energies[iDigit];
	  PVBARgeom->AbsToRelNumbering(digit->GetId(), relid) ;
	  Float_t ix,iz;
	  PVBARgeom->RelPosInModule(relid, ix, iz);

  	  if(AreNeighbours(digitMax,digit))
  	    {
	      Float_t dx = xMax - ix;
	      Float_t dz = zMax - iz;
	      Float_t singleShowerGain,gxMax,gyMax;
	      GetReconstructionManager()->AG(eMax,dz,dx,singleShowerGain,gxMax,gyMax);
  	      Float_t totalGain = eFit[iDigit];
    	      Float_t ratio = singleShowerGain/totalGain; 
	      IlcInfo(Form(" ratio -> %f", ratio)) ;
  	      eDigit = eDigit*ratio;
	      newRP->AddDigit(*digit,eDigit);
  	    }
	}

      TVector3 vtx(0.,0.,0.) ;
      TVector3 inc(0.,0.,0.) ;
      newRP->EvalLocalPosition(logWeight,vtx,digits,inc);
      IlcInfo(Form("======= Unfold: daughter rec point %d =================", 
		   iMax)) ;
      newRP->Print();
    }

//    RemoveFromWorkingPool(this);

  delete[] eFit;

  return nMax;

}

void IlcPVBAREvalRecPoint::PrintPoint()
{
  // print rec.point to stdout
  IlcPVBARCpvRecPoint::Print();

  TVector3 lpos;
  GetLocalPosition(lpos);

  TString message ; 
  message  = "       Chi2/dof = %f" ;
  message += "       Local (x,z) = (%f, %f) in module %d" ; 
  IlcInfo(Form(message.Data(), Chi2Dof(), lpos.X(), lpos.Z(), GetPVBARMod())) ;

}

IlcPVBARRecManager* IlcPVBAREvalRecPoint::GetReconstructionManager() const
{
  // returns reconstruction manager
  TFolder* ilcF = (TFolder*)gROOT->FindObjectAny("YSIlc");
  TFolder* wPoolF = (TFolder*)ilcF->FindObject("WhiteBoard/RecPoints/PVBAR/SmP");
  IlcPVBARRecManager* recMng = (IlcPVBARRecManager*)wPoolF->FindObject("IlcPVBARRecManager");
  if(!recMng) { 
    IlcFatal(Form("Couldn't find Reconstruction Manager")) ;  
  }

  return recMng;
}


IlcPVBARRecPoint* IlcPVBAREvalRecPoint::Parent()
{
  // returns a parent
  if(fParent<0) return NULL;
  else
    return (IlcPVBARRecPoint*)GetFromWorkingPool(fParent);
//    return fParent;
}

Float_t IlcPVBAREvalRecPoint::Chi2Dof() const 
{
  // returns a chi^2 per degree of freedom
  return fChi2Dof;
}

const TObject* IlcPVBAREvalRecPoint::GetWorkingPool()
{
  // returns a pool of rec.points
  TFolder* ilcF = (TFolder*)gROOT->FindObjectAny("YSIlc");
  TFolder* wPoolF = (TFolder*)ilcF->FindObject("WhiteBoard/RecPoints/PVBAR/SmP");
  TObject* wPool = wPoolF->FindObject("SmartPoints");
  if(!wPool) { 
    IlcFatal(Form("Couldn't find Working Pool")) ;  
  }

  return wPool;
}

void IlcPVBAREvalRecPoint::AddToWorkingPool(TObject* obj)
{
  // add a rec.point to a pool
  ((TObjArray*)GetWorkingPool())->Add(obj);
}

TObject* IlcPVBAREvalRecPoint::GetFromWorkingPool(Int_t i)
{
  // return a rec.point from a pool at an index i
//    return fWorkingPool->At(i);
  return ((TObjArray*)GetWorkingPool())->At(i);
}

Int_t IlcPVBAREvalRecPoint::InWorkingPool()
{
  // return the number of rec.points in a pool
  return ((TObjArray*)GetWorkingPool())->GetEntries();
}

void IlcPVBAREvalRecPoint::RemoveFromWorkingPool(TObject* obj)
{
  // remove a rec.point from a pool
  ((TObjArray*)GetWorkingPool())->Remove(obj);
  ((TObjArray*)GetWorkingPool())->Compress();
}

void IlcPVBAREvalRecPoint::PrintWorkingPool()
{
  // print pool of rec.points to stdout
  ((TObjArray*)GetWorkingPool())->Print();
}
