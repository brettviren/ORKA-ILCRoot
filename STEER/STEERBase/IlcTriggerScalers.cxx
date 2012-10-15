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

/* $Id: IlcTriggerScalers.cxx 22322 2007-11-22 11:43:14Z cvetan $ */

///////////////////////////////////////////////////////////////////////////////
//
//  Class to define the ILC Trigger Scalers  
//
//  For each trigger class there are six scalers:
//
//    LOCB       L0 triggers before any vetos 
//    LOCA       L0 triggers after all vetos 
//    L1CB       L1 triggers before any vetos 
//    L1CA       L1 triggers after all vetos 
//    L2CB       L2 triggers before any vetos 
//    L2CA       L2 triggers after all vetos 
//
//////////////////////////////////////////////////////////////////////////////

#include <Riostream.h>

#include "IlcLog.h"
#include "IlcTriggerScalers.h"

ClassImp( IlcTriggerScalers )

//_____________________________________________________________________________
IlcTriggerScalers::IlcTriggerScalers(): 
  TObject(),
  fClassIndex(0),
  fLOCB(0),     
  fLOCA(0),     
  fL1CB(0),     
  fL1CA(0),     
  fL2CB(0),     
  fL2CA(0)      
{
  // Default constructor
}

//_____________________________________________________________________________
IlcTriggerScalers::IlcTriggerScalers( UChar_t classIndex, UInt_t LOCB, UInt_t LOCA,        
                                      UInt_t L1CB, UInt_t L1CA, UInt_t L2CB, UInt_t L2CA ):   
  TObject(),
  fClassIndex( classIndex ),
  fLOCB(LOCB),     
  fLOCA(LOCA),     
  fL1CB(L1CB),     
  fL1CA(L1CA),     
  fL2CB(L2CB),     
  fL2CA(L2CA)      
{
  // Default constructor
}

//_____________________________________________________________________________
Int_t  IlcTriggerScalers::Compare( const TObject* obj ) const
{
  // Compare Scaler by class index (to sort in Scaler Record by class index)
  if( fClassIndex < ((IlcTriggerScalers*)obj)->fClassIndex ) return -1;
  if( fClassIndex > ((IlcTriggerScalers*)obj)->fClassIndex ) return 1;
  return 0;
}
//____________________________________________________________________________
void IlcTriggerScalers::GetAllScalers(UInt_t *scalers) const
{
 scalers[0]=fLOCB;
 scalers[1]=fLOCA;
 scalers[2]=fL1CB;
 scalers[3]=fL1CA;
 scalers[4]=fL2CB;
 scalers[5]=fL2CA;
}
//_____________________________________________________________________________
void IlcTriggerScalers::Print( const Option_t* ) const
{
   // Print
  cout << "Trigger Scalers for Class: " << (Int_t)fClassIndex << endl;
  cout << "  LOCB: " << fLOCB << " LOCA: " << fLOCA; //<< endl;
  cout << "  L1CB: " << fL1CB << " L1CA: " << fL1CA; //<< endl;
  cout << "  L2CB: " << fL2CB << " L2CA: " << fL2CA << endl;
}
