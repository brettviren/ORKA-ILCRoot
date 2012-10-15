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
// Jet header base class 
// Stores a comment which describes the jet analysis
// Author: jgcn@mda.cinvestav.mx
//---------------------------------------------------------------------

#include <Riostream.h> 
#include "IlcPxconeJetHeader.h"
ClassImp(IlcPxconeJetHeader)
 
 
////////////////////////////////////////////////////////////////////////

IlcPxconeJetHeader::IlcPxconeJetHeader():
  IlcJetHeader("IlcPxconeJetHeader"),
  fMode(2),
  fRadius(0.3),
  fMinPt(10),
  fOverlap(0.75)
{
  // Default constructor
  SetMode();
  SetRadius();
  SetMinPt();
  SetOverlap();
}
 

////////////////////////////////////////////////////////////////////////
 
void IlcPxconeJetHeader::PrintParameters() const

{
  // prints out parameters of jet algorithm
  cout << " PXCONE jet algorithm " << endl;
  cout << "  Running mode: " << fMode << endl;
  cout << "  Cone size: " << fRadius << endl;
  cout << "  Minimum jet energy: " << fMinPt << endl;
  cout << "  Overlap fraction: " << fOverlap << endl;
}
