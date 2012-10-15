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

/* $Id: IlcGenerator.cxx 51126 2011-08-17 13:37:49Z cvetan $ */

//-----------------------------------------------------------------
//    Generate the final state of the interaction as the input
//    to the MonteCarlo
//    Author: A.Morsch
//-----------------------------------------------------------------
//Begin_Html
/*
<img src="picts/IlcGeneratorClass.gif">
</pre>
<br clear=left>
<font size=+2 color=red>
<p>The responsible person for this module is
<a href="mailto:andreas.morsch@cern.ch">Andreas Morsch</a>.
</font>
<pre>
*/
//End_Html
//                                                               //
///////////////////////////////////////////////////////////////////
#include <TGenerator.h>
#include <TMCProcess.h>

#include "IlcLog.h"
#include "IlcCollisionGeometry.h"
#include "IlcConfig.h"
#include "IlcGenerator.h"
#include "IlcRun.h"
#include "IlcStack.h"
#include "IlcMC.h"
#include "IlcVertexGenerator.h"

ClassImp(IlcGenerator)


//_______________________________________________________________________
IlcGenerator::IlcGenerator():
  TNamed(),
  IlcRndm(),
  fMCEvGen(0),
  fThetaMin(0),
  fThetaMax(0),
  fPhiMin(0),
  fPhiMax(0),
  fPMin(0),
  fPMax(0),
  fPtMin(0),
  fPtMax(0),
  fYMin(0),
  fYMax(0),
  fVMin(3),
  fVMax(3),
  fNpart(0),
  fParentWeight(0),
  fChildWeight(0),
  fAnalog(0),
  fVertexSmear(kNoSmear),
  fVertexSource(kInternal),
  fCutVertexZ(0),
  fPileUpTimeWindow(-1.),
  fTrackIt(0),
  fVertexGenerator(0),
  fOrigin(3),
  fOsigma(3),
  fVertex(3),
  fTimeOrigin(0.),
  fTime(0.),
  fStack(0),
  fContainer(0),
  fCollisionGeometry(0),
  fEnergyCMS(0),
  fAProjectile(0),
  fZProjectile(0),
  fATarget(0),
  fZTarget(0),
  fProjectile(""),
  fTarget("")    
{
  //
  // Default constructor
  //
    if (gIlc) {
	IlcDebug(1, "IlcGenerator Default Constructor");
	IlcMC * mc = gIlc->GetMCApp();
	if (mc) mc->SetGenerator(this);
    }

    SetThetaRange(); ResetBit(kThetaRange);
    SetPhiRange(); ResetBit(kPhiRange);
    SetMomentumRange(); ResetBit(kMomentumRange);
    SetPtRange(); ResetBit(kPtRange);
    SetYRange(); ResetBit(kYRange);
    SetNumberParticles();
    SetTrackingFlag();
    SetCutVertexZ();


    fOrigin[0]=fOrigin[1]=fOrigin[2]=0;
    fOsigma[0]=fOsigma[1]=fOsigma[2]=0;
    fVertex[0]=fVertex[1]=fVertex[2]=0;

    fVMin[0]=fVMin[1]=fVMin[2]=0;
    fVMax[0]=fVMax[1]=fVMax[2]=10000;
}

