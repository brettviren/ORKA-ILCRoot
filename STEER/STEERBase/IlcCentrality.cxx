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

//*****************************************************
//   Class IlcCentrality
//   author: Alberica Toia
//*****************************************************
/// A container for the centrality stored in AOD in ESD
 
#include "IlcCentrality.h"

ClassImp(IlcCentrality)

IlcCentrality::IlcCentrality() : TNamed("Centrality", "Centrality"),
  fQuality(999),
  fCentralityV0M(0),
  fCentralityV0A(0),
  fCentralityV0C(0),
  fCentralityFMD(0),
  fCentralityTRK(0),
  fCentralityTKL(0),
  fCentralityCL0(0),
  fCentralityCL1(0),
  fCentralityCND(0),
  fCentralityV0MvsFMD(0),
  fCentralityTKLvsV0M(0),
  fCentralityZEMvsZDC(0)
{
  /// constructor
}

IlcCentrality::IlcCentrality(const IlcCentrality& cnt) : 
  TNamed(cnt),
  fQuality(cnt.fQuality), 
  fCentralityV0M(cnt.fCentralityV0M),
  fCentralityV0A(cnt.fCentralityV0A),
  fCentralityV0C(cnt.fCentralityV0C),
  fCentralityFMD(cnt.fCentralityFMD),
  fCentralityTRK(cnt.fCentralityTRK),
  fCentralityTKL(cnt.fCentralityTKL),
  fCentralityCL0(cnt.fCentralityCL0),
  fCentralityCL1(cnt.fCentralityCL1),
  fCentralityCND(cnt.fCentralityCND),
  fCentralityV0MvsFMD(cnt.fCentralityV0MvsFMD),
  fCentralityTKLvsV0M(cnt.fCentralityTKLvsV0M),
  fCentralityZEMvsZDC(cnt.fCentralityZEMvsZDC)
{
  /// Copy constructor
}

IlcCentrality& IlcCentrality::operator=(const IlcCentrality& c)
{
  /// Assignment operator
  if (this!=&c) {
    TNamed::operator=(c);
    fQuality = c.fQuality;
    fCentralityV0M = c.fCentralityV0M;
    fCentralityV0A = c.fCentralityV0A;
    fCentralityV0C = c.fCentralityV0C;
    fCentralityFMD = c.fCentralityFMD;
    fCentralityTRK = c.fCentralityTRK;
    fCentralityTKL = c.fCentralityTKL;
    fCentralityCL0 = c.fCentralityCL0;
    fCentralityCL1 = c.fCentralityCL1;
    fCentralityCND = c.fCentralityCND;
    fCentralityV0MvsFMD = c.fCentralityV0MvsFMD;
    fCentralityTKLvsV0M = c.fCentralityTKLvsV0M;
    fCentralityZEMvsZDC = c.fCentralityZEMvsZDC;
  }

  return *this;
}

IlcCentrality::~IlcCentrality()
{
  /// destructor
}

Int_t IlcCentrality::GetQuality() const
{
  return fQuality;
}

Float_t IlcCentrality::GetCentralityPercentile(const char *x) const
{
// Return the centrality percentile
  if (fQuality == 0) {
    TString method = x;
    if(method.CompareTo("V0M")==0)      return fCentralityV0M;
    if(method.CompareTo("V0A")==0)      return fCentralityV0A;
    if(method.CompareTo("V0C")==0)      return fCentralityV0C;
    if(method.CompareTo("FMD")==0)      return fCentralityFMD;
    if(method.CompareTo("TRK")==0)      return fCentralityTRK;
    if(method.CompareTo("TKL")==0)      return fCentralityTKL;
    if(method.CompareTo("CL0")==0)      return fCentralityCL0;
    if(method.CompareTo("CL1")==0)      return fCentralityCL1;
    if(method.CompareTo("CND")==0)      return fCentralityCND;
    if(method.CompareTo("V0MvsFMD")==0) return fCentralityV0MvsFMD;
    if(method.CompareTo("TKLvsV0M")==0) return fCentralityTKLvsV0M;
    if(method.CompareTo("ZEMvsZDC")==0) return fCentralityZEMvsZDC;
    return -1;
  } else {
    return -1;
  }
}

