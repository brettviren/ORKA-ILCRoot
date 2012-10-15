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

/* $Id$ */

//---------------------------------------------------------------------
// Class for input particles
// manages the search for jets
// Authors: Elena Bruna elena.bruna@yale.edu
//
// ** 2011 magali.estienne@subatech.in2p3.fr &  alexandre.shabetai@cern.ch
// Modified accordingly to reader/finder splitting and new handling of neutral information
//---------------------------------------------------------------------

#include <Riostream.h> 

#include "IlcJetHeader.h"
#include "IlcFastJetHeaderV1.h"
#include "IlcFastJetInput.h"
#include "IlcJetCalTrk.h"

#include "fastjet/PseudoJet.hh"

#include<vector> 

using namespace std;

ClassImp(IlcFastJetInput)

////////////////////////////////////////////////////////////////////////

IlcFastJetInput::IlcFastJetInput():
  fHeader(0x0),
  fCalTrkEvent(0x0),
  fInputParticles(0),
  fInputParticlesCh(0)
{
  // Default constructor
}

//______________________________________________________________________
IlcFastJetInput::IlcFastJetInput(const IlcFastJetInput &input):
  TObject(input),
  fHeader(input.fHeader),
  fCalTrkEvent(input.fCalTrkEvent),
  fInputParticles(input.fInputParticles),
  fInputParticlesCh(input.fInputParticlesCh)
{
  // copy constructor
}

//______________________________________________________________________
IlcFastJetInput& IlcFastJetInput::operator=(const IlcFastJetInput& source)
{
  // Assignment operator. 
  if(this!=&source){
   TObject::operator=(source);
   fHeader = source.fHeader;
   fCalTrkEvent = source.fCalTrkEvent;
   fInputParticles = source.fInputParticles;
   fInputParticlesCh = source.fInputParticlesCh;
  }

  return *this;

}

//___________________________________________________________
void IlcFastJetInput::FillInput()
{
  // fills input particles for FASTJET based analysis
  
  IlcFastJetHeaderV1 *header = (IlcFastJetHeaderV1*)fHeader;
  Int_t debug  = header->GetDebug();     // debug option

  if(debug>0) cout<<"-------- IlcFastJetInput::FillInput()  ----------------"<<endl;

  fInputParticles.clear();
  fInputParticlesCh.clear();

  // RUN ALGORITHM  
  // read input particles -----------------------------
  vector<fastjet::PseudoJet> inputParticles;

  if(fCalTrkEvent == 0) { cout << "Could not get the CalTrk Event" << endl; return; }
  Int_t nIn =  fCalTrkEvent->GetNCalTrkTracks() ;
  if(nIn == 0) { if (debug>0) cout << "entries = 0 ; Event empty !!!" << endl ; return; }

  // Information extracted from fCalTrkEvent
  // load input vectors and calculate total energy in array
  Float_t px = -999., py = -999., pz = -999., en = -999.; 
 
  // Fill charged tracks
  for(Int_t i = 0; i < fCalTrkEvent->GetNCalTrkTracks(); i++)
    { // loop for all input particles
      if (fCalTrkEvent->GetCalTrkTrack(i)->GetCutFlag() != 1) continue;
      px =  fCalTrkEvent->GetCalTrkTrack(i)->GetPx();
      py =  fCalTrkEvent->GetCalTrkTrack(i)->GetPy();
      pz =  fCalTrkEvent->GetCalTrkTrack(i)->GetPz();
      en =  fCalTrkEvent->GetCalTrkTrack(i)->GetP();

      fastjet::PseudoJet inputPart(px,py,pz,en);  // create PseudoJet object
      inputPart.set_user_index(i);      //label the particle into Fastjet algortihm
      fInputParticles.push_back(inputPart);       // back of the inputParticles vector
 
      // only for charged particles (TPC+ITS)
      fastjet::PseudoJet inputPartCh(px,py,pz,en); // create PseudoJet object
      inputPartCh.set_user_index(i);               //label the particle into Fastjet algortihm
      fInputParticlesCh.push_back(inputPartCh);    // back of the inputParticles vector
    } // End loop on CalTrk

}

//_____________________________________________________________________
Double_t IlcFastJetInput::Thermalspectrum(const Double_t *x, const Double_t *par)
{
  // compute an exponential function
  return x[0]*TMath::Exp(-x[0]/par[0]);

}
