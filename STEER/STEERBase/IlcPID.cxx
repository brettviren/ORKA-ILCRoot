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

/* $Id: IlcPID.cxx 53929 2012-01-16 13:31:28Z fca $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// particle id probability densities                                         //
//                                                                           //
// The IlcPID class stores the probability densities for the different       //
// particle type hypotheses electron, muon, pion, kaon, proton, photon,      //
// pi0, neutron, K0 and electron conversion. These probability densities     //
// are determined from the detector response functions.                      //
// The * and *= operators are overloaded for IlcPID to combine the PIDs      //
// from different detectors.                                                 //
//                                                                           //
// The Bayesian probability to be a particle of a given type can be          //
// calculated from the probability densities, if the a priori probabilities  //
// (or abundences, concentrations) of particle species are known. These      //
// priors can be given as argument to the GetProbability or GetMostProbable  //
// method or they can be set globally by calling the static method           //
// SetPriors().                                                              //
//                                                                           //
// The implementation of this class is based on the note ...                 //
// by Iouri Belikov and Karel Safarik.                                       //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <TClass.h>
#include <TDatabasePDG.h>
#include <TPDGCode.h>

#include "IlcLog.h"
#include "IlcPDG.h"
#include "IlcPID.h"

#define M(PID) TDatabasePDG::Instance()->GetParticle(fgkParticleCode[(PID)])->Mass()

ClassImp(IlcPID)

const char* IlcPID::fgkParticleName[IlcPID::kSPECIESN+IlcPID::kSPECIESLN+1] = {
  "electron",
  "muon",
  "pion",
  "kaon",
  "proton",
  "photon",
  "pi0",
  "neutron",
  "kaon0",
  "eleCon",
  "deuteron",
  "triton",
  "helium-3",
  "alpha",
  "unknown"
};

const char* IlcPID::fgkParticleShortName[IlcPID::kSPECIESN+IlcPID::kSPECIESLN+1] = {
  "e",
  "mu",
  "pi",
  "K",
  "p",
  "photon",
  "pi0",
  "n",
  "K0",
  "eleCon",
  "d",
  "t",
  "he3",
  "alpha",
  "unknown"
};

const char* IlcPID::fgkParticleLatexName[IlcPID::kSPECIESN+IlcPID::kSPECIESLN+1] = {
  "e",
  "#mu",
  "#pi",
  "K",
  "p",
  "#gamma",
  "#pi_{0}",
  "n",
  "K_{0}",
  "eleCon",
  "d",
  "t",
  "he3",
  "#alpha",
  "unknown"
};

const Int_t IlcPID::fgkParticleCode[IlcPID::kSPECIESN+IlcPID::kSPECIESLN+1] = {
  ::kElectron, 
  ::kMuonMinus, 
  ::kPiPlus, 
  ::kKPlus, 
  ::kProton,
  ::kGamma,
  ::kPi0,
  ::kNeutron,
  ::kK0,
  ::kElectron,
  1000010020,
  1000010030,
  1000020030,
  1000020040,
  0
};

/*const*/ Float_t IlcPID::fgkParticleMass[IlcPID::kSPECIESN+IlcPID::kSPECIESLN+1] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
  /*
  M(kElectron),  // electron
  M(kMuon), // muon
  M(kPion),    // pion
  M(kKaon),     // kaon
  M(kProton),    // proton
  M(kPhoton),     // photon
  M(kPi0),       // pi0
  M(kNeutron),   // neutron
  M(kKaon0),        // kaon0
  M(kEleCon),     // electron conversion
  M(kDeuteron), // deuteron
  M(kTriton),   // triton
  M(kHe3),      // he3
  M(kAlpha),    // alpha
  0.00000        // unknown
  */
};

Double_t IlcPID::fgPrior[kSPECIESN] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


//_______________________________________________________________________
IlcPID::IlcPID() :
  TObject(),
  fCharged(0)
{
  //
  // Default constructor
  //
  Init();
  // set default values (= equal probabilities)
  for (Int_t i = 0; i < kSPECIESN; i++)
    fProbDensity[i] = 1./kSPECIESN;
}

