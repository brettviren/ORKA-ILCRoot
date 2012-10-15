#ifndef ILCPVBARVFAST_H
#define ILCPVBARVFAST_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPVBARvFast.h 30416 2008-12-16 07:46:03Z fca $ */

/* History of cvs commits:
 *
 * $Log$
 * Revision 1.26  2006/09/13 07:31:01  kharlov
 * Effective C++ corrections (T.Pocheptsov)
 *
 * Revision 1.25  2005/05/28 14:19:05  schutz
 * Compilation warnings fixed by T.P.
 *
 */

//_________________________________________________________________________
// Implementation of the PVBAR manager class for fast simulations     
// Tracks particles until the reach a grossly designed PVBAR module
// Modify the particles property (momentum, energy, type) according to
//  the PVBAR response function. The result is called a virtual reconstructed
//  particle.                                
//                  
//*-- Author: Yves Schutz (SUBATECH)

// --- ROOT system ---
//#include "TClonesArray.h"
#include "TRandom.h"

class TVector3 ;
class TFile;

// --- IlcRoot header files ---
#include "IlcPVBAR.h"
class IlcPVBARGeometry ;
class IlcPVBARFastRecParticle ;

class IlcPVBARvFast : public IlcPVBAR {

public:

  IlcPVBARvFast() ;
  IlcPVBARvFast(const char *name, const char *title="") ;
  
  virtual ~IlcPVBARvFast(void) ;

  void           AddRecParticle(const IlcPVBARFastRecParticle & rp) ; // adds primary particle to the RecParticles list
  virtual void   CreateGeometry(void) ;                              // creates the geometry for GEANT
  Float_t        GetBigBox(Int_t index) const;                             
  virtual void   Init(void) ;                                        // does nothing
  virtual Int_t  IsVersion(void) const {
    // Gives the version number 
    return 99 ; 
  }

  void    MakeBranch(Option_t* opt);
  Double_t MakeEnergy(Double_t energy) ;                       // makes the detected energy    
  TVector3 MakePosition(Double_t energy, TVector3 pos, Double_t th, Double_t ph) ; 
                                                                     // makes the detected position
  void MakeRecParticle(Int_t modid, TVector3 pos, IlcPVBARFastRecParticle & rp) ;  // makes a reconstructes particle from primary
  Int_t   MakeType(IlcPVBARFastRecParticle & rp) ;                    // gets the detected type of particle
  // gets TClonesArray of reconstructed particles
  TClonesArray * FastRecParticles() const { return fFastRecParticles ; } 
  virtual void ResetPoints() ; 
  void         ResetFastRecParticles() ; 
  void         SetBigBox(Int_t index, Float_t value) ;                             
  Double_t     SigmaE(Double_t energy) ;    // calulates the energy resolution at a given Energy                           
  Double_t     SigmaP(Double_t energy, Double_t inc) ; // calulates the position resolution at a given Energy at a given incidence                           
  virtual void StepManager(void) ;          // does the tracking through PVBAR and a preliminary digitalization
  virtual const TString Version(void)const { 
    // As IsVersion
    return TString("vFast") ; 
  }

private:
  IlcPVBARvFast(IlcPVBARvFast & fast);
  IlcPVBARvFast & operator = (const IlcPVBARvFast & );
  
  Float_t fBigBoxX ;                         // main box containing all PVBAR (EMC+PPSD)
  Float_t fBigBoxY ;                         // main box containing all PVBAR (EMC+PPSD)
  Float_t fBigBoxZ ;                         // main box containing all PVBAR (EMC+PPSD)
  TClonesArray * fFastRecParticles ;         // list of particles modified by the response function 
  Int_t fNRecParticles ;                     // number of detected particles
  TRandom fRan ;                             // random number generator
  Double_t fResPara1 ;                       // parameter for the energy resolution dependence  
  Double_t fResPara2 ;                       // parameter for the energy resolution dependence  
  Double_t fResPara3 ;                       // parameter for the energy resolution dependence 
  Double_t fPosParaA0 ;                      // parameter for the position resolution
  Double_t fPosParaA1 ;                      // parameter for the position resolution 
  Double_t fPosParaB0 ;                      // parameter for the position resolution 
  Double_t fPosParaB1 ;                      // parameter for the position resolution 
  Double_t fPosParaB2 ;                      // parameter for the position resolution

  ClassDef(IlcPVBARvFast,1)  //  Implementation of the PVBAR manager class for fast simulations  

};

#endif // IlcPVBARVFAST_H
