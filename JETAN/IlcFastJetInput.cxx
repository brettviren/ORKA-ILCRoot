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

#include <Riostream.h> 
#include <TChain.h>
#include <TFile.h>
#include <TF1.h>
#include <TRandom.h>
#include <TList.h>
#include <TLorentzVector.h>
#include <TArrayF.h>
#include <TClonesArray.h>

#include "IlcJetHeader.h"
#include "IlcJetReader.h"
#include "IlcJetReaderHeader.h"
#include "IlcJetHistos.h"

#include "IlcFastJetFinder.h"
#include "IlcFastJetHeaderV1.h"
#include "IlcJetReaderHeader.h"
#include "IlcJetReader.h"
#include "IlcJetESDReader.h"
#include "IlcJetUnitArray.h"
#include "IlcFastJetInput.h"

#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequenceArea.hh"
#include "fastjet/AreaDefinition.hh"
#include "fastjet/JetDefinition.hh"
// get info on how fastjet was configured
#include "fastjet/config.h"

#ifdef ENABLE_PLUGIN_SISCONE
#include "fastjet/SISConePlugin.hh"
#endif

#include<sstream>  // needed for internal io
#include<vector> 
#include <cmath> 

using namespace std;


ClassImp(IlcFastJetInput)

////////////////////////////////////////////////////////////////////////

