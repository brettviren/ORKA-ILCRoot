#ifndef ILCPYTHIA_H
#define ILCPYTHIA_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPythia.h 54863 2012-02-28 12:10:29Z hristov $ */

#include <TPythia6.h>
#include <IlcRndm.h>
#include <IlcStructFuncType.h>
#include "PythiaProcesses.h"
#include "IlcOmegaDalitz.h"
class IlcFastGlauber;
class IlcQuenchingWeights;

class IlcPythia : public TPythia6, public IlcRndm
{

 public:
    virtual ~IlcPythia(){;}
    // convert to compressed code and print result (for debugging only)
    virtual Int_t CheckedLuComp(Int_t kf);
    // Pythia initialisation for selected processes
    virtual void ProcInit
	(Process_t process, Float_t energy, StrucFunc_t strucfunc, Int_t itune = -1);
    // treat protons as inside nuclei
    virtual void  SetNuclei(Int_t a1, Int_t a2, Int_t pdf);
    // Print particle properties
    virtual void PrintParticles();
    virtual void ResetDecayTable();
    virtual void SetDecayTable();
    virtual void Pyevnw();
    virtual void Pycell(Int_t& nclus);
    virtual void Pyclus(Int_t& nclus);
    virtual void Pyshow(Int_t ip1, Int_t ip2, Double_t qmax);
    virtual void Pyshowq(Int_t ip1, Int_t ip2, Double_t qmax);
    virtual void Pyrobo(Int_t imi, Int_t ima, Double_t the, Double_t phi, Double_t bex, Double_t bey, Double_t bez);
    virtual void Pytune(Int_t itune);
    virtual void Py2ent(Int_t idx, Int_t pdg1, Int_t pdg2, Double_t p);
    virtual void InitQuenching(Float_t bmin, Float_t bmax, Float_t k, Int_t iECMethod, Float_t zmax = 0.97, Int_t ngmax = 30);
    virtual void SetPyquenParameters(Double_t t0, Double_t tau0, Int_t nf, Int_t iengl, Int_t iangl);
    virtual void Pyquen(Double_t a, Int_t ibf, Double_t b);
    virtual void Qpygin0();
    virtual void GetQuenchingParameters(Double_t& xp, Double_t& yp, Double_t z[4]);
    // return instance of the singleton
    static  IlcPythia* Instance();
    virtual void Quench();
    void DalitzDecays();
    // Assignment Operator
    IlcPythia & operator=(const IlcPythia & rhs);
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
    Int_t                 fItune;             //  ! Pythia tune 
    IlcOmegaDalitz        fOmegaDalitz;       //  ! omega dalitz decayer
    static IlcPythia*     fgIlcPythia;        // Pointer to single instance
 private: 
    IlcPythia();
    IlcPythia(const IlcPythia& pythia);
    void ConfigHeavyFlavor();
    void AtlasTuning();
    void AtlasTuningMC09();
    ClassDef(IlcPythia,1) //ILC UI to PYTHIA
};

#endif





