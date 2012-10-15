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


/* $Id: IlcRSTACKvImpacts.cxx 49869 2011-05-18 04:49:51Z hristov $ */

/* History of cvs commits:
 *
 * $Log$
 * Revision 1.24  2006/11/14 17:11:15  hristov
 * Removing inheritances from TAttLine, TAttMarker and IlcRndm in IlcModule. The copy constructor and assignment operators are moved to the private part of the class and not implemented. The corresponding changes are propagated to the detectors
 *
 * Revision 1.23  2006/09/13 07:31:01  kharlov
 * Effective C++ corrections (T.Pocheptsov)
 *
 * Revision 1.22  2005/06/17 07:39:07  hristov
 * Removing GetDebug and SetDebug from IlcRun and IlcModule. Using IlcLog for the messages
 *
 * Revision 1.21  2005/05/28 14:19:05  schutz
 * Compilation warnings fixed by T.P.
 *
 */

//_________________________________________________________________________
// Implementation version vImpacts of RSTACK Manager class.
// This class inherits from v1 and adds impacts storing.
// Impacts stands for exact values of track coming to the detectors
// EMC, CPV or PPSD.
// Impacts are written to the same tree as hits are
// but in separate branches.
//---
//*-- Author: Yuri Kharlov (IHEP, Protvino/SUBATECH, Nantes)


// --- ROOT system ---

//#include <TTree.h>
#include <TClonesArray.h>
#include <TVirtualMC.h>
#include <TTree.h>

// --- Standard library ---

// --- IlcRoot header files ---

#include "IlcRSTACKGeometry.h"
#include "IlcRSTACKImpact.h"
#include "IlcRSTACKvImpacts.h"
#include "IlcRun.h"
#include "IlcLoader.h"
#include "IlcMC.h"
#include "IlcLog.h"

ClassImp(IlcRSTACKvImpacts)

//____________________________________________________________________________
IlcRSTACKvImpacts::IlcRSTACKvImpacts():
  fEMCImpacts(new TList),
  fCPVImpacts(new TList),
  fPPSDImpacts(new TList),
  fNEMCImpacts(),
  fNCPVImpacts(),
  fNPPSDImpacts()
{
  // ctor
}

//____________________________________________________________________________
IlcRSTACKvImpacts::IlcRSTACKvImpacts(const char *name, const char *title):
  IlcRSTACKv1(name,title),
  fEMCImpacts(new TList),
  fCPVImpacts(new TList),
  fPPSDImpacts(0),
  fNEMCImpacts(),
  fNCPVImpacts(),
  fNPPSDImpacts()
{
  // ctor : title is used to identify the layout
  //
  // We store hits :
  //   - fHits (the "normal" one), which retains the hits associated with
  //     the current primary particle being tracked
  //     (this array is reset after each primary has been tracked).
  //     This part inherits from IlcRSTACKv1
  //
  // We store impacts :
  //  - fEMCImpacts, fCPVImpacts which are
  //    TList of EMC and CPV modules respectively, each
  //    modules contains TClonesArray of IlcRSTACKImpacts

  Int_t nRSTACKModules = GetGeometry()->GetNModules();
  Int_t nCPVModules  = GetGeometry()->GetNModules();

  Int_t iRSTACKModule;
  TClonesArray * impacts;
  for (iRSTACKModule=0; iRSTACKModule<nRSTACKModules; iRSTACKModule++) {
    fEMCImpacts->Add(new TClonesArray("IlcRSTACKImpact",200)) ;
    fNEMCImpacts[iRSTACKModule] = 0;
    impacts = dynamic_cast<TClonesArray *>(fEMCImpacts->At(iRSTACKModule));
  }
  for (iRSTACKModule=0; iRSTACKModule<nCPVModules; iRSTACKModule++) {
    fCPVImpacts->Add(new TClonesArray("IlcRSTACKImpact",200)) ;
    fNCPVImpacts[iRSTACKModule] = 0;
    impacts = dynamic_cast<TClonesArray *>(fCPVImpacts->At(iRSTACKModule));
  }

}

//____________________________________________________________________________
IlcRSTACKvImpacts::~IlcRSTACKvImpacts()
{
  // dtor

  // Delete hits
  if ( fHits ) {
    fHits->Delete() ; 
    delete fHits ;
    fHits = 0 ; 
  }

  // Delete impacts in EMC, CPV
  if ( fEMCImpacts ) {
    fEMCImpacts->Delete() ; 
    delete fEMCImpacts ;
    fEMCImpacts = 0 ; 
  }
  if ( fCPVImpacts ) {
    fCPVImpacts->Delete() ; 
    delete fCPVImpacts ;
    fCPVImpacts = 0 ; 
  }
}