//_______________________________________________________________________
IlcGenerator::IlcGenerator(Int_t npart):
  TNamed(),
  IlcRndm(),
  fMCEvGen(0),
  fThetaMin(0),
  fThetaMax(0),
  fPhiMin(0),
  fPhiMax(0),
  fPMin(0),
  fPMax(0),
  fPtMin(0),
  fPtMax(0),
  fYMin(0),
  fYMax(0),
  fVMin(3),
  fVMax(3),
  fNpart(0),
  fParentWeight(0),
  fChildWeight(0),
  fAnalog(0),
  fVertexSmear(kNoSmear),
  fVertexSource(kInternal),
  fCutVertexZ(0),
  fPileUpTimeWindow(-1.),
  fTrackIt(0),
  fVertexGenerator(0),
  fOrigin(3),
  fOsigma(3),
  fVertex(3),
  fTimeOrigin(0.),
  fTime(0.),
  fStack(0),
  fContainer(0),
  fCollisionGeometry(0),
  fEnergyCMS(0),
  fAProjectile(0),
  fZProjectile(0),
  fATarget(0),
  fZTarget(0),
  fProjectile(""),
  fTarget("")    
{
  //
  // Standard constructor
  //
    if (gIlc) {
        IlcDebug(1, "IlcGenerator Constructor initializing number of particles");
	IlcMC * mc = gIlc->GetMCApp();
	if (mc) mc->SetGenerator(this);
    }
    
    SetThetaRange(); ResetBit(kThetaRange);
    SetPhiRange(); ResetBit(kPhiRange);
    SetMomentumRange(); ResetBit(kMomentumRange);
    SetPtRange(); ResetBit(kPtRange);
    SetYRange(); ResetBit(kYRange);
    SetTrackingFlag();
    SetCutVertexZ();

    fOrigin[0]=fOrigin[1]=fOrigin[2]=0;
    fOsigma[0]=fOsigma[1]=fOsigma[2]=0;
    fVertex[0]=fVertex[1]=fVertex[2]=0;

    fVMin[0]=fVMin[1]=fVMin[2]=0;
    fVMax[0]=fVMax[1]=fVMax[2]=10000;

    SetNumberParticles(npart);

    IlcConfig::Instance()->Add(this);
}

//_______________________________________________________________________
IlcGenerator::~IlcGenerator()
{
  //
  // Destructor
  //
  fOrigin.Set(0);
  fOsigma.Set(0);
  if (fMCEvGen) {
    delete fMCEvGen;
    fMCEvGen=0;
  }
}

//_______________________________________________________________________
void IlcGenerator::Init()
{   
  //
  // Dummy initialisation
  //
}

//_______________________________________________________________________
void IlcGenerator::SetOrigin(Float_t ox, Float_t oy, Float_t oz)
{
  //
  // Set the vertex for the generated tracks
  //
  fOrigin[0]=ox;
  fOrigin[1]=oy;
  fOrigin[2]=oz;
}

//_______________________________________________________________________
void IlcGenerator::SetOrigin(const TLorentzVector &o)
{
  //
  // Set the vertex for the generated tracks
  //
  fOrigin[0]=o[0];
  fOrigin[1]=o[1];
  fOrigin[2]=o[2];
}

//_______________________________________________________________________
void IlcGenerator::SetSigma(Float_t sx, Float_t sy, Float_t sz)
{
  //
  // Set the spread of the vertex
  //
  fOsigma[0]=sx;
  fOsigma[1]=sy;
  fOsigma[2]=sz;
}

//_______________________________________________________________________
void IlcGenerator::SetMomentumRange(Float_t pmin, Float_t pmax)
{
  //
  // Set the momentum range for the generated particles
  //
  fPMin = pmin;
  fPMax = pmax;
  SetBit(kMomentumRange);
}

//_______________________________________________________________________
void IlcGenerator::SetPtRange(Float_t ptmin, Float_t ptmax)
{
  //
  // Set the Pt range for the generated particles
  //
  fPtMin = ptmin;
  fPtMax = ptmax;
  SetBit(kPtRange);
}

//_______________________________________________________________________
void IlcGenerator::SetPhiRange(Float_t phimin, Float_t phimax)
{
  //
  // Set the Phi range for the generated particles
  //
  fPhiMin = TMath::Pi()*phimin/180;
  fPhiMax = TMath::Pi()*phimax/180;
  SetBit(kPhiRange);
}

//_______________________________________________________________________
void IlcGenerator::SetYRange(Float_t ymin, Float_t ymax)
{
  //
  // Set the Rapidity range for the generated particles
  //
  fYMin=ymin;
  fYMax=ymax;
  SetBit(kYRange);
}

//_______________________________________________________________________
void IlcGenerator::SetVRange(Float_t vxmin, Float_t vxmax,
			     Float_t vymin, Float_t vymax,
			     Float_t vzmin, Float_t vzmax)
{
  //
  // Set the vertex range for the generated particles
  //
  fVMin[0]=vxmin; fVMin[1]=vymin; fVMin[2]=vzmin;
  fVMax[0]=vxmax; fVMax[1]=vymax; fVMax[2]=vzmax;
  SetBit(kVertexRange);
}

//_______________________________________________________________________
void IlcGenerator::SetThetaRange(Float_t thetamin, Float_t thetamax)
{
  //
  // Set the theta range for the generated particles
  //
  fThetaMin = TMath::Pi()*thetamin/180;
  fThetaMax = TMath::Pi()*thetamax/180;
  SetBit(kThetaRange);
}

