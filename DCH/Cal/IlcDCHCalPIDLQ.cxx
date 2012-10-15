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

//-----------------------------------------------------------------
// Class for dE/dx and Time Bin of Max. Cluster for Electrons and 
// pions in DCH. 
// It is instantiated in class IlcDCHpidESD for particle identification
// in DCH
// Prashant Shukla <shukla@pi0.physi.uni-heidelberg.de>
//-----------------------------------------------------------------

#include "IlcDCHCalPIDLQ.h"
#include <TH1F.h>
#include <TFile.h>

ClassImp(IlcDCHCalPIDLQ)

Char_t* IlcDCHCalPIDLQ::fpartName[IlcPID::kSPECIES] = {"electron", "muon", "pion", "kaon", "proton"};
    
//_________________________________________________________________________
IlcDCHCalPIDLQ::IlcDCHCalPIDLQ():
  TNamed(),
  fNMom(0),
  fTrackMomentum(0),
  fMeanChargeRatio(0),
  fNbins(0),
  fBinSize(0),
  fHistdEdx(0),
  fHistTimeBin(0)
{
  //
  //  The Default constructor
  //
  
}

//_________________________________________________________________________
IlcDCHCalPIDLQ::IlcDCHCalPIDLQ(const Text_t *name, const Text_t *title) : TNamed(name, title)
{
  //
  //  The main constructor
  //
  
  Init();
}

//_____________________________________________________________________________
IlcDCHCalPIDLQ::IlcDCHCalPIDLQ(const IlcDCHCalPIDLQ &c) : TNamed(c)
{
  //
  // copy constructor
  //

  Init();
  
  ((IlcDCHCalPIDLQ &) c).Copy(*this);
}

//_________________________________________________________________________
IlcDCHCalPIDLQ::~IlcDCHCalPIDLQ()
{
  //
  // Destructor
  //
  
  CleanUp();
}

//_________________________________________________________________________
void IlcDCHCalPIDLQ::CleanUp()
{
  if (fHistdEdx)
  {
    delete fHistdEdx;
    fHistdEdx = 0;
  }
  
  if (fHistTimeBin)
  {
    delete fHistTimeBin;
    fHistTimeBin = 0;
  }

  if (fTrackMomentum)
  {
    delete[] fTrackMomentum;
    fTrackMomentum = 0;
  }
}

//_____________________________________________________________________________
IlcDCHCalPIDLQ &IlcDCHCalPIDLQ::operator=(const IlcDCHCalPIDLQ &c)
{
  //
  // Assignment operator
  //

  if (this != &c) ((IlcDCHCalPIDLQ &) c).Copy(*this);
  return *this;
}

//_____________________________________________________________________________
void IlcDCHCalPIDLQ::Copy(TObject &c) const
{
  //
  // Copy function
  //

  IlcDCHCalPIDLQ& target = (IlcDCHCalPIDLQ &) c;
  
  target.CleanUp();
  
  target.fNMom = fNMom;
  
  target.fTrackMomentum = new Double_t[fNMom];
  for (Int_t i=0; i<fNMom; ++i)
    target.fTrackMomentum[i] = fTrackMomentum[i];
      
  target.fMeanChargeRatio = fMeanChargeRatio;

  target.fNbins = fNbins;
  target.fBinSize = fBinSize;

  if (fHistdEdx)
    target.fHistdEdx = (TObjArray*) fHistdEdx->Clone();
  
  if (fHistTimeBin)
    target.fHistTimeBin = (TObjArray*) fHistTimeBin->Clone();
    
  TObject::Copy(c);
}