//____________________________________________________________________________
void IlcRSTACKvImpacts::AddImpact(const char* det, Int_t shunt, Int_t primary, Int_t track, Int_t module,
			   Int_t pid, TLorentzVector p, Float_t *xyz)
{
  // Add an impact to the impact list.

  TClonesArray * impacts = 0;
  Int_t         nImpacts = 0;

  if (strcmp(det,"EMC ")==0) {
    impacts = dynamic_cast<TClonesArray *>(fEMCImpacts->At(module));
    nImpacts= fNEMCImpacts[module];
    fNEMCImpacts[module]++ ;
  }
  else if (strcmp(det,"CPV ")==0) {
    impacts = dynamic_cast<TClonesArray *>(fCPVImpacts->At(module));
    nImpacts= fNCPVImpacts[module];
    fNCPVImpacts[module]++ ;
  }
  else
    IlcFatal(Form("Wrong RSTACK configuration: det=%s",det));

  new((*impacts)[nImpacts]) IlcRSTACKImpact(shunt,primary,track,pid,p,xyz) ;

  IlcDebugClass(1,Form("Module %d %s: ",module,det));
  if (IlcLog::GetGlobalDebugLevel()>0)
    (static_cast<IlcRSTACKImpact*>((impacts->At(nImpacts))))->Print();
}

//____________________________________________________________________________
void IlcRSTACKvImpacts::MakeBranch(Option_t *opt)
{  
  // Create new branch in the current Hits Root Tree containing
  // a list of RSTACK impacts (exact values of track coming to detector)

  IlcDetector::MakeBranch(opt);
  
  Int_t bufferSize = 32000 ;
  Int_t splitlevel = 0 ;
  fLoader->TreeH()->Branch("RSTACKEmcImpacts" , "TList", &fEMCImpacts , bufferSize, splitlevel);
  fLoader->TreeH()->Branch("RSTACKCpvImpacts" , "TList", &fCPVImpacts , bufferSize, splitlevel);
  
}

//____________________________________________________________________________
void IlcRSTACKvImpacts::ResetHits() 
{              
  // Reset impact branches for EMC, CPV and PPSD

  IlcDetector::ResetHits();

  Int_t i;
  for (i=0; i<GetGeometry()->GetNModules(); i++) {
    (static_cast<TClonesArray*>(fEMCImpacts->At(i))) -> Clear();
    fNEMCImpacts[i] = 0 ;
  }

  for (i=0; i<GetGeometry()->GetNModules(); i++) {
    (static_cast<TClonesArray*>(fCPVImpacts->At(i))) -> Clear();
    fNCPVImpacts[i] = 0 ;
  }
  
}

//_____________________________________________________________________________
void IlcRSTACKvImpacts::StepManager(void)
{
  // Find impacts (tracks which enter the EMC, CPV)
  // and add them to to the impact lists

  IlcRSTACKv1::StepManager();

  Float_t xyzm[3], xyzd[3], pm[3], pd[3];
  TLorentzVector pmom     ;           // Lorentz momentum of the particle initiated hit
  TLorentzVector pos      ;           // Lorentz vector of the track current position
  Int_t          copy     ;

  Int_t tracknumber =  gIlc->GetMCApp()->GetCurrentTrackNumber() ; 
  Int_t primary     =  gIlc->GetMCApp()->GetPrimary( gIlc->GetMCApp()->GetCurrentTrackNumber() ); 
  TString name      =  GetGeometry()->GetName() ; 

  // Add impact to EMC

  static Int_t idPXTL = gMC->VolId("PXTL");
  if( gMC->CurrentVolID(copy) == idPXTL &&
      gMC->IsTrackEntering() ) {
    gMC->TrackMomentum(pmom);
    gMC->TrackPosition(pos) ;

    Int_t i;
    for (i=0; i<3; i++) xyzm[i] = pos[i];

    for (i=0; i<3; i++) {
      xyzm[i] = pos[i] ;
      pm[i]   = pmom[i];
    }
    gMC -> Gmtod (xyzm, xyzd, 1);    // transform coordinate from master to daughter system
    gMC -> Gmtod (pm,   pd,   2);    // transform 3-momentum from master to daughter system

    // Select tracks coming to the crystal from up or down sides
    if ((pd[1]<0 && xyzd[1] >  GetGeometry()->GetCrystalSize(1)/2-0.1) ||
	(pd[1]>0 && xyzd[1] < -GetGeometry()->GetCrystalSize(1)/2+0.1)) {
    // Select tracks coming to the crystal from up or down sides
      Int_t pid = gMC->TrackPid();
      Int_t module;
      gMC->CurrentVolOffID(10,module);
      module--;
      AddImpact("EMC ",fIshunt, primary,tracknumber, module, pid, pmom, xyzm);
    }
  }

  // Add impact to CPV

  static Int_t idPCPQ = gMC->VolId("PCPQ");
  if( gMC->CurrentVolID(copy) == idPCPQ &&
      gMC->IsTrackEntering() ) {
    gMC->TrackMomentum(pmom);
    gMC->TrackPosition(pos) ;

    Int_t i;
    for (i=0; i<3; i++) xyzm[i] = pos[i];

    for (i=0; i<3; i++) {
      xyzm[i] = pos[i] ;
      pm[i]   = pmom[i];
    }
    Int_t pid = gMC->TrackPid();
    Int_t module;
    gMC->CurrentVolOffID(3,module);
    module--;
    AddImpact("CPV ",fIshunt, primary,tracknumber, module, pid, pmom, xyzm);
  }
  
}
