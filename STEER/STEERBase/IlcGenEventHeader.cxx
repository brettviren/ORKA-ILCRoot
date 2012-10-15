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

/* $Id: IlcGenEventHeader.cxx 50615 2011-07-16 23:19:19Z hristov $ */

//---------------------------------------------------------------------
// Event header base class for generator. 
// Stores as a minimum the date, run number, event number,
// number of particles produced  
// and the impact parameter.
// Author: andreas.morsch@cern.ch
//---------------------------------------------------------------------

#include "IlcGenEventHeader.h"
ClassImp(IlcGenEventHeader)


//_______________________________________________________________________
IlcGenEventHeader::IlcGenEventHeader():
  fNProduced(-1),
  fVertex(3),
  fInteractionTime(0.),
  fEventWeight(1.)
{
  //
  // Constructor
  //
}

//_______________________________________________________________________
IlcGenEventHeader::IlcGenEventHeader(const char * name):
  TNamed(name, "Event Header"),
  fNProduced(-1),
  fVertex(3),
  fInteractionTime(0.),
  fEventWeight(1.)
{
  //
  // Constructor
  //
}

//_______________________________________________________________________
void IlcGenEventHeader::SetPrimaryVertex(const TArrayF &o)
{
    //
    // Set the primary vertex for the event
    //
    fVertex[0]=o.At(0);
    fVertex[1]=o.At(1);
    fVertex[2]=o.At(2);
}

//_______________________________________________________________________
void  IlcGenEventHeader::PrimaryVertex(TArrayF &o) const
{
    //
    // Return the primary vertex for the event
    //
    o.Set(3);
    o[0] = fVertex.At(0);
    o[1] = fVertex.At(1);
    o[2] = fVertex.At(2);    
}