Int_t IlcCentrality::GetCentralityClass10(const char *x) const
{
// Return the centrality class
  if (fQuality == 0) {
    TString method = x;
    if(method.CompareTo("V0M")==0)      return (Int_t) (fCentralityV0M / 10.0);
    if(method.CompareTo("V0A")==0)      return (Int_t) (fCentralityV0A / 10.0);
    if(method.CompareTo("V0C")==0)      return (Int_t) (fCentralityV0C / 10.0);
    if(method.CompareTo("FMD")==0)      return (Int_t) (fCentralityFMD / 10.0);
    if(method.CompareTo("TRK")==0)      return (Int_t) (fCentralityTRK / 10.0);
    if(method.CompareTo("TKL")==0)      return (Int_t) (fCentralityTKL / 10.0);
    if(method.CompareTo("CL0")==0)      return (Int_t) (fCentralityCL0 / 10.0);
    if(method.CompareTo("CL1")==0)      return (Int_t) (fCentralityCL1 / 10.0);
    if(method.CompareTo("CND")==0)      return (Int_t) (fCentralityCND / 10.0);
    if(method.CompareTo("V0MvsFMD")==0) return (Int_t) (fCentralityV0MvsFMD / 10.0);
    if(method.CompareTo("TKLvsV0M")==0) return (Int_t) (fCentralityTKLvsV0M / 10.0);
    if(method.CompareTo("ZEMvsZDC")==0) return (Int_t) (fCentralityZEMvsZDC / 10.0);
    return -1;
  } else {
    return -1;
  }
}

Int_t IlcCentrality::GetCentralityClass5(const char *x) const
{
// Return the centrality class
  if (fQuality == 0) {
    TString method = x;
    if(method.CompareTo("V0M")==0)      return (Int_t) (fCentralityV0M / 5.0);
    if(method.CompareTo("V0A")==0)      return (Int_t) (fCentralityV0A / 5.0);
    if(method.CompareTo("V0C")==0)      return (Int_t) (fCentralityV0C / 5.0);
    if(method.CompareTo("FMD")==0)      return (Int_t) (fCentralityFMD / 5.0);
    if(method.CompareTo("TRK")==0)      return (Int_t) (fCentralityTRK / 5.0);
    if(method.CompareTo("TKL")==0)      return (Int_t) (fCentralityTKL / 5.0);
    if(method.CompareTo("CL0")==0)      return (Int_t) (fCentralityCL0 / 5.0);
    if(method.CompareTo("CL1")==0)      return (Int_t) (fCentralityCL1 / 5.0);
    if(method.CompareTo("CND")==0)      return (Int_t) (fCentralityCND / 5.0);
    if(method.CompareTo("V0MvsFMD")==0) return (Int_t) (fCentralityV0MvsFMD / 5.0);
    if(method.CompareTo("TKLvsV0M")==0) return (Int_t) (fCentralityTKLvsV0M / 5.0);
    if(method.CompareTo("ZEMvsZDC")==0) return (Int_t) (fCentralityZEMvsZDC / 5.0);
    return -1;
  } else {
    return -1;
  }
}


