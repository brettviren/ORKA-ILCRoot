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
 *                                                                        *
 **************************************************************************/

/* $Id: IlcGenTPHIC.cxx 51126 2011-08-17 13:37:49Z cvetan $ */

// Event generator of two-photon processes
// in ultra-peripheral ion collisions.
// 5 two-photon process are implemented, see comments to SetProcess().
//%
// The example of the generator initialization for the process
// gamma gamma -> X in CaCa collisions at 7000 TeV/nucleon is the following:
//%
//    IlcGenTPHIC *gener = new IlcGenTPHIC();
//    gener->SetProcess(1);
//    gener->SetBeamEnergy(3500.);
//    gener->SetBeamZ(20);
//    gener->SetBeamA(40);
//    gener->SetMggRange(70.,200.);
//    gener->SetYggRange(-5.,5.);
//    gener->SetLumFunName("lum_ca_70_200.dat");
//    gener->SetLumFunFlag(-1);
//    gener->Init();
//%
// The two-photon luminosity function needed for the process cross section
// calculation is time-consuming, therefore it can be calculated once for a
// selected two-photon energy and rapidity range and selected ion species,
// saved into a file and then can be reused in further calculation.
//%
// The integral cross section of the process is calculated in each event
// by the Monte Carlo integration procedure, the differential cross section
// of each thrown event is assigned as a weight to each track of the event
// which can be then used during the event analysis by retreiving this weight
// from any of the event tracks.
// 
// The manual of the fortran generator is available in
// $ILC_ROOT/TPHIC/TPHIC_doc.ps.gz
// For the two-photon physics in heavy ion collisions see
// G.Baur et al, Phys.Rep. 364 (2002), 359.
//%
// Author: Yuri.Kharlov@cern.ch
// 15 April 2003

#include <TParticle.h>
#include <TParticlePDG.h>
#include <TDatabasePDG.h>
#include <TClonesArray.h>

#include "IlcPythia.h"
#include "IlcRun.h"
#include <IlcGenTPHIC.h>
#include <TPHICgen.h>
#include <TPHICcommon.h>

ClassImp(IlcGenTPHIC)

//------------------------------------------------------------

IlcGenTPHIC::IlcGenTPHIC() :
  IlcGenMC(),
  fTPHICgen(0x0),
  fPythia(0x0),
  fParticles(0x0),
  fEvent(-1),
  fDebug(0),
  fDebugEventFirst(-1),
  fDebugEventLast(-1)
{
  // Constructor: create generator instance,
  // create particle array
  // Set TPHIC parameters to default values:
  // eta_b production in Ca-Ca collisions at 7 A*TeV

  SetMC(new TPHICgen());
  fPythia = IlcPythia::Instance();
  fParticles = new TClonesArray("TParticle",100);

  SetProcess   ();
  SetBeamEnergy();
  SetBeamZ     ();
  SetBeamA     ();
  SetYggRange  ();
  SetMggRange  ();
  SetNgridY    ();
  SetNgridM    ();
  SetLumFunName();
  SetLumFunFlag();
  SetKfOnium   ();
}

//____________________________________________________________
IlcGenTPHIC::~IlcGenTPHIC()
{
  // Destroys the object, deletes and disposes all TParticles currently on list.
  if (fParticles) {
    fParticles->Delete();
    delete fParticles;
    fParticles = 0;
  }
}

//____________________________________________________________
void IlcGenTPHIC::Init()
{
  // Initialize the generator TPHIC

  fTPHICgen->Initialize();
  fEvent = 0;
}