//_________________________________________________________________________
void IlcDCHCalPIDLQ::Init()
{
  fNMom = 11;
  
  fTrackMomentum = new Double_t[fNMom];
  Double_t trackMomentum[11] = {0.6, 0.8, 1, 1.5, 2, 3, 4, 5, 6, 8, 10};
  for (Int_t imom = 0; imom < 11; imom++)
    fTrackMomentum[imom] = trackMomentum[imom];
  
  fHistdEdx = new TObjArray(IlcPID::kSPECIES * fNMom);
  fHistdEdx->SetOwner();
  fHistTimeBin = new TObjArray(IlcPID::kSPECIES * fNMom);
  fHistTimeBin->SetOwner();  
  
  // ADC Gain normilczation
  fMeanChargeRatio=1.0;
  
  // Number of bins and bin size
  fNbins = 0;
  fBinSize = 0.;
}

//_________________________________________________________________________
Bool_t IlcDCHCalPIDLQ::ReadData(Char_t *responseFile)
{
  //
  // Read the DCH dEdx histograms.
  //
  // Read histogram Root file  
  TFile *histFile = new TFile(responseFile, "READ");
  if (!histFile || !histFile->IsOpen()) {
    Error("IlcDCHCalPIDLQ", "opening DCH histgram file %s failed", responseFile);
    return kFALSE;
  }
  gROOT->cd();

  // Read histograms
  Char_t text[200];
  for (Int_t particle = 0; particle < IlcPID::kSPECIES; ++particle)
  {
    Char_t* particleKey = "";
    switch (particle)
    {
      case IlcPID::kElectron: particleKey = "EL"; break;
      case IlcPID::kPion: particleKey = "PI"; break;
      case IlcPID::kMuon: particleKey = "MU"; break;
      case IlcPID::kKaon: particleKey = "KA"; break;
      case IlcPID::kProton: particleKey = "PR"; break;
    }
    
    for (Int_t imom = 0; imom < fNMom; imom++) 
    {
      sprintf(text, "h1dEdx%s%01d", particleKey, imom+1);
      TH1F* hist = (TH1F*)histFile->Get(text)->Clone();
      hist->Scale(1.0/hist->Integral());
      fHistdEdx->AddAt(hist, GetHistID(particle, imom));
  
      if (particle == IlcPID::kElectron || particle == IlcPID::kPion)
      {
        sprintf(text,"h1MaxTimBin%s%01d", particleKey, imom+1);
        TH1F* hist = (TH1F*)histFile->Get(text)->Clone();
        hist->Scale(1.0/hist->Integral());
        fHistTimeBin->AddAt(hist, GetHistID(particle,imom));
      }
    }
  }
  
  histFile->Close();
  delete histFile;
  
  // Number of bins and bin size
  TH1F* hist = (TH1F*) fHistdEdx->At(GetHistID(IlcPID::kPion, 1));
  fNbins   = hist->GetNbinsX();
  fBinSize = hist->GetBinWidth(1);
  
  return kTRUE;
}

//_________________________________________________________________________
Double_t  IlcDCHCalPIDLQ::GetMean(Int_t k, Int_t ip) const
{
  //
  // Gets mean of de/dx dist. of e
  printf("Mean for particle = %s and momentum = %.2f is:\n", fpartName[k], fTrackMomentum[ip]);
  if (k < 0 || k > IlcPID::kSPECIES)
    return 0;
  
  return ((TH1F*) fHistdEdx->At(GetHistID(k,ip)))->GetMean();
}

//_________________________________________________________________________
Double_t  IlcDCHCalPIDLQ::GetNormilczation(Int_t k, Int_t ip) const
{
  //
  // Gets Normilczation of de/dx dist. of e

  printf("Normilczation for particle = %s and momentum = %.2f is:\n",fpartName[k], fTrackMomentum[ip]);
  if (k < 0 || k > IlcPID::kSPECIES)
    return 0;
  
  return ((TH1F*) fHistdEdx->At(GetHistID(k,ip)))->Integral();
}

//_________________________________________________________________________
TH1F* IlcDCHCalPIDLQ::GetHistogram(Int_t k, Int_t ip) const
{
  //
  //
  printf("Histogram for particle = %s and momentum = %.2f is:\n", fpartName[k], fTrackMomentum[ip]);
  if (k < 0 || k > IlcPID::kSPECIES)
    return 0;
  
  return (TH1F*) fHistdEdx->At(GetHistID(k,ip));
}