Bool_t IlcCentrality::IsEventInCentralityClass(Float_t a, Float_t b, const char *x) const
{
// True if event is inside a given class
  if (fQuality == 0) {
    TString method = x;
    if ((method.CompareTo("V0M")==0) && (fCentralityV0M >=a && fCentralityV0M < b)) return kTRUE;
    if ((method.CompareTo("V0A")==0) && (fCentralityV0A >=a && fCentralityV0A < b)) return kTRUE;
    if ((method.CompareTo("V0C")==0) && (fCentralityV0C >=a && fCentralityV0C < b)) return kTRUE;
    if ((method.CompareTo("FMD")==0) && (fCentralityFMD >=a && fCentralityFMD < b)) return kTRUE;
    if ((method.CompareTo("TRK")==0) && (fCentralityTRK >=a && fCentralityTRK < b)) return kTRUE;
    if ((method.CompareTo("TKL")==0) && (fCentralityTKL >=a && fCentralityTKL < b)) return kTRUE;
    if ((method.CompareTo("CL0")==0) && (fCentralityCL0 >=a && fCentralityCL0 < b)) return kTRUE;
    if ((method.CompareTo("CL1")==0) && (fCentralityCL1 >=a && fCentralityCL1 < b)) return kTRUE;
    if ((method.CompareTo("CND")==0) && (fCentralityCND >=a && fCentralityCND < b)) return kTRUE;
    if ((method.CompareTo("V0MvsFMD")==0) && (fCentralityV0MvsFMD >=a && fCentralityV0MvsFMD < b)) return kTRUE;
    if ((method.CompareTo("TKLvsV0M")==0) && (fCentralityTKLvsV0M >=a && fCentralityTKLvsV0M < b)) return kTRUE;
    if ((method.CompareTo("ZEMvsZDC")==0) && (fCentralityZEMvsZDC >=a && fCentralityZEMvsZDC < b)) return kTRUE;
    else return kFALSE;
  } else {
    return kFALSE;
  }
}

Float_t IlcCentrality::GetCentralityPercentileUnchecked(const char *x) const
{
// Return the centrality percentile
  TString method = x;
  if(method.CompareTo("V0M")==0)      return fCentralityV0M;
  if(method.CompareTo("V0A")==0)      return fCentralityV0A;
  if(method.CompareTo("V0C")==0)      return fCentralityV0C;
  if(method.CompareTo("FMD")==0)      return fCentralityFMD;
  if(method.CompareTo("TRK")==0)      return fCentralityTRK;
  if(method.CompareTo("TKL")==0)      return fCentralityTKL;
  if(method.CompareTo("CL0")==0)      return fCentralityCL0;
  if(method.CompareTo("CL1")==0)      return fCentralityCL1;
  if(method.CompareTo("CND")==0)      return fCentralityCND;
  if(method.CompareTo("V0MvsFMD")==0) return fCentralityV0MvsFMD;
  if(method.CompareTo("TKLvsV0M")==0) return fCentralityTKLvsV0M;
  if(method.CompareTo("ZEMvsZDC")==0) return fCentralityZEMvsZDC;
  return -1;
}

Int_t IlcCentrality::GetCentralityClass10Unchecked(const char *x) const
{
// Return the centrality class
  TString method = x;
  if(method.CompareTo("V0M")==0)      return (Int_t) (fCentralityV0M / 10.0);
  if(method.CompareTo("V0A")==0)      return (Int_t) (fCentralityV0A / 10.0);
  if(method.CompareTo("V0C")==0)      return (Int_t) (fCentralityV0C / 10.0);
  if(method.CompareTo("FMD")==0)      return (Int_t) (fCentralityFMD / 10.0);
  if(method.CompareTo("TRK")==0)      return (Int_t) (fCentralityTRK / 10.0);
  if(method.CompareTo("TKL")==0)      return (Int_t) (fCentralityTKL / 10.0);
  if(method.CompareTo("CL0")==0)      return (Int_t) (fCentralityCL0 / 10.0);
  if(method.CompareTo("CL1")==0)      return (Int_t) (fCentralityCL1 / 10.0);
  if(method.CompareTo("CND")==0)      return (Int_t) (fCentralityCND / 10.0);
  if(method.CompareTo("V0MvsFMD")==0) return (Int_t) (fCentralityV0MvsFMD / 10.0);
  if(method.CompareTo("TKLvsV0M")==0) return (Int_t) (fCentralityTKLvsV0M / 10.0);
  if(method.CompareTo("ZEMvsZDC")==0) return (Int_t) (fCentralityZEMvsZDC / 10.0);
  return -1;
}