//____________________________________________________________
void IlcGenTPHIC::Generate()
{
  // Generate one event of two-photon process.
  // Gaussian smearing on the vertex is done if selected. 
  // All particles from the TPHIC/PYTHIA event listing
  // are stored in TreeK, and only final-state particles are tracked.
  // The event differectial cross section is assigned as a weight
  // to each track of the event.

  Float_t polar[3]   = {0,0,0};
  Float_t origin0[3] = {0,0,0};
  Float_t time0 = 0.;
  Float_t origin[3]  = {0,0,0};
  Float_t p[3], tof;
  Double_t weight;

  Int_t    ks,kf,iparent,nt, trackIt;
  Float_t  random[6];
  const Float_t kconv=0.001/2.999792458e8;

  fTPHICgen->GenerateEvent();
  weight = GetXSectionCurrent();
  if (gIlc->GetEvNumber()>=fDebugEventFirst &&
      gIlc->GetEvNumber()<=fDebugEventLast) fPythia->Pylist(1);
  fPythia->ImportParticles(fParticles,"All");

  if (fDebug == 1)
    Info("Generate()","one event is produced");

  Int_t j;
  for (j=0;j<3;j++) origin0[j]=fOrigin[j];
  time0 = fTimeOrigin;
  if(fVertexSmear==kPerEvent) {
    Rndm(random,6);
    for (j=0;j<3;j++) {
      origin0[j]+=fOsigma[j]*TMath::Cos(2*random[2*j]*TMath::Pi())*
	TMath::Sqrt(-2*TMath::Log(random[2*j+1]));
    }
    Rndm(random,2);
    time0 += fOsigma[2]/TMath::Ccgs()*
      TMath::Cos(2*random[0]*TMath::Pi())*
      TMath::Sqrt(-2*TMath::Log(random[1]));
  }

  Int_t ip;
  Int_t np = fParticles->GetEntriesFast();
  TParticle *iparticle;
//   Int_t* pParent   = new Int_t[np];
  for (ip=0; ip<np; ip++) {
    iparticle = (TParticle *) fParticles->At(ip);
    ks = iparticle->GetStatusCode();
//     // No initial state partons
//     if (ks==21) continue;
    p[0] = iparticle->Px();
    p[1] = iparticle->Py();
    p[2] = iparticle->Pz();
    origin[0] = origin0[0]+iparticle->Vx()/10.;
    origin[1] = origin0[1]+iparticle->Vy()/10.;
    origin[2] = origin0[2]+iparticle->Vz()/10.;
    kf = CheckPDGCode(iparticle->GetPdgCode());
    iparent = -1;
    tof     = time0 + kconv*iparticle->T();
    if (ks == 1) trackIt = 1;
    else         trackIt = 0;
    PushTrack(fTrackIt*trackIt,iparent,kf,p,origin,polar,tof,kPPrimary,nt,weight,ks);
    KeepTrack(nt); 

    if (fDebug == 2)
      printf("id=%+4d, parent=%3d, ks=%d, p = (%+11.4e,%+11.4e,%+11.4e) GeV\n",
	     kf,iparent,fTrackIt*trackIt,p[0],p[1],p[2]);
  }
  fEvent++;
  fTPHICgen->SetNEVENT(fEvent);
  if (fDebug == 1 && fEvent%100 == 0) {
    Info("Generate","Event %d\n",fEvent);
    fTPHICgen->Finish();
  }
}

//____________________________________________________________
void IlcGenTPHIC::SetEventListRange(Int_t eventFirst, Int_t eventLast)
{
  // Set a range of event numbers, for which a table
  // of generated particle will be printed
  fDebugEventFirst = eventFirst;
  fDebugEventLast  = eventLast;
  if (fDebugEventLast==-1) fDebugEventLast=fDebugEventFirst;
}

