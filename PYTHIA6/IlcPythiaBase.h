#ifndef ILCPYTHIABASE_H
#define ILCPYTHIABASE_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id$ */
#include "IlcRndm.h"
#include <TObject.h>
#include "IlcStructFuncType.h"
#include "PythiaProcesses.h"

class IlcFastGlauber;
class IlcQuenchingWeights;
class IlcStack;
class TClonesArray;

class IlcPythiaBase : public IlcRndm 
{

 public:
    IlcPythiaBase();
    virtual ~IlcPythiaBase(){;}
    virtual Int_t Version()                                                                                         = 0;
    // Convert to compressed code and print result (for debugging only)
    virtual Int_t CheckedLuComp(Int_t kf)                                                                           = 0;
    // Pythia initialisation for selected processes
    virtual void  ProcInit (Process_t process, Float_t energy, StrucFunc_t strucfunc)                               = 0;
    virtual void  GenerateEvent()                                                                                   = 0;
    virtual void  GenerateMIEvent()                                                                                 = 0;
    virtual Int_t GetNumberOfParticles()                                                                            = 0;
    virtual void  SetNumberOfParticles(Int_t i)                                                                     = 0;
    virtual void  EditEventList(Int_t i)                                                                            = 0;
    virtual void  HadronizeEvent()                                                                                  = 0;
    virtual Int_t GetParticles(TClonesArray *particles)                                                             = 0;
    virtual void  PrintStatistics()                                                                                 = 0;
    virtual void  EventListing()                                                                                    = 0;    
    // Treat protons as inside nuclei
    virtual void  SetNuclei(Int_t a1, Int_t a2)                                                                     = 0;
    // Print particle properties
    virtual void PrintParticles()                                                                                   = 0;
    // Reset the decay table
    virtual void ResetDecayTable()                                                                                  = 0;
    //
    // Common Physics Configuration
    virtual void SetPtHardRange(Float_t ptmin, Float_t ptmax)                                                       = 0;
    virtual void SetYHardRange(Float_t ymin, Float_t ymax)                                                          = 0;
    virtual void SetFragmentation(Int_t flag)                                                                       = 0;
    virtual void SetInitialAndFinalStateRadiation(Int_t flag1, Int_t flag2)                                         = 0;
    virtual void SetIntrinsicKt(Float_t kt)                                                                         = 0;
    virtual void SwitchHFOff()                                                                                      = 0;
    virtual void SetPycellParameters(Float_t etamax, Int_t neta, Int_t nphi,
				     Float_t thresh, Float_t etseed, Float_t minet, Float_t r)                      = 0;
    virtual void ModifiedSplitting()                                                                                = 0;
    virtual void SwitchHadronisationOff()                                                                           = 0;
    virtual void SwitchHadronisationOn()                                                                            = 0;
    //
    // Common Getters
    virtual void    GetXandQ(Float_t& x1, Float_t& x2, Float_t& q)                                                  = 0;
    virtual Float_t GetXSection()                                                                                   = 0;
    virtual Int_t   ProcessCode()                                                                                   = 0;
    virtual Float_t GetPtHard()                                                                                     = 0;
    //
    //
    virtual void SetDecayTable()                                                                                    = 0;
    virtual void Pycell(Int_t& nclus)                                                                               = 0;
    virtual void Pyclus(Int_t& nclus)                                                                               = 0;
    virtual void GetJet(Int_t i, Float_t& px, Float_t& py, Float_t& pz, Float_t& e)                                 = 0;
    virtual void LoadEvent(IlcStack* stack, Int_t flag, Int_t reHadr)                                               = 0;
    virtual void Pyshow(Int_t ip1, Int_t ip2, Double_t qmax)                                                        = 0;
    virtual void Pyrobo(Int_t imi, Int_t ima, Double_t the, Double_t phi, Double_t bex, Double_t bey, Double_t bez) = 0;
    virtual void InitQuenching(Float_t bmin, Float_t bmax, Float_t k, Int_t iECMethod, Float_t zmax, Int_t ngmax)   = 0;
    virtual void Pyquen(Double_t a, Int_t ibf, Double_t b)                                                          = 0;
    virtual void GetQuenchingParameters(Double_t& xp, Double_t& yp, Double_t z[4])                                  = 0;
    // return instance of the singleton
    virtual void Quench()                                                                                           = 0;
    virtual void ConfigHeavyFlavor()                                                                                = 0;
    virtual void AtlasTuning()                                                                                      = 0;
    ClassDef(IlcPythiaBase, 1) //ILC UI to PYTHIA
};

#endif