//_______________________________________________________________________
IlcPID::IlcPID(const Double_t* probDensity, Bool_t charged) : 
  TObject(),
  fCharged(charged)
{
  //
  // Standard constructor
  //
  Init();
  // set given probability densities
  for (Int_t i = 0; i < kSPECIES; i++) 
    fProbDensity[i] = probDensity[i];

  for (Int_t i = kSPECIES; i < kSPECIESN; i++) 
    fProbDensity[i] = ((charged) ? 0 : probDensity[i]);
}

//_______________________________________________________________________
IlcPID::IlcPID(const Float_t* probDensity, Bool_t charged) :
  TObject(),
  fCharged(charged)
{
  //
  // Standard constructor
  //
  Init();
  // set given probability densities
  for (Int_t i = 0; i < kSPECIES; i++) 
    fProbDensity[i] = probDensity[i];

  for (Int_t i = kSPECIES; i < kSPECIESN; i++) 
    fProbDensity[i] = ((charged) ? 0 : probDensity[i]);
}

//_______________________________________________________________________
IlcPID::IlcPID(const IlcPID& pid) : 
  TObject(pid),
  fCharged(pid.fCharged)
{
  //
  // copy constructor
  //
  // We do not call init here, MUST already be done
  for (Int_t i = 0; i < kSPECIESN; i++) 
    fProbDensity[i] = pid.fProbDensity[i];
}

//_______________________________________________________________________
void IlcPID::SetProbabilities(const Double_t* probDensity, Bool_t charged) 
{
  //
  // Set the probability densities
  //
  for (Int_t i = 0; i < kSPECIES; i++) 
    fProbDensity[i] = probDensity[i];

  for (Int_t i = kSPECIES; i < kSPECIESN; i++) 
    fProbDensity[i] = ((charged) ? 0 : probDensity[i]);
}

//_______________________________________________________________________
IlcPID& IlcPID::operator = (const IlcPID& pid)
{
// assignment operator

  if(this != &pid) {
    fCharged = pid.fCharged;
    for (Int_t i = 0; i < kSPECIESN; i++) {
      fProbDensity[i] = pid.fProbDensity[i];
    }
  }
  return *this;
}

//_______________________________________________________________________
void IlcPID::Init() 
{
  //
  // Initialise the masses
  //
  // Initialise only once... 
  if(!fgkParticleMass[0]) {
    IlcPDG::AddParticlesToPdgDataBase();
    for (Int_t i = 0; i < kSPECIESN + kSPECIESLN; i++) 
      fgkParticleMass[i] = M(i);
  }
}

//_____________________________________________________________________________
Double_t IlcPID::GetProbability(EParticleType iType,
				const Double_t* prior) const
{
  //
  // Get the probability to be a particle of type "iType"
  // assuming the a priori probabilities "prior"
  //
  Double_t sum = 0.;
  Int_t nSpecies = ((fCharged) ? kSPECIES : kSPECIESN);
  for (Int_t i = 0; i < nSpecies; i++) {
    sum += fProbDensity[i] * prior[i];
  }
  if (sum <= 0) {
    IlcError("Invalid probability densities or priors");
    return -1;
  }
  return fProbDensity[iType] * prior[iType] / sum;
}

//_____________________________________________________________________________
Double_t IlcPID::GetProbability(EParticleType iType) const
{
// get the probability to be a particle of type "iType"
// assuming the globaly set a priori probabilities

  return GetProbability(iType, fgPrior);
}

//_____________________________________________________________________________
void IlcPID::GetProbabilities(Double_t* probabilities,
			      const Double_t* prior) const
{
// get the probabilities to be a particle of given type
// assuming the a priori probabilities "prior"

  Double_t sum = 0.;
  Int_t nSpecies = ((fCharged) ? kSPECIES : kSPECIESN);
  for (Int_t i = 0; i < nSpecies; i++) {
    sum += fProbDensity[i] * prior[i];
  }
  if (sum <= 0) {
    IlcError("Invalid probability densities or priors");
    for (Int_t i = 0; i < nSpecies; i++) probabilities[i] = -1;
    return;
  }
  for (Int_t i = 0; i < nSpecies; i++) {
    probabilities[i] = fProbDensity[i] * prior[i] / sum;
  }
}

