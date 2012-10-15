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
/* $Id: IlcRSTACKTrackSegmentMaker.cxx 25703 2008-05-10 19:18:21Z kharlov $ */

/* History of cvs commits:
 *
 * $Log$
 * Revision 1.29  2007/08/28 12:55:08  policheh
 * Loaders removed from the reconstruction code (C.Cheshkov)
 *
 * Revision 1.28  2007/08/07 14:12:03  kharlov
 * Quality assurance added (Yves Schutz)
 *
 * Revision 1.27  2006/08/25 16:56:30  kharlov
 * Compliance with Effective C++
 *
 * Revision 1.26  2006/08/25 16:00:53  kharlov
 * Compliance with Effective C++IlcRSTACKHit.cxx
 *
 * Revision 1.25  2005/05/28 14:19:05  schutz
 * Compilation warnings fixed by T.P.
 *
 */

//_________________________________________________________________________
// Algorithm Base class to construct RSTACK track segments
// Associates EMC and PPSD clusters
// Unfolds the EMC cluster   
//*-- 
//*-- Author: Dmitri Peressounko (RRC Ki & SUBATECH)


// --- ROOT system ---
#include "TTree.h"

// --- Standard library ---

// --- IlcRoot header files ---
#include "IlcRSTACKTrackSegmentMaker.h"
#include "IlcLog.h"

ClassImp( IlcRSTACKTrackSegmentMaker) 


//____________________________________________________________________________
IlcRSTACKTrackSegmentMaker:: IlcRSTACKTrackSegmentMaker() : 
  TObject(),
  fESD(0), 
  fGeom(0),
  fEMCRecPoints(0),
  fCPVRecPoints(0)
{
 // ctor
  fEMCRecPoints = new TObjArray(100) ;
  fCPVRecPoints = new TObjArray(100) ;
}

//____________________________________________________________________________
IlcRSTACKTrackSegmentMaker::IlcRSTACKTrackSegmentMaker(IlcRSTACKGeometry *geom):
  TObject(),
  fESD(0), 
  fGeom(geom),
  fEMCRecPoints(0),
  fCPVRecPoints(0)
{
  // ctor
  fEMCRecPoints = new TObjArray(100) ;
  fCPVRecPoints = new TObjArray(100) ;
}

//____________________________________________________________________________
IlcRSTACKTrackSegmentMaker::IlcRSTACKTrackSegmentMaker(const IlcRSTACKTrackSegmentMaker & tsmaker) :
  TObject(tsmaker),
  fESD(tsmaker.GetESD()), 
  fGeom(tsmaker.fGeom),
  fEMCRecPoints(tsmaker.fEMCRecPoints),
  fCPVRecPoints(tsmaker.fCPVRecPoints)
{
  //Copy constructor
} 

//____________________________________________________________________________
IlcRSTACKTrackSegmentMaker::~IlcRSTACKTrackSegmentMaker()
{
  //Remove this from the parental task before destroying
  if (fEMCRecPoints) {
    fEMCRecPoints->Delete();
    delete fEMCRecPoints;
  }
  if (fCPVRecPoints) {
    fCPVRecPoints->Delete();
    delete fCPVRecPoints;
  }
}

//____________________________________________________________________________
void IlcRSTACKTrackSegmentMaker::SetInput(TTree *clustersTree)
{
  // Read the clusters tree and set addresses to the
  // arrays with the EMC and CPV clusters

  TBranch *emcbranch = clustersTree->GetBranch("RSTACKEmcRP");
  if (!emcbranch) { 
    IlcError("can't get the branch with the RSTACK EMC clusters !");
    return;
  }
  emcbranch->SetAddress(&fEMCRecPoints);
  fEMCRecPoints->Delete();
  emcbranch->GetEntry(0);

  TBranch *cpvbranch = clustersTree->GetBranch("RSTACKCpvRP");
  if (!cpvbranch) { 
    IlcError("can't get the branch with the RSTACK CPV clusters !");
    return;
  }
  cpvbranch->SetAddress(&fCPVRecPoints);
  fCPVRecPoints->Delete();
  cpvbranch->GetEntry(0);
}