void IlcGenerator::Vertex()
{
  //
  // Obtain vertex for current event from external source or calculated (internal)
  //
    if (fVertexSource == kInternal) {
	VertexInternal();
    } else if (fVertexSource == kContainer) {
	;
    } else if (fVertexSource == kExternal) {
	VertexExternal();
    }
}

//_______________________________________________________________________
void IlcGenerator::VertexExternal()
{
    //
    // Obtain vertex from external source (vertex generator)
    //
    TVector3 vertex = fVertexGenerator->GetVertex();
    fVertex[0] = vertex.X();
    fVertex[1] = vertex.Y();
    fVertex[2] = vertex.Z();
    fTime = 0.;
}

//_______________________________________________________________________
void IlcGenerator::VertexInternal()
{
    // 
    // Obtain calculated vertex 
    // Default is gaussian smearing
    Float_t random[6];
    Float_t dv[3];
    Int_t j;
    dv[2] = 1.e10;
    if (!TestBit(kVertexRange)) {
	while(TMath::Abs(dv[2]) > fCutVertexZ*fOsigma[2]) {
	    Rndm(random,6);
	    for (j=0; j < 3; j++) {
		dv[j] = fOsigma[j]*TMath::Cos(2*random[2*j]*TMath::Pi())*
		    TMath::Sqrt(-2*TMath::Log(random[2*j+1]));
	    }
	}
	for (j=0; j < 3; j++) fVertex[j] = fOrigin[j] + dv[j];

	// In case of gaussian smearing we smear also the event/collision time
	Rndm(random,2);
	fTime = fTimeOrigin + fOsigma[2]/TMath::Ccgs()*
	  TMath::Cos(2*random[0]*TMath::Pi())*
	  TMath::Sqrt(-2*TMath::Log(random[1]));
	
    } else {
	Rndm(random,3);
	for (j=0; j < 3; j++)
	    fVertex[j] =  fVMin[j] + random[j] * (fVMax[j] - fVMin[j]);
	fTime = 0.;
    }
}

//_______________________________________________________________________
void  IlcGenerator::PushTrack(Int_t done, Int_t parent, Int_t pdg,
                               Float_t *pmom, Float_t *vpos, Float_t *polar,
                               Float_t tof, TMCProcess mech, Int_t &ntr,
                               Float_t weight, Int_t is)
{
  //
  // Loads one track on the stack
  //
    
  if (fStack)
    fStack->PushTrack(done, parent, pdg, pmom, vpos, polar, tof,
                     mech, ntr, weight, is);
  else 
    gIlc->GetMCApp()->PushTrack(done, parent, pdg, pmom, vpos, polar, tof,
                     mech, ntr, weight, is);
}

//_______________________________________________________________________
void  IlcGenerator::PushTrack(Int_t done, Int_t parent, Int_t pdg,
                      Double_t px, Double_t py, Double_t pz, Double_t e,
                      Double_t vx, Double_t vy, Double_t vz, Double_t tof,
                      Double_t polx, Double_t poly, Double_t polz,
                      TMCProcess mech, Int_t &ntr, Float_t weight, Int_t is)
{
  //
  // Loads one track on the stack
  //
  
  if (fStack)
     fStack->PushTrack(done, parent, pdg, px, py, pz, e, vx, vy, vz, tof,
                      polx, poly, polz, mech, ntr, weight, is);
  else 
     gIlc->GetMCApp()->PushTrack(done, parent, pdg, px, py, pz, e, vx, vy, vz, tof,
                        polx, poly, polz, mech, ntr, weight, is);
}


//_______________________________________________________________________
void IlcGenerator:: KeepTrack(Int_t itrack)
{
  //
  // Declare a track permanent on the stack
  //
  if (fStack)
     fStack->KeepTrack(itrack);
  else 
     gIlc->GetMCApp()->KeepTrack(itrack);
   
}

//_______________________________________________________________________
void IlcGenerator:: SetHighWaterMark(Int_t nt)
{
  //
  // Internal function to set the maximum index used in the stack
  //
  if (fStack)
     fStack->SetHighWaterMark(nt);
  else 
     gIlc->GetMCApp()->SetHighWaterMark(nt);
   
}
void IlcGenerator::FinishRun()
{
    ;
}