//____________________________________________________________
void IlcGenTPHIC::SetProcess       (Int_t   proc  )
{
  // Set process number:
  // proc=1 - gamma gamma -> X
  // proc=2 - gamma gamma -> quarkonium
  // proc=3 - gamma gamma -> fermion+ fermion-
  // proc=4 - gamma gamma -> W+ W-
  // proc=5 - not implemented
  // proc=6 - gamma gamma -> V1 V2 (vector meson pair)

  fTPHICgen->SetIPROC(proc);
}
//____________________________________________________________
  void IlcGenTPHIC::SetBeamEnergy  (Float_t energy)
{
  // Set energy of the beam ion per nucleon in GeV
  fTPHICgen->SetEBMN(energy);
}
//____________________________________________________________
  void IlcGenTPHIC::SetBeamZ       (Int_t   z     )
{
  // Set beam ion charge
  fTPHICgen->SetIZ(z);
}
//____________________________________________________________
  void IlcGenTPHIC::SetBeamA       (Int_t   a     )
{
  // Set beam ion atomic number
  fTPHICgen->SetIA(a);
}
//____________________________________________________________
  void IlcGenTPHIC::SetYggRange    (Float_t ymin, Float_t ymax)
{
  // Set rapidity range of 2-photon system for the
  // luminosity function calculation
  fTPHICgen->SetYMIN(ymin);
  fTPHICgen->SetYMAX(ymax);
}
//____________________________________________________________
  void IlcGenTPHIC::SetMggRange    (Float_t mmin, Float_t mmax)
{
  // Set invariant mass range of 2-photon system for the
  // luminosity function calculation
  fTPHICgen->SetAMIN(mmin);
  fTPHICgen->SetAMAX(mmax);
}
//____________________________________________________________
  void IlcGenTPHIC::SetNgridY      (Int_t   ny    )
{
  // Set number of nodes on the grid along the rapidity axis
  // to calculate the 2-photon luminosity function
  fTPHICgen->SetNY(ny);
}
//____________________________________________________________
  void IlcGenTPHIC::SetNgridM      (Int_t   nm    )
{
  // Set number of nodes on the grid along the mass axis
  // to calculate the 2-photon luminosity function
  fTPHICgen->SetNMAS(nm);
}
//____________________________________________________________
  void IlcGenTPHIC::SetLumFunName  (TString name  )
{
  // Set filename to store the 2-photon luminosity
  // function calculated on the grid
  fTPHICgen->SetLUMFIL(name);
}
//____________________________________________________________
  void IlcGenTPHIC::SetLumFunFlag  (Int_t   flag  )
{
  // Set flag to calculate the 2-photon luminosity function:
  // flag=-1 if a new lumimosity function to be calculated
  //         and stored in a file
  // flag=+1 if a previously calculated function to be read
  //         from a file
  fTPHICgen->SetILUMF(flag);
}
//____________________________________________________________
  void IlcGenTPHIC::SetKfFermion   (Int_t   kf    )
{
  // Set a PDG flavour code of a fermion for the process 3,
  // gamma gamma -> fermion+ fermion-
  fTPHICgen->SetKFERM(kf);
}
//____________________________________________________________
  void IlcGenTPHIC::SetKfOnium    (Int_t   kf    )
{
  // Set a PDG flavour code of a quarkonium for the process 2,
  // gamma gamma -> quarkonium
  fTPHICgen->SetKFONIUM(kf);
}
//____________________________________________________________
  void IlcGenTPHIC::SetMassOnium   (Float_t mass  )
{
  // Set a quarkonium mass [GeV] for the process 2 if it
  // differes from the Pythia's particle table.
  // For the well-known quarkonia no need to set this mass
  // because it will be taken from the Pythia table
  fTPHICgen->SetXMRES(mass);
}
//____________________________________________________________
  void IlcGenTPHIC::SetGGwidthOnium(Float_t width )
{
  // Set 2-photon partial width [GeV] of the quarkonium for
  // the process 2 if it differes fromthe Pythia's particle table.
  // For the well-known quarkonia no need to set this width
  // because it will be taken from the Pythia table
  fTPHICgen->SetXGGRES(width);
}
//____________________________________________________________
  void IlcGenTPHIC::SetKfVmesons   (Int_t kf1, Int_t kf2)
{
  // Set PDG flavour codes of the two vector vesons
  // for the process 2:  gamma gamma -> V1 V2
  // So far this processes is implemented for the following
  // mesons and their combinations only:
  // pho0 (113), omega (223), phi (333), J/psi (443)
  fTPHICgen->SetKV1(kf1);
  fTPHICgen->SetKV2(kf2);
}
//____________________________________________________________
  Float_t IlcGenTPHIC::GetGGmass    ()
{
  // Get invariant mass of generated 2-photon system
  return fTPHICgen->GetWSQ();
}
//____________________________________________________________
  Float_t IlcGenTPHIC::GetGGrapidity()
{
  // Get rapidity of generated 2-photon system
  return fTPHICgen->GetYGG();
}
//____________________________________________________________
  Float_t IlcGenTPHIC::GetG1mass    ()
{
  // Get a mass of the first virtual photon of
  // the 2-photon process (-sqrt(q1^2)).
  return fTPHICgen->GetXMG1();
}
//____________________________________________________________
  Float_t IlcGenTPHIC::GetG2mass    ()
{
  // Get a mass of the second virtual photon of
  // the 2-photon process (-sqrt(q2^2)).
  return fTPHICgen->GetXMG2();
}
//____________________________________________________________
  TClonesArray*  IlcGenTPHIC::GetParticleList ()
{
  // Get array of particles of the event listing
  return fParticles;
}
//____________________________________________________________
  TLorentzVector IlcGenTPHIC::MomentumRecNucl1()
{
  // Get 4-momentum of the first recoil nucleus after
  // the 2-photon process.
  return TLorentzVector(fTPHICgen->GetPTAG1(0),
			fTPHICgen->GetPTAG1(1),
			fTPHICgen->GetPTAG1(2),
			fTPHICgen->GetPTAG1(3));
}
//____________________________________________________________
  TLorentzVector IlcGenTPHIC::MomentumRecNucl2()
{
  // Get 4-momentum of the first recoil nucleus after
  // the 2-photon process.
  return TLorentzVector(fTPHICgen->GetPTAG2(0),
			fTPHICgen->GetPTAG2(1),
			fTPHICgen->GetPTAG2(2),
			fTPHICgen->GetPTAG2(3));
}
//____________________________________________________________
  Float_t IlcGenTPHIC::GetXSectionCurrent()
{
  // Get the cross section of the produced event for the
  // Monte Carlo integral cross section calculation
  return fTPHICgen->GetXSCUR();
}
//____________________________________________________________
  Float_t IlcGenTPHIC::GetXSection       ()
{
  // Get the integral cross section of the process
  // calculated so far
  return fTPHICgen->GetXSTOT();
}
//____________________________________________________________
  Float_t IlcGenTPHIC::GetXSectionError  ()
{
  // Get the error of the integral cross section of the process
  // calculated so far
  return fTPHICgen->GetXSTOTE();
}
