#ifndef ILCPID_H
#define ILCPID_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

///////////////////////////////////////////////////////////////////////////////
///                                                                          //
/// particle id probability densities                                        //
///                                                                          //
///////////////////////////////////////////////////////////////////////////////

/* $Id: IlcPID.h 50615 2011-07-16 23:19:19Z hristov $ */


#include <TObject.h>


class IlcPID : public TObject {
 public:
  enum {
    kSPECIES = 5,    // Number of particle species recognized by the PID
    kSPECIESN = 10,  // Number of charged+neutral particle species recognized by the PHOS/EMCAL PID
    kSPECIESLN = 4   // Number of light nuclei: deuteron, triton, helium-3 and alpha
  };
  enum EParticleType {
    kElectron = 0, 
    kMuon = 1, 
    kPion = 2, 
    kKaon = 3, 
    kProton = 4, 
    kPhoton = 5, 
    kPi0 = 6, 
    kNeutron = 7, 
    kKaon0 = 8, 
    kEleCon = 9,
    kDeuteron = 10,
    kTriton = 11,
    kHe3 = 12,
    kAlpha = 13,
    kUnknown = 14
  };
 #ifdef WIN32
  static Float_t       ParticleMass(Int_t iType);
  static Int_t         ParticleCode(Int_t iType);
#else
  static Float_t       ParticleMass(Int_t iType) {
     if(!fgkParticleMass[0]) Init(); 
     return fgkParticleMass[iType];
  }
  static Int_t         ParticleCode(Int_t iType) 
    {return fgkParticleCode[iType];};
#endif

  static const char*   ParticleName(Int_t iType) 
    {return fgkParticleName[iType];};
  static const char*   ParticleShortName(Int_t iType) 
    {return fgkParticleShortName[iType];};
  static const char*   ParticleLatexName(Int_t iType) 
    {return fgkParticleLatexName[iType];};

  IlcPID();
  IlcPID(const Double_t* probDensity, Bool_t charged = kTRUE);
  IlcPID(const Float_t* probDensity, Bool_t charged = kTRUE);
  IlcPID(const IlcPID& pid);
  IlcPID& operator = (const IlcPID& pid);

  Double_t             GetProbability(EParticleType iType,
				      const Double_t* prior) const;
  Double_t             GetProbability(EParticleType iType) const;
  void                 GetProbabilities(Double_t* probabilities,
					const Double_t* prior) const;
  void                 GetProbabilities(Double_t* probabilities) const;
  EParticleType        GetMostProbable(const Double_t* prior) const;
  EParticleType        GetMostProbable() const;
  
  void                 SetProbabilities(const Double_t* probabilities,
                                        Bool_t charged = kTRUE);

  static void          SetPriors(const Double_t* prior,
				 Bool_t charged = kTRUE);
  static void          SetPrior(EParticleType iType, Double_t prior);

  IlcPID&              operator *= (const IlcPID& pid);

 private:

  static void          Init();

  Bool_t               fCharged;                   // flag for charged/neutral
  Double_t             fProbDensity[kSPECIESN];    // probability densities
  static Double_t      fgPrior[kSPECIESN];         // a priori probabilities

#ifndef __MAKECINT__
	#ifdef WIN32
		__declspec(dllexport)       static /*const*/ Float_t fgkParticleMass[kSPECIESN+kSPECIESLN+1]; // particle masses
		__declspec(dllexport)       static const Int_t   fgkParticleCode[kSPECIESN+kSPECIESLN+1]; // particle codes
	#else
		static /*const*/ Float_t fgkParticleMass[kSPECIESN+kSPECIESLN+1]; // particle masses
		  static const Int_t   fgkParticleCode[kSPECIESN+kSPECIESLN+1]; // particle codes
	#endif
#else
  static /*const*/ Float_t fgkParticleMass[kSPECIESN+kSPECIESLN+1]; // particle masses
#endif  /*__MAKECINT__*/

  static const char*   fgkParticleName[kSPECIESN+kSPECIESLN+1]; // particle names
  static const char*   fgkParticleShortName[kSPECIESN+kSPECIESLN+1]; // particle names
  static const char*   fgkParticleLatexName[kSPECIESN+kSPECIESLN+1]; // particle names

  ClassDef(IlcPID, 2)    // particle id probability densities
};


IlcPID operator * (const IlcPID& pid1, const IlcPID& pid2);


#endif
