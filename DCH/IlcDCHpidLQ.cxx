/**************************************************************************
 * Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 *                                                                        *
// Author: The ILC Off-line Project. 
 // Part of the code has been developed by Alice Off-line Project. 
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

/* $Id: IlcDCHpidLQ.cxx,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//   The DCH particle identification class                                   //
//                                                                           //
//   Its main purposes are:                                                  //
//      - Creation and bookkeeping of the propability distributions          //
//      - Assignment of a e/pi propability to a given track based on         //
//        the LQ method                                                      //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <math.h>

#include <TROOT.h>
#include <TH1.h>
#include <TObjArray.h>
#include <TTree.h>
#include <TFile.h>
#include <TParticle.h>

#include "IlcRun.h"
#include "IlcDCHpidLQ.h"
#include "IlcDCHcluster.h"
#include "IlcDCHtrack.h"
#include "IlcDCHtracker.h"
#include "IlcDCHgeometry.h"

ClassImp(IlcDCHpidLQ)

//_____________________________________________________________________________
IlcDCHpidLQ::IlcDCHpidLQ():IlcDCHpid()
{
  //
  // IlcDCHpidLQ default constructor
  // 

  fNMom   = 0;
  fMinMom = 0;
  fMaxMom = 0;
  fWidMom = 0;

  fNLh    = 0;
  fMinLh  = 0;
  fMaxLh  = 0;

  fHist   = NULL;

  fChargeMin   = 0;
  fNClusterMin = 0;

}

//_____________________________________________________________________________
IlcDCHpidLQ::IlcDCHpidLQ(const char* name, const char* title)
            :IlcDCHpid(name,title)
{
  //
  // IlcDCHpidLQ constructor
  //

  fNMom   = 0;
  fMinMom = 0;
  fMaxMom = 0;
  fWidMom = 0;

  fNLh    = 0;
  fMinLh  = 0;
  fMaxLh  = 0;

  fHist   = NULL;

  Init();

}

//_____________________________________________________________________________
IlcDCHpidLQ::IlcDCHpidLQ(const IlcDCHpidLQ &p):IlcDCHpid(p)
{
  //
  // IlcDCHpidLQ copy constructor
  //

  ((IlcDCHpidLQ &) p).Copy(*this);

}

//_____________________________________________________________________________
IlcDCHpidLQ::~IlcDCHpidLQ()
{
  //
  // IlcDCHpidLQ destructor
  //

  if (fHist) {
    fHist->Delete();
    delete fHist;
  }

}

//_____________________________________________________________________________
IlcDCHpidLQ &IlcDCHpidLQ::operator=(const IlcDCHpidLQ &p)
{
  //
  // Assignment operator
  //

  if (this != &p) ((IlcDCHpidLQ &) p).Copy(*this);
  return *this;

}

//_____________________________________________________________________________
void IlcDCHpidLQ::Copy(TObject &p) const
{
  //
  // Copy function
  //

  fHist->Copy(*((IlcDCHpidLQ &) p).fHist);

  ((IlcDCHpidLQ &) p).fNMom        = fNMom;
  ((IlcDCHpidLQ &) p).fMinMom      = fMinMom;
  ((IlcDCHpidLQ &) p).fMaxMom      = fMaxMom;
  ((IlcDCHpidLQ &) p).fWidMom      = fWidMom;
  ((IlcDCHpidLQ &) p).fChargeMin   = fChargeMin;
  ((IlcDCHpidLQ &) p).fNClusterMin = fNClusterMin;
  ((IlcDCHpidLQ &) p).fNLh         = fNLh;
  ((IlcDCHpidLQ &) p).fMinLh       = fMinLh;
  ((IlcDCHpidLQ &) p).fMaxLh       = fMaxLh;

  IlcDCHpid::Copy(p);

}

//_____________________________________________________________________________
Bool_t IlcDCHpidLQ::Init()
{
  //
  // Initializes the PID object 
  //

  fChargeMin   = 0.0;
  fNClusterMin = 10;

  fNLh         = 100;
  fMinLh       =   0.0;
  fMaxLh       = 500.0;

  return kTRUE;

}

//_____________________________________________________________________________
Bool_t IlcDCHpidLQ::AssignLikelihood(IlcDCHtrack *t)
{
  //
  // Assigns the e / pi likelihood to a given track
  //

  const Int_t kNpla = IlcDCHgeometry::Nplan();
  Float_t * charge = new Float_t[kNpla];
  Int_t   * nCluster = new Int_t[kNpla];

  Float_t  lhPi = 0;
  Float_t  lhEl = 0;

  Double_t pPi  = 1;
  Double_t pEl  = 1;
  Double_t pSum = 0;

  Int_t   indexEl;
  Int_t   indexPi;
  TH1F   *hTmpEl;
  TH1F   *hTmpPi;

  t->SetLikelihoodElectron(-1.);
  if (TMath::IsNaN(t->GetP())) return kFALSE;
  Float_t mom = t->GetP();

  // Calculate the total charge in each plane
  if (!SumCharge(t,charge,nCluster)) return kFALSE;

  indexEl = GetIndex(mom,kElectron);
  indexPi = GetIndex(mom,kPion);
  if ((indexEl > -1) && (indexPi > -1)) {
    hTmpEl = (TH1F *) fHist->UncheckedAt(indexEl);
    hTmpPi = (TH1F *) fHist->UncheckedAt(indexPi);
    for (Int_t ipla = 0; ipla < kNpla; ipla++) {
      Float_t chargePlane   = charge[ipla];
      Int_t   nClusterPlane = nCluster[ipla];
      if ((chargePlane   >   fChargeMin) &&
          (nClusterPlane > fNClusterMin)){
        Float_t chargeNorm = chargePlane / ((Float_t) nClusterPlane);
        if (chargeNorm < fMaxLh) {
          Int_t   ibinEl     = hTmpEl->FindBin(chargeNorm);
          Float_t pElPlane   = hTmpEl->GetBinContent(ibinEl);
          if (pElPlane > 0) { 
            pEl  = pEl * pElPlane;
          } 
          Int_t   ibinPi     = hTmpPi->FindBin(chargeNorm);
          Float_t pPiPlane   = hTmpPi->GetBinContent(ibinPi);
          if (pPiPlane > 0) { 
            pPi  = pPi * pPiPlane;
          } 
//           printf("          ipla = %d, nCluster = %d, charge = %f\n"
//                  ,ipla,nClusterPlane,chargeNorm);
//           printf("electron: pElPlane = %f, ibinEl = %d, pEl = %f\n"
// 	         ,pElPlane,ibinEl,pEl);
//           printf("    pion: pPiPlane = %f, ibinPi = %d, pPi = %f\n"
// 	         ,pPiPlane,ibinPi,pPi);
	}
      }
    }
  }
  else {
    delete [] charge;
    delete [] nCluster;
    return kTRUE;
  }

  pSum = pEl + pPi;
  if (pSum <= 0) {
    delete [] charge;
    delete [] nCluster;
    return kFALSE;
  }
  lhEl = pEl / pSum;
  lhPi = pPi / pSum;

//   printf("---- mom = %f, lhEl = %f, lhPi = %f\n",mom,lhEl,lhPi);

  // Assign the likelihoods 
  t->SetLikelihoodElectron(lhEl);

  delete [] charge;
  delete [] nCluster;
  return kTRUE;  

}

//_____________________________________________________________________________
Bool_t IlcDCHpidLQ::CreateHistograms(Int_t   nmom, Float_t minmom, Float_t maxmom)
{
  //
  // Creates the histograms
  //

  Int_t imom;
  Int_t ipid;

  fNMom   = nmom;
  fMinMom = minmom;
  fMaxMom = maxmom;
  fWidMom = (maxmom - minmom) / ((Float_t) nmom);

  fHist = new TObjArray(kNpid * nmom);
  for (imom = 0; imom < nmom;  imom++) {
    for (ipid = 0; ipid < kNpid; ipid++) {

      Int_t index = GetIndex(imom,ipid);
      Char_t name[10];
      Char_t title[80];
      sprintf(name ,"hQ%03d",index);
      if (ipid == kElectron) {
        sprintf(title,"Q electron p-bin %03d",imom);
      }
      else {
        sprintf(title,"Q pion p-bin %03d",imom);
      }
      TH1F *hTmp = new TH1F(name,title,fNLh,fMinLh,fMaxLh);
      fHist->AddAt(hTmp,index);

    }
  }

  return kTRUE;

}

// //_____________________________________________________________________________
// Bool_t IlcDCHpidLQ::FillSpectra(const IlcDCHtrack *t)
// {
//   //
//   // Fills the energy loss distributions with track <t>.
//   //

//   Bool_t status = kTRUE;

//   if (TMath::IsNaN(t->GetP())) return kFALSE;

//   Float_t        mom     = t->GetP();
//   Int_t          ipid    = MCpid(t);
//   TH1F          *hTmp    = NULL;
//   IlcDCHcluster *cluster = NULL;

//   Int_t index = GetIndex(mom,ipid);
//   if (index > -1) {
//     hTmp = (TH1F *) fHist->UncheckedAt(index);
//     // Loop through all clusters associated to this track
//     Int_t nClus = t->GetNclusters();
//     for (Int_t iClus = 0; iClus < nClus; iClus++) {
//       // Get a cluster
//       Int_t idxClus = t->GetClusterIndex(iClus);
//       if (!(cluster = (IlcDCHcluster *) fClusterArray->UncheckedAt(idxClus))) {
//         status = kFALSE;
//         break;
//       } 
//       hTmp->Fill(cluster->GetQ());
//     }
//   }  

//   return status;

// }

//_____________________________________________________________________________
Bool_t IlcDCHpidLQ::FillSpectra(const IlcDCHtrack *t)
{
  //
  // Fills the energy loss distributions with track <t>.
  //

  const Int_t kNpla = IlcDCHgeometry::Nplan();

  if (TMath::IsNaN(t->GetP())) return kFALSE;

  Float_t * charge = new Float_t[kNpla];
  Int_t   * nCluster = new Int_t[kNpla];
  Float_t mom  = t->GetP();
  Int_t   ipid = MCpid(t);
  TH1F   *hTmp = NULL;

  if (!SumCharge(t,charge,nCluster)) {
    delete [] charge;
    delete [] nCluster;
    return kFALSE;
  }

  Int_t index = GetIndex(mom,ipid);
  if (index > -1) {
    hTmp = (TH1F *) fHist->UncheckedAt(index);
    for (Int_t ipla = 0; ipla < kNpla; ipla++) {
      if ((charge[ipla]   >   fChargeMin) &&
          (nCluster[ipla] > fNClusterMin)){
        hTmp->Fill(charge[ipla] / ((Float_t) nCluster[ipla]));
      }
    }
  }  

  delete [] charge;
  delete [] nCluster;
  return kTRUE;

}

//_____________________________________________________________________________
Int_t IlcDCHpidLQ::GetIndex(const IlcDCHtrack *t)
{
  //
  // Returns the histogram index
  //

  if (TMath::IsNaN(t->GetP())) return -1;
  Float_t mom  = t->GetP();
  Int_t   ipid = MCpid(t);

  return GetIndex(mom,ipid);

}

//_____________________________________________________________________________
Int_t IlcDCHpidLQ::GetIndex(Float_t mom, Int_t ipid)
{
  //
  // Returns the histogram index
  //

  if ((mom < fMinMom) || (mom > fMaxMom))  return -1;
  Int_t imom = ((Int_t) ((mom - fMinMom) / fWidMom));
  return GetIndex(imom,ipid);

}

//_____________________________________________________________________________
Int_t IlcDCHpidLQ::GetIndex(Int_t imom, Int_t ipid)
{
  //
  // Returns the histogram index
  //

  if ((ipid < 0) || (ipid >= kNpid)) return -1;
  return imom * kNpid + ipid; 

}
