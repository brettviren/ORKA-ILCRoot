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
// Jet kinematics reader 
// MC reader for jet analysis
// Author: Andreas Morsch (andreas.morsch@cern.ch)
//-------------------------------------------------------------------------

// From root ...
#include <TClonesArray.h>
#include <TPDGCode.h>
#include <TParticle.h>
#include <TParticlePDG.h>
#include <TLorentzVector.h>
#include <TSystem.h>
#include <TDatabasePDG.h>
#include <TRandom.h>
// From IlcRoot ...
#include "IlcAODJet.h"
#include "IlcPDG.h"
#include "IlcJetKineReaderHeader.h"
#include "IlcJetKineReader.h"
#include "IlcMCEventHandler.h"
#include "IlcMCEvent.h"
#include "IlcStack.h"
#include "IlcHeader.h"
#include "IlcGenPythiaEventHeader.h"
#include "IlcLog.h"

ClassImp(IlcJetKineReader)

IlcJetKineReader::IlcJetKineReader():
    IlcJetReader(),  
    fIlcHeader(0),
    fMCEvent(0),
    fGenJets(0)
{
  // Default constructor
}

//____________________________________________________________________________

IlcJetKineReader::~IlcJetKineReader()
{
  // Destructor
    if (fIlcHeader) {
	delete fIlcHeader;
	fIlcHeader = 0;
    }
}

//____________________________________________________________________________

void IlcJetKineReader::OpenInputFiles()
{
  // Opens the input file using the run loader
  // Not used anymore   
}

//____________________________________________________________________________

Bool_t IlcJetKineReader::FillMomentumArray()
{
  // Fill momentum array for event
    Int_t goodTrack = 0;
    // Clear array
    // temporary storage of signal and cut flags
    Int_t* sflag  = new Int_t[50000];
    Int_t* cflag  = new Int_t[50000];
  
    ClearArray();
    // Get the stack
    IlcStack* stack = fMCEvent->Stack();
    // Number of primaries
    Int_t nt = stack->GetNprimary();
      
    // Get cuts set by user and header
    Double_t ptMin = ((IlcJetKineReaderHeader*) fReaderHeader)->GetPtCut();
    Float_t etaMin = fReaderHeader->GetFiducialEtaMin();
    Float_t etaMax = fReaderHeader->GetFiducialEtaMax();  
    fIlcHeader = fMCEvent->Header();
      
    
    TLorentzVector p4;
    // Loop over particles
    for (Int_t it = 0; it < nt; it++) {
	TParticle *part = stack->Particle(it);
	Int_t   status  = part->GetStatusCode();
	Int_t   pdg     = TMath::Abs(part->GetPdgCode());
	Float_t pt      = part->Pt(); 
	  
	// Skip non-final state particles, neutrinos 
	// and particles with pt < pt_min 
	if ((status != 1)            
	    || (pdg == 12 || pdg == 14 || pdg == 16)) continue; 
	
	Float_t p       = part->P();
	Float_t p0      = p;
	Float_t eta     = part->Eta();
	Float_t phi     = part->Phi();


	if (((IlcJetKineReaderHeader*)fReaderHeader)->ChargedOnly()) {
	    // Charged particles only
	    Float_t charge = 
		TDatabasePDG::Instance()->GetParticle(pdg)->Charge();
	    if (charge == 0)               continue;
	} // End charged only

	
	// Fast simulation of TPC if requested
	if (((IlcJetKineReaderHeader*)fReaderHeader)->FastSimTPC()) {
	    // Charged particles only
	    Float_t charge = 
		TDatabasePDG::Instance()->GetParticle(pdg)->Charge();
	    if (charge == 0)               continue;
	    // Simulate efficiency
	    if (!Efficiency(p0, eta, phi)) continue;
	    // Simulate resolution
	    p = SmearMomentum(4, p0);
	} // End fast TPC
	  
	  // Fast simulation of EMCAL if requested
	if (((IlcJetKineReaderHeader*)fReaderHeader)->FastSimEMCAL()) {
	    Float_t charge = 
		TDatabasePDG::Instance()->GetParticle(pdg)->Charge();
	    // Charged particles only
	    if (charge != 0){
		// Simulate efficiency
		if (!Efficiency(p0, eta, phi)) continue;
		// Simulate resolution
		p = SmearMomentum(4, p0);
	    } // end "if" charged particles
	      // Neutral particles (exclude K0L, n, nbar)
	    if (pdg == kNeutron || pdg == kK0Long) continue;
	} // End fast EMCAL
	
	  // Fill momentum array
	Float_t r  = p/p0;
	Float_t px = r * part->Px(); 
	Float_t py = r * part->Py(); 
	Float_t pz = r * part->Pz();
	TParticlePDG *pPDG = part->GetPDG();
	Float_t m = 0;
	if(!pPDG){
	  // this is very rare...
	  // Is avoided by IlcPDG::AddParticlesToPdgDataBase();
	  // but this should be called only once... (how in proof?)
	  // Calucluate mass with unsmeared momentum values
	  m  = part->Energy()*part->Energy() - 
	    (px * px + py * py + pz * pz)/r/r; 
	  if(m>0)m = TMath::Sqrt(m);
	  else m = 0;
	  IlcInfo(Form("Unknown Particle using %d calculated mass m = %3.3f",part->GetPdgCode(),m));

	}
	else{
	  m  = pPDG->Mass();
	}
	Float_t e  = TMath::Sqrt(px * px + py * py + pz * pz + m * m);
	p4 = TLorentzVector(px, py, pz, e);
	if ( (p4.Eta()>etaMax) || (p4.Eta()<etaMin)) continue;
	new ((*fMomentumArray)[goodTrack]) TLorentzVector(p4);
	
	// all tracks are signal ... ?
	sflag[goodTrack]=1;
	cflag[goodTrack]=0;
	if (pt > ptMin) cflag[goodTrack] = 1; // track surviving pt cut
	goodTrack++;
    } // track loop

// set the signal flags
    fSignalFlag.Set(goodTrack, sflag);
    fCutFlag.Set(goodTrack, cflag);
    IlcInfo(Form(" Number of good tracks %d \n", goodTrack));
    
    delete[] sflag;
    delete[] cflag;
    
    return kTRUE;
}