Int_t IlcCentrality::GetCentralityClass5Unchecked(const char *x) const
{
// Return the centrality class
  TString method = x;
  if(method.CompareTo("V0M")==0)      return (Int_t) (fCentralityV0M / 5.0);
  if(method.CompareTo("V0A")==0)      return (Int_t) (fCentralityV0A / 5.0);
  if(method.CompareTo("V0C")==0)      return (Int_t) (fCentralityV0C / 5.0);
  if(method.CompareTo("FMD")==0)      return (Int_t) (fCentralityFMD / 5.0);
  if(method.CompareTo("TRK")==0)      return (Int_t) (fCentralityTRK / 5.0);
  if(method.CompareTo("TKL")==0)      return (Int_t) (fCentralityTKL / 5.0);
  if(method.CompareTo("CL0")==0)      return (Int_t) (fCentralityCL0 / 5.0);
  if(method.CompareTo("CL1")==0)      return (Int_t) (fCentralityCL1 / 5.0);
  if(method.CompareTo("CND")==0)      return (Int_t) (fCentralityCND / 5.0);
  if(method.CompareTo("V0MvsFMD")==0) return (Int_t) (fCentralityV0MvsFMD / 5.0);
  if(method.CompareTo("TKLvsV0M")==0) return (Int_t) (fCentralityTKLvsV0M / 5.0);
  if(method.CompareTo("ZEMvsZDC")==0) return (Int_t) (fCentralityZEMvsZDC / 5.0);
  return -1;
} 

Bool_t IlcCentrality::IsEventInCentralityClassUnchecked(Float_t a, Float_t b, const char *x) const
{
// True if event inside given centrality class
  TString method = x;
  if ((method.CompareTo("V0M")==0) && (fCentralityV0M >=a && fCentralityV0M < b)) return kTRUE;
  if ((method.CompareTo("V0A")==0) && (fCentralityV0A >=a && fCentralityV0A < b)) return kTRUE;
  if ((method.CompareTo("V0C")==0) && (fCentralityV0C >=a && fCentralityV0C < b)) return kTRUE;
  if ((method.CompareTo("FMD")==0) && (fCentralityFMD >=a && fCentralityFMD < b)) return kTRUE;
  if ((method.CompareTo("TRK")==0) && (fCentralityTRK >=a && fCentralityTRK < b)) return kTRUE;
  if ((method.CompareTo("TKL")==0) && (fCentralityTKL >=a && fCentralityTKL < b)) return kTRUE;
  if ((method.CompareTo("CL0")==0) && (fCentralityCL0 >=a && fCentralityCL0 < b)) return kTRUE;
  if ((method.CompareTo("CL1")==0) && (fCentralityCL1 >=a && fCentralityCL1 < b)) return kTRUE;
  if ((method.CompareTo("CND")==0) && (fCentralityCND >=a && fCentralityCND < b)) return kTRUE;
  if ((method.CompareTo("V0MvsFMD")==0) && (fCentralityV0MvsFMD >=a && fCentralityV0MvsFMD < b)) return kTRUE;
  if ((method.CompareTo("TKLvsV0M")==0) && (fCentralityTKLvsV0M >=a && fCentralityTKLvsV0M < b)) return kTRUE;
  if ((method.CompareTo("ZEMvsZDC")==0) && (fCentralityZEMvsZDC >=a && fCentralityZEMvsZDC < b)) return kTRUE;
  else return kFALSE;
} 

void IlcCentrality::Reset()
{
// Reset.

  fQuality            =  999;
  fCentralityV0M      =  0;
  fCentralityV0A      =  0;
  fCentralityV0C      =  0;
  fCentralityFMD      =  0;
  fCentralityTRK      =  0;
  fCentralityTKL      =  0;
  fCentralityCL0      =  0;
  fCentralityCL1      =  0;
  fCentralityCND      =  0;
  fCentralityV0MvsFMD =  0;
  fCentralityTKLvsV0M =  0;
  fCentralityZEMvsZDC =  0;
}
