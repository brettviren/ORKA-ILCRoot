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

/* $Id: IlcPVECPID.cxx 44091 2010-10-05 09:46:25Z prsnko $ */

//_________________________________________________________________________
//  Algorithm class for the identification of particles detected in PVEC        
//  base  class  of identified particle  
//  Why should I put meaningless comments
//  just to satisfy
//  the code checker                
                         
//                  
//*-- Author: Yves Schutz (SUBATECH) & Dmitri Peressounko


// --- ROOT system ---
#include "TBranch.h"
#include "TClonesArray.h"
#include "TTree.h"

// --- Standard library ---

// --- IlcRoot header files ---
#include "IlcConfig.h"
#include "IlcLog.h"
#include "IlcPVECPID.h"

ClassImp(IlcPVECPID)

//____________________________________________________________________________
IlcPVECPID::IlcPVECPID():
  TObject(),
  fGeom(NULL),
  fESD(0x0),
  fEMCRecPoints(NULL),
  fCPVRecPoints(NULL),
  fTrackSegments(NULL),
  fRecParticles(NULL)
{
  // ctor
}


//____________________________________________________________________________
IlcPVECPID::IlcPVECPID(IlcPVECGeometry *geom):
  TObject(),
  fGeom(geom),
  fESD(0x0),
  fEMCRecPoints(NULL),
  fCPVRecPoints(NULL),
  fTrackSegments(NULL),
  fRecParticles(NULL)
{
  // ctor
  fEMCRecPoints = new TObjArray(100) ;
  fCPVRecPoints = new TObjArray(100) ;
  fRecParticles = new TClonesArray("IlcPVECRecParticle",100) ;
  fRecParticles->SetName("RECPARTICLES");

}

//____________________________________________________________________________
IlcPVECPID::IlcPVECPID(const IlcPVECPID & pid) :
  TObject(pid),
  fGeom(pid.fGeom),
  fESD(pid.fESD), 
  fEMCRecPoints(pid.fEMCRecPoints),
  fCPVRecPoints(pid.fCPVRecPoints),
  fTrackSegments(pid.fTrackSegments),
  fRecParticles(pid.fRecParticles)
{
  // Copy constructor
}

//____________________________________________________________________________
IlcPVECPID::~IlcPVECPID()
{
  // dtor
  if (fEMCRecPoints) {
    fEMCRecPoints->Delete();
    delete fEMCRecPoints;
  }
  if (fCPVRecPoints) {
    fCPVRecPoints->Delete();
    delete fCPVRecPoints;
  }
  if (fRecParticles) {
    fRecParticles->Delete();
    delete fRecParticles;
  }
}

//____________________________________________________________________________
void IlcPVECPID::SetInput(TTree *clustersTree, TClonesArray *trackSegments)
{
  // Read the clusters tree and creates the
  // arrays with the EMC and CPV
  // clusters.
  // and set the corresponding branch addresses

  fTrackSegments = trackSegments;

  TBranch *emcbranch = clustersTree->GetBranch("PVECEmcRP");
  if (!emcbranch) { 
    IlcError("can't get the branch with the PVEC EMC clusters !");
    return;
  }
  emcbranch->SetAddress(&fEMCRecPoints);
  fEMCRecPoints->Delete();
  emcbranch->GetEntry(0);

  TBranch *cpvbranch = clustersTree->GetBranch("PVECCpvRP");
  if (!cpvbranch) { 
    IlcError("can't get the branch with the PVEC CPV clusters !");
    return;
  }
  cpvbranch->SetAddress(&fCPVRecPoints);
  fCPVRecPoints->Delete();
  cpvbranch->GetEntry(0);
}
