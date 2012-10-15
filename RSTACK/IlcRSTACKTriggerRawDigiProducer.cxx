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
//This class produces RSTACK trigger digits of one event.
//Authors: Henrik Qvigstad, Boris Polishchuk.

#include "IlcRSTACKTriggerRawDigiProducer.h"
#include "IlcRSTACKTriggerRawReader.h"
#include "IlcRSTACKTRURawReader.h"
#include "IlcRSTACKTriggerParameters.h"
#include "IlcRSTACKTriggerRawDigit.h"
#include "IlcRSTACKGeometry.h"
#include "IlcRawReader.h"
#include "IlcCaloRawStreamV3.h"

#include "TH1I.h"
#include "TH2I.h"

#include <iostream>
using namespace std;

ClassImp(IlcRSTACKTriggerRawDigiProducer)

IlcRSTACKTriggerRawDigiProducer::IlcRSTACKTriggerRawDigiProducer()
  :fModules(kNMods, false),
   fSaturationThreshold(950),
   fParameters(0),
   fRawReader(0),
   fRawStream(0),
   fTriggerReader(0)
{}

IlcRSTACKTriggerRawDigiProducer::IlcRSTACKTriggerRawDigiProducer(IlcRawReader *rawReader)
  :fModules(kNMods, false),
   fSaturationThreshold(950),
   fParameters(0),
   fRawReader(rawReader),
   fRawStream(0),
   fTriggerReader(new IlcRSTACKTriggerRawReader)
{
  SetAnalyseModule(2);
  SetAnalyseModule(3);
  SetAnalyseModule(4);

  fRawStream = new IlcCaloRawStreamV3(rawReader,"RSTACK");
}

IlcRSTACKTriggerRawDigiProducer::~IlcRSTACKTriggerRawDigiProducer()
{
  delete fRawStream;
  delete fTriggerReader;
}

void IlcRSTACKTriggerRawDigiProducer::ProcessEvent(TClonesArray* tdigits)
{
  
  fTriggerReader->Reset();

  tdigits->Clear();
  Int_t iDigit=0 ;

  while (fRawStream->NextDDL()) {
    while (fRawStream->NextChannel()) {

      if (fRawStream->IsTRUData()) {
	fTriggerReader->ReadFromStream(fRawStream);
      }// IsTRUData
    }// NextChannel
  }//NextDDL
	
  // Loop over modules
  for(unsigned int mod = 0; mod < fModules.size(); ++mod) {
    if( fModules[mod] ) {
      
      // Loop over 4x4 cells
      for(int TRURow = 0; TRURow < kNTRURows; ++TRURow) {
	for(int branch = 0; branch < kNBranches; ++branch) {
	  
	  IlcRSTACKTRURawReader* truReader = fTriggerReader->GetTRU(mod, TRURow, branch);
	  if( truReader->IsActive() ) {
	    
	    for(int xIdx = 0; xIdx < kN4x4XPrTRURow; ++xIdx) {
	      for(int zIdx = 0; zIdx < kN4x4ZPrBranch; ++zIdx) {
	      
		// Determin if Trigger is flagged for any timeBin
		bool triggered = false;

		for(int timeBin = 0; timeBin < kNTRUTimeBins; ++timeBin){
		  if(truReader->IsActive(timeBin)) {
		    if( fTriggerReader->GetTRU(mod, TRURow, branch)->GetTriggerFlag(xIdx, zIdx, timeBin) ){
		      triggered = true;
		    } // end "if TriggerBit"
		  }
		}// end TimeBin loop
		
		if( triggered ){
		  // Get peak values
		  const int TSmax = Get4x4Max(fTriggerReader, fParameters, mod, TRURow, branch, xIdx, zIdx);
		  new((*tdigits)[iDigit]) IlcRSTACKTriggerRawDigit(mod,xIdx,zIdx,TRURow,branch,TSmax); 
		  iDigit++;
		}// end  "if triggered"
	      
	      } // end zIdx loop
	    } // end xIdx loop
	  } // truReader->IsActive
	} // end branch loop
      } // end tru loop
    } // end "if module"
  } // end mod loop
  
}

