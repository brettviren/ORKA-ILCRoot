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

/* $Id: IlcTriggerCluster.cxx 50636 2011-07-18 11:27:31Z hristov $ */

///////////////////////////////////////////////////////////////////////////////
//
//  Class to define a Trigger Cluster  
//
//  A Trigger Cluster is a group of detector to be trigger together
//
///////////////////////////////////////////////////////////////////////////////

#include <Riostream.h>

#include <TObject.h>
#include <TClass.h>
#include <TString.h>
#include <TMath.h>

#include "IlcLog.h"
#include "IlcDAQ.h"
#include "IlcTriggerCluster.h"
#include "IlcTriggerInput.h"

ClassImp( IlcTriggerCluster )

//_____________________________________________________________________________
IlcTriggerCluster::IlcTriggerCluster(): 
  TNamed(),
  fClusterMask(0)
{
}

//_____________________________________________________________________________
IlcTriggerCluster::IlcTriggerCluster( TString & name, UChar_t index, TString & detectors ) :
  TNamed(name,detectors),
  fClusterMask((index <=6) ? 1 << (index-1) : 0)
{
  TString detClus;
  for( Int_t iDet = 0; iDet < IlcDAQ::kNDetectors; iDet++ ) {
    if( IsSelected( IlcTriggerInput::fgkCTPDetectorName[iDet], fTitle )) {
      // Add the detector
      detClus.Append( " " );
      detClus.Append( IlcDAQ::DetectorName(iDet) );
    }
  }
  SetTitle(detClus.Data());
}

//_____________________________________________________________________________
IlcTriggerCluster::IlcTriggerCluster( const IlcTriggerCluster &clus ):
  TNamed(clus),
  fClusterMask(clus.fClusterMask)
{
  // Copy constructor
}

//_____________________________________________________________________________
Bool_t IlcTriggerCluster::IsDetectorInCluster( TString & detName )
{
   // search for the given detector
   Bool_t result = kFALSE;
   if( (fTitle.CompareTo( detName ) == 0) ||
        fTitle.BeginsWith( detName+" " ) ||
        fTitle.EndsWith( " "+detName ) ||
        fTitle.Contains( " "+detName+" " ) ) {
      result = kTRUE;
   }
   return result;
}


//_____________________________________________________________________________
void IlcTriggerCluster::Print( const Option_t* ) const
{
   // Print
   cout << "Detector Cluster:" << endl;
   cout << "  Name:           " << GetName() << endl;
   cout << "  Cluster index:  " << (Int_t)TMath::Log2(fClusterMask) + 1 << endl;
   cout << "  Detectors:      " << GetDetectorsInCluster() << endl;
}


//////////////////////////////////////////////////////////////////////////////
// Helper method

//_____________________________________________________________________________
Bool_t IlcTriggerCluster::IsSelected( TString detName, TString& detectors ) const
{
   // check whether detName is contained in detectors
   // if yes, it is removed from detectors

   // check if all detectors are selected
   if( (detectors.CompareTo("ALL") == 0 ) ||
        detectors.BeginsWith("ALL ") ||
        detectors.EndsWith(" ALL") ||
        detectors.Contains(" ALL ") ) {
      detectors = "ALL";
      return kTRUE;
   }

   // search for the given detector
   Bool_t result = kFALSE;
   if( (detectors.CompareTo( detName ) == 0) ||
        detectors.BeginsWith( detName+" " ) ||
        detectors.EndsWith( " "+detName ) ||
        detectors.Contains( " "+detName+" " ) ) {
      detectors.ReplaceAll( detName, "" );
      result = kTRUE;
   }

   // clean up the detectors string
   while( detectors.Contains("  ") )  detectors.ReplaceAll( "  ", " " );
   while( detectors.BeginsWith(" ") ) detectors.Remove( 0, 1 );
   while( detectors.EndsWith(" ") )   detectors.Remove( detectors.Length()-1, 1 );

   return result;
}
