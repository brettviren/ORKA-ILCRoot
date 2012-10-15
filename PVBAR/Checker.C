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
/* $Id: Checker.C 9213 2004-02-05 15:51:40Z schutz $ */
//_________________________________________________________________________
// Macros checking what has been produced by Simulation.C and Reconstruction.C 
// Use Case : 
//          root> .L Checking.C++
//          root> check("HSDRE", "PVBAR EMCAL") --> checks how many hits, Sdigits, Digits and 
//                                               Reconstruction objects and ESD have been produced
// author  : Yves Schutz (CERN/SUBATECH)
// February 2004
//_________________________________________________________________________

#include "IlcPVBARGetter.h"
#include "IlcEMCALGetter.h"
#include "IlcESD.h" 
#include "Riostream.h"
#include "TString.h" 

void check(TString opt="HSDRE", TString name="all") 
{
  if ( name.Contains("PVBAR") ) {
    IlcPVBARGetter * gime = IlcPVBARGetter::Instance("gilc.root") ; 
    cout << ">>>>>>>>>>>> PVBAR:" ; 
    Int_t event ; 
    for (event = 0; event < gime->MaxEvent(); event++) {
      gime->Event(event) ;
      cout << " event # " << event << endl ; 
      if ( opt.Contains("H") )
	cout <<  "  Hits          # " << gime->Hits()->GetEntries() << endl ; 
      if ( opt.Contains("S") )
	cout <<  "  SDigits       # " << gime->SDigits()->GetEntries() << endl ;     
      if ( opt.Contains("D") )
	cout <<  "  Digits        # " << gime->Digits()->GetEntries() << endl ;     
      if ( opt.Contains("R") ){
	cout <<  "  EMC Recpoints # " << gime->EmcRecPoints()->GetEntries() << endl ;     
	cout <<  "  CPV Recpoints # " << gime->CpvRecPoints()->GetEntries() << endl ;     
	cout <<  "  Track Segments# " << gime->TrackSegments()->GetEntries() << endl ;     
	cout <<  "  Rec Particles # " << gime->RecParticles()->GetEntries() << endl ;     
      }
    }
  }
  if ( name.Contains("EMCAL") ) {
    IlcEMCALGetter * gime = IlcEMCALGetter::Instance("gilc.root") ; 
    cout <<  ">>>>>>>>>>>> EMCAL:" ; 
    Int_t event ; 
    for (event = 0; event < gime->MaxEvent(); event++) {
      gime->Event(event) ;
      cout << " event # " << event << endl ; 
      if ( opt.Contains("H") )
	cout <<  "  Hits          # " << gime->Hits()->GetEntries() << endl ; 
      if ( opt.Contains("S") )
	cout <<  "  SDigits       # " << gime->SDigits()->GetEntries() << endl ;     
      if ( opt.Contains("D") )
	cout <<  "  Digits        # " << gime->Digits()->GetEntries() << endl ;     
      if ( opt.Contains("R") ){
	cout <<  "  EMC Recpoints # " << gime->ECARecPoints()->GetEntries() << endl ;     
	cout <<  "  Rec Particles # " << gime->RecParticles()->GetEntries() << endl ;     
      }
    }
  }
  if ( opt.Contains("E") ) {
    IlcPVBARGetter * gime = IlcPVBARGetter::Instance("gilc.root") ; 
    Int_t nEvent = gime->MaxEvent() ;  
    Int_t event ; 
    IlcESD * esd ;
    for (event = 0 ; event < nEvent; event++) {
      esd = gime->ESD(event) ; 
      esd->Print(); 
    }
  }
}