IlcFastJetInput::IlcFastJetInput():
    fReader(0),
    fHeader(0),
    fInputParticles(0),
    fInputParticlesCh(0)
{
  // Default constructor
}
IlcFastJetInput::IlcFastJetInput(const IlcFastJetInput &input):
    TObject(),
    fReader(input.fReader),
    fHeader(input.fHeader),
    fInputParticles(input.fInputParticles),
    fInputParticlesCh(input.fInputParticlesCh)
{
  // copy constructor
}
//______________________________________________________________________
IlcFastJetInput& IlcFastJetInput::operator=(const IlcFastJetInput& source){
    // Assignment operator. 
    this->~IlcFastJetInput();
    new(this) IlcFastJetInput(source);
    return *this;

}
//___________________________________________________________
void IlcFastJetInput::FillInput(){
  //cout<<"-------- IlcFastJetInput::FillInput()  ----------------"<<endl;

  IlcFastJetHeaderV1 *header = (IlcFastJetHeaderV1*)fHeader;
  fInputParticles.clear();
  fInputParticlesCh.clear();

  Int_t debug  = header->GetDebug();     // debug option
  Int_t fOpt   = fReader->GetReaderHeader()->GetDetector();

  // RUN ALGORITHM  
  // read input particles -----------------------------
  vector<fastjet::PseudoJet> inputParticles;
  //  cout<<"=============== IlcFastJetInput::FillInput()  =========== fOpt="<<fOpt<<endl;
  //cout<<"pointers --> fReader="<<fReader<<" header="<<header<<" fHeader="<<fHeader<<endl;
  //cout<<"Rparam="<<Rparam<<"  ghost_etamax="<<ghost_etamax<<"  ghost_area="<<ghost_area<<endl;
  //cout<<fReader->ClassName()<<endl;

  if(fOpt==0)
    {
      TClonesArray *lvArray = fReader->GetMomentumArray();
      if(lvArray == 0) { cout << "Could not get the momentum array" << endl; return; }
      Int_t nIn =  lvArray->GetEntries();
      if(nIn == 0) { if (debug) cout << "entries = 0 ; Event empty !!!" << endl ; return; }
      Float_t px,py,pz,en;
      // load input vectors
      for(Int_t i = 0; i < nIn; i++){ // loop for all input particles
	TLorentzVector *lv = (TLorentzVector*) lvArray->At(i);
	px = lv->Px();
	py = lv->Py();
	pz = lv->Pz();
	en = lv->Energy();
	//	cout<<"in FillInput...... "<<i<<" "<<px<<" "<<py<<"  "<<pz<<" "<<en<<endl;
	fastjet::PseudoJet inputPart(px,py,pz,en); // create PseudoJet object
	inputPart.set_user_index(i); //label the particle into Fastjet algortihm
	fInputParticles.push_back(inputPart);  // back of the inputParticles vector  
      } // end loop 
    }
  else {
    TClonesArray* fUnit = fReader->GetUnitArray();
    if(fUnit == 0) { cout << "Could not get the momentum array" << endl; return; }
    Int_t         nIn = fUnit->GetEntries();
    if(nIn == 0) { if (debug) cout << "entries = 0 ; Event empty !!!" << endl ; return; }
   
    // Information extracted from fUnitArray
    // load input vectors and calculate total energy in array
    Float_t pt,eta,phi,theta,px,py,pz,en;
    Int_t ipart = 0;
    Int_t countUnit=0,countUnitNonZero=0;
    //cout<<" nIn = "<<nIn<<endl;

    for(Int_t i=0; i<nIn; i++) 
      {
	IlcJetUnitArray *uArray = (IlcJetUnitArray*)fUnit->At(i);
	if(uArray->GetUnitEnergy()>0.){
	  countUnit++;
	  // It is not necessary anymore to cut on particle pt
	  pt    = uArray->GetUnitEnergy();
	  eta   = uArray->GetUnitEta();
	  phi   = uArray->GetUnitPhi();
	  theta = EtaToTheta(eta);
	  en    = (TMath::Abs(TMath::Sin(theta)) == 0) ? pt : pt/TMath::Abs(TMath::Sin(theta));
	  px    = TMath::Cos(phi)*pt;
	  py    = TMath::Sin(phi)*pt;
	  pz    = en*TMath::TanH(eta);
	  if(debug) cout << "pt: " << pt << ", eta: " << eta << ", phi: " << phi << ", en: " << en << ", px: " << px << ", py: " << py << ", pz: " << pz << endl;
	  //cout << i<<" "<<"pt: " << pt << ", eta: " << eta << ", phi: " << phi << ", en: " << en << ", px: " << px << ", py: " << py << ", pz: " << pz << endl;
	  //cout<<"in FillInput...... "<<i<<" "<<px<<" "<<py<<"  "<<pz<<" "<<en<<endl;

	  fastjet::PseudoJet inputPart(px,py,pz,en); // create PseudoJet object
	  inputPart.set_user_index(ipart); //label the particle into Fastjet algortihm
	  fInputParticles.push_back(inputPart);  // back of the inputParticles vector 
	  ipart++;

	
	
	  //only for charged particles (TPC+ITS)
	  TRefArray* ref = uArray->GetUnitTrackRef();
	  Int_t nRef = ref->GetEntries();
	  for(Int_t j=0; j<nRef;j++){
	    Float_t pxj=0.;  Float_t pyj=0.;  Float_t pzj=0.;Float_t enj=0.;
	    pxj = ((IlcVTrack*)ref->At(j))->Px();
	    pyj = ((IlcVTrack*)ref->At(j))->Py();
	    pzj = ((IlcVTrack*)ref->At(j))->Pz();
	    enj=TMath::Sqrt(pxj*pxj+pyj*pyj+pzj*pzj);
	    fastjet::PseudoJet inputPartCh(pxj,pyj,pzj,enj); // create PseudoJet object
	    inputPartCh.set_user_index(((IlcVTrack*)ref->At(j))->GetID()); //label the particle into Fastjet algortihm
	    fInputParticlesCh.push_back(inputPartCh);  // back of the inputParticles vector 

	  }
	}
      } // End loop on UnitArray 
    if (debug) cout<<"countUnit(En>0) = "<<countUnit<<"  countUnit with Non ZeroSize = "<<countUnitNonZero<<endl;
  }

}

//_____________________________________________________________________
Float_t  IlcFastJetInput::EtaToTheta(Float_t arg)
{
  //  return (180./TMath::Pi())*2.*atan(exp(-arg));
  return 2.*atan(exp(-arg));


}
Double_t IlcFastJetInput::Thermalspectrum(const Double_t *x, const Double_t *par){

  return x[0]*TMath::Exp(-x[0]/par[0]);

}
