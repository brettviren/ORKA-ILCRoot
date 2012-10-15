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
///////////////////////////////////////////////////////////////////////////
//  Virtual base Plane Efficiency class           
//  Specific detector implementation is done in  
//  IlcITSPlaneEff                           
//  IlcTRDPlaneEff                               
//  IlcTOFPlaneEff                               
//
//  Author: G.E. Bruno 
//          giuseppe.bruno@ba.infn.it
//
///////////////////////////////////////////////////////////////////////////


#include <TMath.h>
#include "IlcPlaneEff.h"
#include "IlcLog.h"
//#include "IlcCDBManager.h"
//#include "IlcCDBStorage.h"

ClassImp(IlcPlaneEff)
//______________________________________________________________________
IlcPlaneEff::IlcPlaneEff(): TObject()/*,
fRunNumber(0), 
fCDBUri(""),
fInitCDBCalled(kFALSE)*/
{
    // Default constructor
    // Inputs:
    //    none.
    // Outputs:
    //    none.
    // Return:
    //    a default constructed IlcPlaneEff class
 //InitCDB();
}
//______________________________________________________________________
IlcPlaneEff::IlcPlaneEff(const IlcPlaneEff &s) : TObject(s)/*,
fRunNumber(s.fRunNumber),
fCDBUri(s.fCDBUri),
fInitCDBCalled(s.fInitCDBCalled)*/
{
    //     Copy Constructor
    // Inputs:
    //    const IlcPlaneEff &s  simulation class to copy from
    // Outputs:
    //    none.
    // Return:
    //    a standard constructed IlcPlaneEff class with values the same
    //    as that of s.

}
//_________________________________________________________________________
IlcPlaneEff&  IlcPlaneEff::operator=(const IlcPlaneEff &source){
    //    Assignment operator
    // Inputs:
    //    const IlcPlaneEff &source  simulation class to copy from
    // Outputs:
    //    none.
    // Return:
    //    a standard constructed IlcPlaneEff class with values the same
    //    as that of s.
    if(this != &source){
       source.Copy(*this);
    }
    return *this;
}
//_____________________________________________________________
void IlcPlaneEff::Copy(TObject &/*obj*/) const {
  // copy this to obj
/*  ((IlcPlaneEff& ) obj).fRunNumber		= fRunNumber;
  ((IlcPlaneEff& ) obj).fCDBUri		= fCDBUri;
  ((IlcPlaneEff& ) obj).fInitCDBCalled	= fInitCDBCalled;
*/
}
//_________________________________________________________________________
