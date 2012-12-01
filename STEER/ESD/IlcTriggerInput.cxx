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

/* $Id: IlcTriggerInput.cxx 52786 2011-11-12 19:17:12Z lietava $ */

///////////////////////////////////////////////////////////////////////////////
//
//  Class to define a Trigger Input from an specific detector                                                                                           //
//
//
//                        name         description     id mask
//    Ej:
//      IlcTriggerInput( "V0_MB_L0", "VO minimum bias", 0x01 );
//      IlcTriggerInput( "V0_SC_L0", "VO semi central", 0x02 );
//      IlcTriggerInput( "V0_C_L0",  "VO central",      0x04 );
//
//    The name must be globaly unique. Spaces are not allowed.
//    As convention should start with detector name then an id
//    and the trigger level (L0, L1, L2) separated by "_"
//    for valid detector names see IlcTriggerCluster::fgkDetectorName
//
//    A maximun of 60 inputs trigger are allow.
//    So, the id mask should set only bit from the position 1 to 60.
//
///////////////////////////////////////////////////////////////////////////////

#include <Riostream.h>
#include <TMath.h>

#include "IlcLog.h"
#include "IlcTriggerInput.h"

ClassImp( IlcTriggerInput )

Bool_t IlcTriggerInput::fgkIsTriggerDetector[IlcDAQ::kNDetectors] = {1,0,0,1,1,1,1,0,1,0,0,0,1,0,1,1,1,1,1,1,1,0};
const char* IlcTriggerInput::fgkCTPDetectorName[IlcDAQ::kNDetectors] = {
  "SPD",
  "SDD",
  "SSD",
  "TPC",
  "TRD",
  "DCH",
  "TOF",
  "HMPID",
  "PHOS",
  "CPV",
  "PMD",
  "MUON_TRK",
  "MUON_TRG",
  "FMD",
  "T0",
  "V0",
  "ZDC",
  "ACORDE",
  "CTP",
  "EMCal",
  "DAQ_TEST",
  "HLT"
};
//_______________________________________________________________________
#ifdef WIN32
   const char* IlcTriggerInput::CTPDetectorName(Int_t iType) 
    {return fgkCTPDetectorName[iType];};
#endif

//_____________________________________________________________________________
  IlcTriggerInput::IlcTriggerInput( TString name, TString det, UChar_t level, Int_t signature, Char_t number ):
    TNamed( name.Data(), det.Data() ),
    fMask((number > 0) ? 1 << (number-1) : 0 ),
    fValue(0),
    fSignature(signature),
    fLevel(level),
    fDetectorId(-1),
    fIsActive(kFALSE)
{
   //  Standard constructor
   //
   //    The name must be globaly unique. Spaces are not allowed.
   //    For valid detector names see IlcDAQ::fgkDetectorName

   // Check for valid detector name
   Int_t iDet = 0;
   for( iDet = 0; iDet < IlcDAQ::kNDetectors; iDet++ ) {
     if( !fgkIsTriggerDetector[iDet] ) continue;
      if( det.CompareTo( fgkCTPDetectorName[iDet] ) == 0 ) {
	fTitle = IlcDAQ::DetectorName(iDet);
	fDetectorId = iDet;
	break;
      }
      if( det.CompareTo( IlcDAQ::DetectorName(iDet) ) == 0 ) {
	fDetectorId = iDet;
	break;
      }
   }
   if( iDet == IlcDAQ::kNDetectors ) {
      IlcError( Form( "%s is not a valid trigger input, it must contain a valid trigger detector name instead of (%s)", name.Data(), det.Data() ) );
   }
}

//_____________________________________________________________________________
void IlcTriggerInput::Print( const Option_t* ) const
{
   // Print
   cout << "Trigger Input:" << endl; 
   cout << "  Name:        " << GetName() << endl;
   cout << "  Detector:    " << GetTitle() << "(Id=" << (Int_t)fDetectorId << ")" << endl;
   cout << "  Level:       " << (Int_t)fLevel << endl;
   cout << "  Signature:   " << fSignature << endl;
   if(fMask)
   cout << "  Number:      " << (Int_t)TMath::Log2(fMask) << endl;
   else cout << " fMask:  " << fMask << endl;
   if (IsActive())
     cout << "   Input is active      " << endl;
   else
     cout << "   Input is not active  " << endl;
   if (Status())
     cout << "   Input is fired      " << endl;
   else
     cout << "   Input is not fired  " << endl;
}

//_____________________________________________________________________________
TString IlcTriggerInput::GetModule() const
{
  // Get the detector module name (in IlcRoot simulation sense)
  TString name = "";
  if (fDetectorId >= 0 && fDetectorId < IlcDAQ::kNDetectors)
    name = IlcDAQ::OfflineModuleName((Int_t)fDetectorId);
  else
    IlcError(Form("Invalid detector Id (%d)",(Int_t)fDetectorId));

  return name;
}