//_____________________________________________________________________________
void IlcPID::GetProbabilities(Double_t* probabilities) const
{
// get the probabilities to be a particle of given type
// assuming the globaly set a priori probabilities

  GetProbabilities(probabilities, fgPrior);
}

//_____________________________________________________________________________
IlcPID::EParticleType IlcPID::GetMostProbable(const Double_t* prior) const
{
// get the most probable particle id hypothesis
// assuming the a priori probabilities "prior"

  Double_t max = 0.;
  EParticleType id = kPion;
  Int_t nSpecies = ((fCharged) ? kSPECIES : kSPECIESN);
  for (Int_t i = 0; i < nSpecies; i++) {
    Double_t prob = fProbDensity[i] * prior[i];
    if (prob > max) {
      max = prob;
      id = EParticleType(i);
    }
  }
  if (max == 0) {
    IlcError("Invalid probability densities or priors");
  }
  return id;
}

//_____________________________________________________________________________
IlcPID::EParticleType IlcPID::GetMostProbable() const
{
// get the most probable particle id hypothesis
// assuming the globaly set a priori probabilities

  return GetMostProbable(fgPrior);
}


//_____________________________________________________________________________
void IlcPID::SetPriors(const Double_t* prior, Bool_t charged)
{
// use the given priors as global a priori probabilities

  Double_t sum = 0;
  for (Int_t i = 0; i < kSPECIESN; i++) {
    if (charged && (i >= kSPECIES)) {
      fgPrior[i] = 0;      
    } else {
      if (prior[i] < 0) {
	IlcWarningClass(Form("negative prior (%g) for %ss. "
			     "Using 0 instead.", prior[i], 
			     fgkParticleName[i]));
	fgPrior[i] = 0;
      } else {
	fgPrior[i] = prior[i];
      }
    }
    sum += prior[i];
  }
  if (sum == 0) {
    IlcWarningClass("all priors are zero.");
  }
}

//_____________________________________________________________________________
void IlcPID::SetPrior(EParticleType iType, Double_t prior)
{
// use the given prior as global a priori probability for particles
// of type "iType"

  if (prior < 0) {
    IlcWarningClass(Form("negative prior (%g) for %ss. Using 0 instead.", 
			 prior, fgkParticleName[iType]));
    prior = 0;
  }
  fgPrior[iType] = prior;
}


//_____________________________________________________________________________
IlcPID& IlcPID::operator *= (const IlcPID& pid)
{
// combine this probability densities with the one of "pid"

  for (Int_t i = 0; i < kSPECIESN; i++) {
    fProbDensity[i] *= pid.fProbDensity[i];
  }
  return *this;
}

//_____________________________________________________________________________
IlcPID operator * (const IlcPID& pid1, const IlcPID& pid2)
{
// combine the two probability densities

  IlcPID result;
  result *= pid1;
  result *= pid2;
  return result;
}
//_______________________________________________________________________
#ifdef WIN32
   Float_t       IlcPID::ParticleMass(Int_t iType) {
     if(!fgkParticleMass[0]) Init(); 
     return fgkParticleMass[iType];
  }
   Int_t IlcPID::ParticleCode(Int_t iType) 
    {return fgkParticleCode[iType];};
  const char*   IlcPID::ParticleName(Int_t iType) 
    {return fgkParticleName[iType];};
  const char*   IlcPID::ParticleShortName(Int_t iType) 
    {return fgkParticleShortName[iType];};
  const char*   IlcPID::ParticleLatexName(Int_t iType) 
    {return fgkParticleLatexName[iType];};
#endif


//_______________________________________________________________________