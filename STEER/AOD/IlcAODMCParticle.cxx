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


//-------------------------------------------------------------------------
//     Realisation of IlcVParticle for MC Particles
//     Basically a stripped down IlcMCParicle / TParticle
//     with minimum information on MC tracks
//     Author: Christian Klein-Bösing, CERN
//-------------------------------------------------------------------------


#include "IlcAODMCParticle.h"
#include "IlcAODEvent.h"

#include "TDatabasePDG.h"
#include "TParticle.h"
#include "TClonesArray.h"


ClassImp(IlcAODMCParticle)

TString IlcAODMCParticle::fgkStdBranchName("mcparticles");

IlcAODMCParticle::IlcAODMCParticle():
IlcVParticle(),
  fPdgCode(0),
  fFlag(0),
  fLabel(0),
  fMother(0),
  fPx(0),
  fPy(0),
  fPz(0),
  fE(0),
  fVx(0),
  fVy(0),
  fVz(0),
  fVt(0)
{
  // Default Constructor
  fDaughter[0] =   fDaughter[1] = 0;
}

    
IlcAODMCParticle::IlcAODMCParticle(const IlcMCParticle* mcpart, Int_t label,Int_t flag):
    IlcVParticle(*mcpart),
    fPdgCode(mcpart->Particle()->GetPdgCode()),
    fFlag(flag),
    fLabel(label),
    fMother(mcpart->GetMother()),
    fPx(mcpart->Particle()->Px()),
    fPy(mcpart->Particle()->Py()),
    fPz(mcpart->Particle()->Pz()),
    fE(mcpart->Particle()->Energy()),
    fVx(mcpart->Particle()->Vx()),
    fVy(mcpart->Particle()->Vy()),
    fVz(mcpart->Particle()->Vz()),
    fVt(mcpart->Particle()->T())
{
    // Constructor
  fDaughter[0] =  mcpart->GetFirstDaughter(); 
  fDaughter[1] =  mcpart->GetLastDaughter();
}
    
    
IlcAODMCParticle::IlcAODMCParticle(const IlcAODMCParticle& mcPart) :
    IlcVParticle(mcPart),
    fPdgCode(mcPart.fPdgCode),
    fFlag(mcPart.fFlag),
    fLabel(mcPart.fLabel),
    fMother(mcPart.fMother),
    fPx(mcPart.fPx),
    fPy(mcPart.fPy),
    fPz(mcPart.fPz),
    fE(mcPart.fE),
    fVx(mcPart.fVx),
    fVy(mcPart.fVy),
    fVz(mcPart.fVz),
    fVt(mcPart.fVt)
{
  // Copy constructor
  fDaughter[0] = mcPart.fDaughter[0]; 
  fDaughter[1] = mcPart.fDaughter[1]; 
}

IlcAODMCParticle& IlcAODMCParticle::operator=(const IlcAODMCParticle& mcPart)
{ 
  //
  // assignment operator
  //
  if (this!=&mcPart) { 
    IlcVParticle::operator=(mcPart);
    fPdgCode    = mcPart.fPdgCode;
    fFlag       = mcPart.fFlag;
    fLabel      = mcPart.fLabel;
    fMother     = mcPart.fMother;
    fPx         = mcPart.fPx;
    fPy         = mcPart.fPy;
    fPz         = mcPart.fPz;
    fE          = mcPart.fE;
    fVx         = mcPart.fVx;
    fVy         = mcPart.fVy;
    fVz         = mcPart.fVz;
    fVt         = mcPart.fVt;
    fDaughter[0] = mcPart.fDaughter[0]; 
    fDaughter[1] = mcPart.fDaughter[1]; 
  }  
  
  return *this;

}

Double_t IlcAODMCParticle::M()         const
{
  //
  // return the mass 
  //
    TParticlePDG* pdg =  TDatabasePDG::Instance()->GetParticle(fPdgCode);
    if (pdg) {
	return (pdg->Mass());
    } else {
	return GetCalcMass();
    }
}


Short_t IlcAODMCParticle::Charge()     const
{
  //
  // return tha charge
  //
    TParticlePDG* pdg =  TDatabasePDG::Instance()->GetParticle(fPdgCode);
    if (pdg) {
	return (Short_t (pdg->Charge()));
    } else {
	return -99;
    }
}

void IlcAODMCParticle::Print(const Option_t */*opt*/) const {
// Print particle information
  if(TDatabasePDG::Instance()->GetParticle(fPdgCode)){
    Printf(">>> PDG (%d) : %s",fPdgCode,TDatabasePDG::Instance()->GetParticle(fPdgCode)->GetName());
  }
  else{
    Printf(">>> PDG (%d) : %s",fPdgCode,"Unknown");
  }
  Printf(">>  P(%3.3f,%3.3f,%3.3f) V((%3.3f,%3.3f,%3.3f,%3.3f)",fPx,fPy,fPz,fVx,fVy,fVz,fVt);  
  Printf(">   Mother %d, First Daughter %d Last Daughter %d , Status %d, PhysicalPrimary %d",
	 fMother,fDaughter[0],fDaughter[1],GetStatus(),
	 IsPhysicalPrimary());
}