int IlcRSTACKTriggerRawDigiProducer::Get2x2Max(IlcRSTACKTriggerRawReader* reader, IlcRSTACKTriggerParameters* params, int mod, int xIdx, int zIdx)
{
  int max = 0;
  for(int timeBin = 0; timeBin < kNTRUTimeBins; ++timeBin) {
    const int signal = Get2x2Signal(reader, params, mod, xIdx, zIdx, timeBin);
    if( max < signal ){
      max = signal;
    }
  }
  return max;
}


int IlcRSTACKTriggerRawDigiProducer::Get2x2Signal(IlcRSTACKTriggerRawReader* reader, IlcRSTACKTriggerParameters* parameters, int mod, int xIdx, int zIdx, int timeBin)
{
  const int TRURow = xIdx / kN2x2XPrTRURow;
  const int branch = zIdx / kN2x2ZPrBranch;
  const int TRUX = xIdx % kN2x2XPrTRURow; // 2x2 coordinates
  const int TRUZ = zIdx % kN2x2ZPrBranch; // 2x2 coordinates

  if( reader->GetTRU(mod, TRURow, branch)->IsActive() ){
    const int signal = reader->GetTRU(mod, TRURow, branch)->GetTriggerSignal( TRUX, TRUZ, timeBin);
    if( parameters )
      return signal - parameters->GetTRUPedestal(mod, TRURow, branch, TRUX, TRUZ);
    else
      return signal - IlcRSTACKTRURawReader::kDefaultSignalValue;
  }
  else
    return 0;
}

int IlcRSTACKTriggerRawDigiProducer::Get4x4Max(IlcRSTACKTriggerRawReader* reader, IlcRSTACKTriggerParameters* params, int mod, int TRURow, int branch, int xIdx, int zIdx)
{
  int max = 0;
  for(int timeBin = 0; timeBin < kNTRUTimeBins; ++timeBin) {
    const int signal = Get4x4Signal(reader, params, mod, TRURow, branch, xIdx, zIdx, timeBin);
    if( max < signal ){
      max = signal;
    }
  }
  return max;
}


int IlcRSTACKTriggerRawDigiProducer::Get4x4Signal(IlcRSTACKTriggerRawReader* reader, IlcRSTACKTriggerParameters* params, int mod, int TRURow, int branch, int xIdx, int zIdx, int timeBin)
{
  const int modX = xIdx + TRURow * kN2x2XPrTRURow;
  const int modZ = zIdx + branch * kN2x2ZPrBranch;

  const int signal
    = Get2x2Signal(reader, params, mod, modX  , modZ  , timeBin)
      + Get2x2Signal(reader, params, mod, modX+1, modZ  , timeBin)
      + Get2x2Signal(reader, params, mod, modX  , modZ+1, timeBin)
      + Get2x2Signal(reader, params, mod, modX+1, modZ+1, timeBin);
  return signal;
}

bool IlcRSTACKTriggerRawDigiProducer::Is2x2Active(IlcRSTACKTriggerRawReader* reader, int mod, int xIdx, int zIdx)
{
  const int TRURow = xIdx / kN2x2XPrTRURow;
  const int branch = zIdx / kN2x2ZPrBranch;

  return reader->GetTRU(mod, TRURow, branch)->IsActive();
}

bool IlcRSTACKTriggerRawDigiProducer::Is2x2Active(IlcRSTACKTriggerRawReader* reader, int mod, int xIdx, int zIdx, int timeBin)
{
  const int TRURow = xIdx / kN2x2XPrTRURow;
  const int branch = zIdx / kN2x2ZPrBranch;

  return reader->GetTRU(mod, TRURow, branch)->IsActive(timeBin);
}


