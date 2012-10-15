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


//---------------------------------------------------------------------
// Service class for jet production data 
// Author: Andreas Morsch
// andreas.morsch@cern.ch
//---------------------------------------------------------------------


#include "IlcJetProductionData.h"
#include "IlcLog.h"

ClassImp(IlcJetProductionData)
 
 
////////////////////////////////////////////////////////////////////////

IlcJetProductionData::IlcJetProductionData():
  fNbins(0),
  fPtHardLimits(0x0),
  fWeights(0x0),
  fRunTitles(0x0)
{
  // Default constructor

} 

////////////////////////////////////////////////////////////////////////

IlcJetProductionData::~IlcJetProductionData()
{
  // Destructor
    delete fPtHardLimits;
    delete fRunTitles;
    
}

void IlcJetProductionData::GetPtHardLimits(Int_t bin, Float_t& ptmin, Float_t& ptmax)
{
// Get pt_hard limits for given bin
    if (bin >= 0 && bin < fNbins) {
	ptmin = fPtHardLimits[bin];
	ptmax = fPtHardLimits[bin + 1];	
    } else {
	IlcFatal("Bin out of range !");
    }
}

TString IlcJetProductionData::GetRunTitle(Int_t bin)
{
    // Get run title for given bin
    
    if (bin < 0 || bin >= fNbins) 
	IlcFatal("Bin out of range !");
    
    return fRunTitles[bin];
}

Float_t  IlcJetProductionData::GetWeight(Int_t bin)
{
    // Get weight for given bin
      if (bin < 0 || bin >= fNbins) 
	  IlcFatal("Bin out of range !");
      return fWeights[bin];
}