Float_t IlcJetKineReader::SmearMomentum(Int_t ind, Float_t p)
{
  //  The relative momentum error, i.e. 
  //  (delta p)/p = sqrt (a**2 + (b*p)**2) * 10**-2,
  //  where typically a = 0.75 and b = 0.16 - 0.24 depending on multiplicity
  //  (the lower value is for dn/d(eta) about 2000, and the higher one for 8000)
  //
  //  If we include information from TRD, b will be a factor 2/3 smaller.
  //
  //  ind = 1: high multiplicity
  //  ind = 2: low  multiplicity
  //  ind = 3: high multiplicity + TRD
  //  ind = 4: low  multiplicity + TRD
  
  Float_t pSmeared;
  Float_t a = 0.75;
  Float_t b = 0.12;
  
  if (ind == 1) b = 0.12;
  if (ind == 2) b = 0.08;
  if (ind == 3) b = 0.12;    
  if (ind == 4) b = 0.08;    
  
  Float_t sigma =  p * TMath::Sqrt(a * a + b * b * p * p)*0.01;
  pSmeared = p + gRandom->Gaus(0., sigma);
  return pSmeared;
}


Bool_t IlcJetKineReader::Efficiency(Float_t p, Float_t /*eta*/, Float_t phi)
{
  // Fast simulation of geometrical acceptance and tracking efficiency
 
  //  Tracking
  Float_t eff = 0.99;
  if (p < 0.5) eff -= (0.5-p)*0.2/0.3;
  // Geometry
  if (p > 0.5) {
    phi *= 180. / TMath::Pi();
    // Sector number 0 - 17
    Int_t isec  = Int_t(phi / 20.);
    // Sector centre
    Float_t phi0 = isec * 20. + 10.;
    Float_t phir = TMath::Abs(phi-phi0);
    // 2 deg of dead space
    if (phir > 9.) eff = 0.;
  }

  if (gRandom->Rndm() > eff) {
    return kFALSE;
  } else {
    return kTRUE;
  }

}

TClonesArray*  IlcJetKineReader::GetGeneratedJets()
{
    //
    // Get generated jets from mc header
    //
    if (!fGenJets) {
	fGenJets = new TClonesArray("IlcAODJets", 100);
    } else {
	fGenJets->Clear();
    }
    
    
    IlcHeader* ilch = GetIlcHeader(); 
    if (ilch == 0) return 0;
    IlcGenEventHeader * genh = ilch->GenEventHeader();
    if (genh == 0) return 0;

    Int_t nj =((IlcGenPythiaEventHeader*)genh)->NTriggerJets(); 
    for (Int_t i = 0; i < nj; i++) {
	Float_t p[4];
	((IlcGenPythiaEventHeader*)genh)->TriggerJet(i,p);
	new ((*fGenJets)[i]) IlcAODJet(p[0], p[1], p[2], p[3]);
    }

    return fGenJets;
}

void IlcJetKineReader::SetInputEvent(const TObject* /*esd*/, const TObject* /*aod*/, const TObject* mc)
{
    // Connect the input event
    fMCEvent = (IlcMCEvent*) mc;
}
