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
/* $Id: IlcPVBARLink.cxx 17222 2007-03-06 06:53:55Z kharlov $ */
//_________________________________________________________________________
//  Algorithm class used only by IlcPVBARTrackSegmentMaker 
//  Links recpoints into tracksegments
//  Why should I put meaningless comments
//  just to satisfy
//  the code checker                                
//*-- Author: Dmitri Peressounko (SUBATECH)

// --- ROOT system ---

// --- Standard library ---

// --- IlcRoot header files ---

#include "IlcPVBARLink.h"

ClassImp(IlcPVBARLink)
//____________________________________________________________________________
  IlcPVBARLink::IlcPVBARLink() : 
    fEmcN(-1), fCpvN(-1), fTrack(-1), fx(-1.), fz(-1.) 
{
}

//____________________________________________________________________________
IlcPVBARLink::IlcPVBARLink(Float_t dx, Float_t dz, Int_t emc, Int_t cpv, Int_t track):
  fEmcN(emc),
  fCpvN(cpv),
  fTrack(track),
  fx(dx),
  fz(dz)
{
  // ctor
}

//____________________________________________________________________________
Int_t IlcPVBARLink::Compare(const TObject * obj) const
{
  // Compare according to the distance between EMC and CPV RecPoints in a track segment 

  Int_t rv ;

  IlcPVBARLink * link = (IlcPVBARLink *) obj ;

  if(fx*fx+fz*fz < link->fz*link->fz+link->fx*link->fx ) 
    rv = -1 ;
  else 
    rv = 1 ;

  return rv ;
}
