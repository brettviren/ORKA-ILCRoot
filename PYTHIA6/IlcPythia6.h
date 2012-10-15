#ifndef ILCPYTHIA6_H
#define ILCPYTHIA6_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPythia.h,v 1.22 2007/10/09 08:43:24 morsch Exp $ */

#include <TPythia6.h>
#include "IlcPythiaBase.h"


class IlcFastGlauber;
class IlcQuenchingWeights;
class IlcStack;

class IlcPythia6 : public TPythia6, public IlcPythiaBase
{

 public:
    virtual ~IlcPythia6(){;}
    virtual Int_t Version() {return (6);}
    // convert to compressed code and print result (for debugging only)
    virtual Int_t CheckedLuComp(Int_t kf);
    // Pythia initialisation for selected processes
    virtual void ProcInit
	(Process_t process, Float_t energy, StrucFunc_t strucfunc);
    virtual void  GenerateEvent()   {Pyevnt();}
    virtual void  GenerateMIEvent() {Pyevnw();}
    virtual void  HadronizeEvent()  {Pyexec();}
    virtual Int_t GetNumberOfParticles() {return GetN();}
    virtual void  SetNumberOfParticles(Int_t i) {SetN(i);}
    virtual void  EditEventList(Int_t i) {Pyedit(i);}
    virtual void  PrintStatistics();
    virtual void  EventListing();    
    virtual Int_t GetParticles(TClonesArray *particles) {return ImportParticles(particles, "All");}
    // Treat protons as inside nuclei
    virtual void  SetNuclei(Int_t a1, Int_t a2);
    // Print particle properties
    virtual void PrintParticles();
    // Reset the decay table
    virtual void ResetDecayTable();
    //
    // Common Physics Configuration
    virtual void SetPtHardRange(Float_t ptmin, Float_t ptmax);
    virtual void SetYHardRange(Float_t ymin, Float_t ymax);
    virtual void SetFragmentation(Int_t flag);
    virtual void SetInitialAndFinalStateRadiation(Int_t flag1, Int_t flag2);
    virtual void SetIntrinsicKt(Float_t kt);
    virtual void SwitchHFOff();
    virtual void SetPycellParameters(Float_t etamax, Int_t neta, Int_t nphi,
				     Float_t thresh, Float_t etseed, Float_t minet, Float_t r);
    virtual void ModifiedSplitting();
    virtual void SwitchHadronisationOff();
    virtual void SwitchHadronisationOn();
    //
    // Common Getters
    virtual void    GetXandQ(Float_t& x1, Float_t& x2, Float_t& q);
    virtual Float_t GetXSection();
    virtual Int_t   ProcessCode();
    virtual Float_t GetPtHard();
    //
    //
    virtual void SetDecayTable();
    virtual void Pyevnw();
    virtual void Pyjoin(Int_t& npart, Int_t* ipart);
    virtual void Pycell(Int_t& nclus);
    virtual void Pyclus(Int_t& nclus);
    virtual void GetJet(Int_t i, Float_t& px, Float_t& py, Float_t& pz, Float_t& e);
    virtual void LoadEvent(IlcStack* stack, Int_t flag, Int_t reHadr);
    virtual void Pyshow(Int_t ip1, Int_t ip2, Double_t qmax);
    virtual void Pyshowq(Int_t ip1, Int_t ip2, Double_t qmax);

    virtual void Pyrobo(Int_t imi, Int_t ima, Double_t the, Double_t phi, Double_t bex, Double_t bey, Double_t bez);
    virtual void InitQuenching(Float_t bmin, Float_t bmax, Float_t k, Int_t iECMethod, Float_t zmax = 0.97, Int_t ngmax = 30);
    virtual void SetPyquenParameters(Double_t t0, Double_t tau0, Int_t nf, Int_t iengl, Int_t iangl);
    virtual void Pyquen(Double_t a, Int_t ibf, Double_t b);
    virtual void Qpygin0();
    virtual void GetQuenchingParameters(Double_t& xp, Double_t& yp, Double_t z[4]);
    // return instance of the singleton
    static  IlcPythia6* Instance();
    virtual void Quench();
    // Assignment Operator
    IlcPythia6 & operator=(const IlcPythia6 & rhs);
    void Copy(TObject&) const;
 protected:
    Process_t             fProcess;           // Process type
    Float_t               fEcms;              // Centre of mass energy
    StrucFunc_t           fStrucFunc;         // Structure function
    Int_t                 fDefMDCY[501];      //  ! Default decay switches per particle
    Int_t                 fDefMDME[2001];     //  ! Default decay switches per mode
    Double_t              fZQuench[4];        //  ! Quenching fractions for this even
    Double_t              fXJet;              //  ! Jet production point X
    Double_t              fYJet;              //  ! Jet production point Y
    Int_t                 fNGmax;             //    Maximum number of radiated gluons in quenching
    Float_t               fZmax;              //    Maximum energy loss in quenching
    IlcFastGlauber*       fGlauber;           //  ! The Glauber model
    IlcQuenchingWeights*  fQuenchingWeights;  //  ! The Quenching Weights model
    static IlcPythia6*    fgIlcPythia;        //  Pointer to single instance
 private: 
    IlcPythia6();
    IlcPythia6(const IlcPythia6& pythia);
    void ConfigHeavyFlavor();
    void AtlasTuning();
    void AtlasTuningMC09();
    ClassDef(IlcPythia6,1) //ILC UI to PYTHIA
};

#endif