//_________________________________________________________________________
Double_t IlcDCHCalPIDLQ::GetProbability(Int_t k, Double_t mom, Double_t dedx1) const
{
  //
  // Gets the Probability of having dedx at a given momentum (mom)
  // and particle type k (0 for e) and (2 for pi)
  // from the precalculated de/dx distributions 
  
  Double_t dedx = dedx1/fMeanChargeRatio;
  Int_t iEnBin= ((Int_t) (dedx/fBinSize+1));
  if(iEnBin > fNbins) iEnBin = fNbins;

  if (k < 0 || k > IlcPID::kSPECIES)
    return 1;
  
  TH1F* hist1 = 0;
  TH1F* hist2 = 0;
  Double_t mom1 = 0;
  Double_t mom2 = 0;
  
  // Lower limit
  if (mom<=fTrackMomentum[0]) 
  {
    hist1 = (TH1F*) fHistdEdx->At(GetHistID(k,1));
    hist2 = (TH1F*) fHistdEdx->At(GetHistID(k,0));
    mom1 = fTrackMomentum[1];
    mom2 = fTrackMomentum[0];
  }
    
  // Upper Limit
  if(mom>=fTrackMomentum[fNMom-1]) 
  {
    hist2 = (TH1F*) fHistdEdx->At(GetHistID(k,fNMom-1));
    hist1 = (TH1F*) fHistdEdx->At(GetHistID(k,fNMom-2));
    mom2 = fTrackMomentum[fNMom-1];
    mom1 = fTrackMomentum[fNMom-2];
  }
    
  // In the range
  for (Int_t ip=1; ip<fNMom; ip++) 
  {
    if ((fTrackMomentum[ip-1]<= mom) && (mom<fTrackMomentum[ip])) 
    {
      hist1 = (TH1F*) fHistdEdx->At(GetHistID(k,ip));
      hist2 = (TH1F*) fHistdEdx->At(GetHistID(k,ip-1));
      mom1 = fTrackMomentum[ip];
      mom2 = fTrackMomentum[ip-1];
    }
  }
  
  Double_t slop = (hist1->GetBinContent(iEnBin) - hist2->GetBinContent(iEnBin)) / (mom1 - mom2);
  return hist2->GetBinContent(iEnBin) + slop * (mom - mom2);
}

//_________________________________________________________________________
Double_t IlcDCHCalPIDLQ::GetProbabilityT(Int_t k, Double_t mom, Int_t timbin) const
{
  //
  // Gets the Probability of having timbin at a given momentum (mom)
  // and particle type k (0 for e) and (2 for pi)
  // from the precalculated timbin distributions 
  
  if (timbin<=0) 
    return 0.;
  Int_t iTBin=timbin+1;
  
  // everything which is not electron counts as pion for time bin
  if (k != IlcPID::kElectron)
    k = IlcPID::kPion;

  if (mom<=fTrackMomentum[0]) 
    return ((TH1F*) fHistTimeBin->At(GetHistID(k,0)))->GetBinContent(iTBin);
  
  if (mom>=fTrackMomentum[fNMom-1]) 
    return ((TH1F*) fHistTimeBin->At(GetHistID(k,fNMom-1)))->GetBinContent(iTBin);
  
  for (Int_t ip=1; ip<fNMom; ip++)
  {
    if ((fTrackMomentum[ip-1]<= mom) && (mom<fTrackMomentum[ip])) 
    {
      Double_t slop=(((TH1F*) fHistTimeBin->At(GetHistID(k,ip)))->GetBinContent(iTBin) - ((TH1F*) fHistTimeBin->At(GetHistID(k,ip-1)))->GetBinContent(iTBin)) / (fTrackMomentum[ip] - fTrackMomentum[ip-1]);
      // Linear Interpolation
      return ((TH1F*) fHistTimeBin->At(GetHistID(k,ip-1)))->GetBinContent(iTBin) + slop * (mom - fTrackMomentum[ip-1]);
    }
  }
  
  return -1;
}
