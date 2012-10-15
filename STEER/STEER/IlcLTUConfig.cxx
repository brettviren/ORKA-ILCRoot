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

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  Class that describes a detector LTU configuration                        //
//                                                                           //
//                                                                           //
//  Presently we store a subset of the LTU parameters:                       //
//  FineDelay1 3126 # picosec                                                //
//  FineDelay2 20459 # picosec                                               //
//  BC_DELAY_ADD 18 # ns                                                     //
//                                                                           //
//  cvetan.cheshkov@cern.ch 3/9/2010                                         //
///////////////////////////////////////////////////////////////////////////////

#include <Riostream.h>

#include "IlcLog.h"
#include "IlcDAQ.h"
#include "IlcLTUConfig.h"

ClassImp( IlcLTUConfig )

//_____________________________________________________________________________
void IlcLTUConfig::Print( const Option_t* ) const
{
   // Print
   cout << "LTU Config:" << endl; 
   cout << "     Detector:  " << GetDetectorName() << "(Id=" << (Int_t)fDetectorId << ")" << endl;
   cout << "   FineDelay1:  " << fFineDelay1 << " ns" << endl;
   cout << "   FineDelay2:  " << fFineDelay2 << " ns" << endl;
   cout << "   BCDelayAdd:  " << fBCDelayAdd << " ns" << endl;
}

//_____________________________________________________________________________
const char* IlcLTUConfig::GetDetectorName() const
{
  // Get the detector name (DAQ naming scheme)
  return IlcDAQ::DetectorName((Int_t)fDetectorId);
}
