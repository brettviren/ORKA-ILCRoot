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

/* $Id: IlcCentralTrigger.cxx 50615 2011-07-16 23:19:19Z hristov $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// This class for running the Central Trigger Processor                      //
//                                                                           //
//                                                                           //
//    Load Configuration                                                     //
//    Make a list the trigger detectors involved (from the classes)          //
//    For the each event                                                     //
//           Run the Trigger for the each detector                           //
//           Get the inputs                                                  //
//           Check the trigger classes                                       //
//           Create the class mask                                           //
//           Save result                                                     //
//                                                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <TString.h>
#include <TObjString.h>
#include <TObjArray.h>
#include <TStopwatch.h>
#include <TFile.h>
#include <TTree.h>

#include "IlcLog.h"
#include "IlcRun.h"
#include "IlcRunLoader.h"
#include "IlcLoader.h"
#include "IlcModule.h"

#include "IlcTriggerInput.h"
#include "IlcTriggerDetector.h"
#include "IlcTriggerConfiguration.h"
#include "IlcTriggerClass.h"
#include "IlcTriggerCluster.h"
#include "IlcCentralTrigger.h"
#include "IlcDetectorEventHeader.h"
#include "IlcHeader.h"

#include "IlcCDBManager.h"
#include "IlcCDBPath.h"
#include "IlcCDBEntry.h"

ClassImp( IlcCentralTrigger )

//_____________________________________________________________________________
IlcCentralTrigger::IlcCentralTrigger() :
   TObject(),
   fClassMask(0),
   fClusterMask(0),
   fL0TriggerInputs(0),
   fL1TriggerInputs(0),
   fL2TriggerInputs(0),
   fConfiguration(NULL)
{
   // Default constructor
  SetOwner();
}

//_____________________________________________________________________________
IlcCentralTrigger::IlcCentralTrigger( TString & config ) :
   TObject(),
   fClassMask(0),
   fClusterMask(0),
   fL0TriggerInputs(0),
   fL1TriggerInputs(0),
   fL2TriggerInputs(0),
   fConfiguration(NULL)
{
   // Default constructor
   LoadConfiguration( config );
}

//_____________________________________________________________________________
IlcCentralTrigger::~IlcCentralTrigger()
{
  // Destructor
  DeleteConfiguration();
}

//_____________________________________________________________________________
void IlcCentralTrigger::DeleteConfiguration()
{
  // Delete the active configuration
  fClassMask = 0;
  fClusterMask = 0;
  fL0TriggerInputs = 0;
  fL1TriggerInputs = 0;
  fL2TriggerInputs = 0;
  if (fConfiguration) {
    if (IsOwner()) delete fConfiguration;
    fConfiguration = 0x0;
  }
}

//_____________________________________________________________________________
void IlcCentralTrigger::Reset()
{
   // Reset Class Mask and classes
   fClassMask = 0;
   fClusterMask = 0;
   fL0TriggerInputs = 0;
   fL1TriggerInputs = 0;
   fL2TriggerInputs = 0;

   if (fConfiguration) {
     const TObjArray& classesArray = fConfiguration->GetClasses();
     Int_t nclasses = classesArray.GetEntriesFast();
     for( Int_t j=0; j<nclasses; j++ ) {
       IlcTriggerClass* trclass = (IlcTriggerClass*)classesArray.At( j );
       trclass->Reset();
     }
   }
}

//_____________________________________________________________________________
void IlcCentralTrigger::MakeBranch( TString name, TTree * tree )
{
   // Make a branch to store only trigger class mask event by event

   if( tree )  {
      IlcDebug( 1, "Got Tree from folder." );
      TBranch* branch = tree->GetBranch( name );
      if( branch == 0x0 ) {
         IlcDebug( 1, "Creating new branch" );
         branch = tree->Branch( name, &(this->fClassMask), "fClassMask/l:fClusterMask/i:fL0TriggerInputs/i:fL1TriggerInputs/i:fL2TriggerInputs/s" );
         branch->SetAutoDelete( kFALSE );
      }
      else {
         IlcDebug( 1, "Got Branch from Tree" );
         branch->SetAddress( &(this->fClassMask) );
      }
   }
}

//_____________________________________________________________________________
Bool_t IlcCentralTrigger::LoadConfiguration( TString & config )
{
   // Load one and only one pre-created COnfiguration from database/file that match
   // with the input string 'config'
   // Ej: "p-p", "Pb-Pb" or "p-p-DIMUON CALIBRATION-CENTRAL-BARREL"

   // Delete the active configuration, if any
  DeleteConfiguration();

   // Load the selected configuration
   if (!config.IsNull()) {
     fConfiguration = IlcTriggerConfiguration::LoadConfiguration( config );
     SetOwner();
     if(fConfiguration)
       return kTRUE;
     else {
       IlcError( Form( "Valid TriggerConfiguration (%s) is not found ! Disabling the trigger simulation !", config.Data() ) );
       return kFALSE;
     }
   }
   else {
     // Load one and only one trigger descriptor from CDB
     IlcInfo( "Getting trigger configuration from OCDB!" );
 
     IlcCDBPath path( "GRP", "CTP", "Config" );
	
     IlcCDBEntry *entry=IlcCDBManager::Instance()->Get(path.GetPath());
     SetOwner(kFALSE);
     if( !entry ) IlcFatal( "Couldn't load trigger description data from CDB!" );

     fConfiguration = (IlcTriggerConfiguration *)entry->GetObject();
     if(fConfiguration)
       return kTRUE;
     else {
       IlcError( "No valid configuration is found in the CDB ! Disabling the trigger simulation !" );
       return kFALSE;
     }
   }
}

//_____________________________________________________________________________
TString IlcCentralTrigger::GetDetectors()
{
   // return TString with the detectors (modules) to be used for triggering

   TString result;

   if (fConfiguration)
     result = fConfiguration->GetTriggeringModules();

   return result;
}

//_____________________________________________________________________________
Bool_t IlcCentralTrigger::RunTrigger( IlcRunLoader* runLoader, const char *detectors )
{
   // run the trigger

   if( !fConfiguration ) {
      IlcError( "No trigger configuration loaded, skipping trigger" );
      return kFALSE;
   }

   TTree *tree = runLoader->TreeCT();
   if( !tree ) {
      IlcError( "No folder with trigger loaded, skipping trigger" );
      return kFALSE;
   }

   TStopwatch stopwatch;
   stopwatch.Start();

   IlcInfo( Form(" Triggering Detectors: %s \n", GetDetectors().Data() ) );
   IlcInfo( Form(" Detectors with digits: %s \n", detectors ) );

   // Process each event
   for( Int_t iEvent = 0; iEvent < runLoader->GetNumberOfEvents(); iEvent++ ) {
      runLoader->GetEvent( iEvent );
      // Get detectors involve
      TString detStr = GetDetectors();
      TString detWithDigits = detectors;
      TObjArray* detArray = runLoader->GetIlcRun()->Detectors();
      // Reset Mask
      fClassMask = 0;
      fClusterMask = 0;
      // Reset configuration object (inputs and classes)
      fConfiguration->Reset();
      TObjArray trgdetArray; // use as garbage collector
      for( Int_t iDet = 0; iDet < detArray->GetEntriesFast(); iDet++ ) {
         IlcModule* det = (IlcModule*) detArray->At( iDet );
         if( !det || !det->IsActive() ) continue;
         if( IsSelected(det->GetName(), detStr) &&
	     IsSelected(det->GetName(), detWithDigits) ) {

	    IlcDebug(1,Form("Triggering from digits for %s", det->GetName() ) );
            IlcTriggerDetector* trgdet = det->CreateTriggerDetector();
            trgdet->AssignInputs(fConfiguration->GetInputs());
            TStopwatch stopwatchDet;
            stopwatchDet.Start();
            trgdet->Trigger();
            IlcDebug(1, Form("Execution time for %s: R:%.2fs C:%.2fs",
                     det->GetName(), stopwatchDet.RealTime(), stopwatchDet.CpuTime() ) );

            trgdetArray.AddLast( trgdet );

            // Write trigger detector in Event folder in Digits file
            TString loadername = det->GetName();
            loadername.Append( "Loader" );
            IlcLoader * loader = runLoader->GetLoader( loadername );
            if( loader ) {
               IlcDataLoader * dataLoader = loader->GetDigitsDataLoader();
               if( !dataLoader->IsFileOpen() ) {
                  if( dataLoader->OpenFile( "UPDATE" ) ) {
                     IlcWarning( Form( "\n\nCan't write trigger for %s\n", det->GetName() ) );
                  }
               }
               dataLoader->Cd();
               if( gFile && !gFile->IsWritable() ) {
                  gFile->ReOpen( "UPDATE" );
                  dataLoader->Cd();
               }
               trgdet->Write( "Trigger", TObject::kOverwrite );
               dataLoader->CloseFile();
            }
            else  IlcWarning( Form( "Not loader found for %s", det->GetName() ) );
         }
      }

      // Check trigger conditions and create the trigger class mask
      TriggerClasses();
      // Calculate trigger Input pattern
      TriggerInputs();

      // Clear trigger detectors
      trgdetArray.SetOwner();
      trgdetArray.Delete();

      if( (detStr.CompareTo( "ALL" ) != 0) && !detStr.IsNull() ) {
         IlcError( Form("the following detectors were not found: %s",
                   detStr.Data()));
         //JF return kFALSE;
      }

      // Save trigger mask
      tree->Fill();
      IlcDebug(1, Form("Event:%d  Class Mask:0x%llX", iEvent,fClassMask ) );
   } // end event loop

   Reset();
//   cout << endl <<  " Print " << endl;
//   Print();

   // Write
   runLoader->WriteTrigger( "OVERWRITE" );

   return kTRUE;
}
//----------------------------------------------------------------------------
void IlcCentralTrigger::TriggerInputs()
{
 // Find which inputs are in configuration
 // and calculate input pattern
 fL0TriggerInputs=0;
 fL1TriggerInputs=0;
 fL2TriggerInputs=0;
 if(fConfiguration){
    const TObjArray& inputsArray = fConfiguration->GetInputs();
    Int_t ninputs = inputsArray.GetEntriesFast();
    for( Int_t j=0; j<ninputs; j++ ) {
      IlcTriggerInput* input = (IlcTriggerInput*)inputsArray.At( j );
      if(input->GetValue()){
       UChar_t level=input->GetLevel();
            if(level == 0) fL0TriggerInputs |= (input->GetMask());
       else if(level == 1) fL1TriggerInputs |= (input->GetMask());
       else if(level == 2) fL2TriggerInputs |= (input->GetMask());
       else{
         IlcError(Form("Unknown input level:%c:",level));
       }
      }
    }
 }
}
//_____________________________________________________________________________
ULong64_t IlcCentralTrigger::TriggerClasses()
{
  // Check trigger conditions and create the trigger class
  // and trigger cluster masks
  fClassMask = 0;
  fClusterMask = 0;
  if (fConfiguration) {
    const TObjArray& classesArray = fConfiguration->GetClasses();
    Int_t nclasses = classesArray.GetEntriesFast();
    for( Int_t j=0; j<nclasses; j++ ) {
      IlcTriggerClass* trclass = (IlcTriggerClass*)classesArray.At( j );
      trclass->Trigger( fConfiguration->GetInputs(), fConfiguration->GetFunctions() );
      fClassMask |= trclass->GetValue();
      if (trclass->GetStatus()) {
	IlcTriggerCluster *trclust = trclass->GetCluster();
	fClusterMask |= IlcDAQ::DetectorPattern(trclust->GetDetectorsInCluster());
      }
    }
  }
  return fClassMask;
}
//_____________________________________________________________________________
TObjArray* IlcCentralTrigger::GetFiredClasses() const
{
   // return only the true conditions

   TObjArray* result = new TObjArray();

   if (fConfiguration) {
     const TObjArray& classesArray = fConfiguration->GetClasses();
     Int_t nclasses = classesArray.GetEntriesFast();
     for( Int_t j=0; j<nclasses; j++ ) {
       IlcTriggerClass* trclass = (IlcTriggerClass*)classesArray.At( j );
       if( trclass->GetStatus() ) result->AddLast( trclass );
     }
   }

   return result;
}

//_____________________________________________________________________________
void IlcCentralTrigger::Print( const Option_t*  ) const
{
   // Print
   cout << "Central Trigger: " << endl;
   cout << "  Trigger Class Mask: 0x" << hex << fClassMask << dec << endl;
   if (fConfiguration) fConfiguration->Print();
   cout << endl;
}


//////////////////////////////////////////////////////////////////////////////
// Helper method

//_____________________________________________________________________________
Bool_t IlcCentralTrigger::IsSelected( TString detName, TString& detectors ) const
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

//_____________________________________________________________________________
Bool_t IlcCentralTrigger::CheckTriggeredDetectors() const
{
  // Check the trigger mask, finds which trigger classes
  // have been fired, load the corresponding trigger clusters and
  // finally makes a list of the detectors that have been readout
  // for each particular event. This list is then compared to the
  // one stored in fClusterMask. Return value:
  // true = two lists are equal
  // false = two lists are not equal meaning wrong trigger config
  // is loaded.

  if (!fConfiguration) {
    IlcError("The trigger confiration has not yet been loaded! Cross-check is not possible!");
    return kFALSE;
  }
  else {

    // Make a cross-check so that to exclude wrong trigger configuration used
    // Loop over the trigger classes
    UInt_t clusterMask = 0;
    const TObjArray& classesArray = fConfiguration->GetClasses();
    Int_t nclasses = classesArray.GetEntriesFast();
    for( Int_t j=0; j<nclasses; j++ ) {
      IlcTriggerClass* trclass = (IlcTriggerClass*)classesArray.At( j );
      if (trclass->GetMask() & fClassMask) { // class was fired
	IlcTriggerCluster *trclust = trclass->GetCluster();
	clusterMask |= IlcDAQ::DetectorPattern(trclust->GetDetectorsInCluster());
      }
    }
    // Compare the stored cluster mask with the one
    // that we get from trigger classes
    if (clusterMask != fClusterMask) {
      if ((clusterMask & fClusterMask) == clusterMask) {
	IlcInfo(Form("Cluster mask from trigger classes (%x) and from data (%x) differ. Concurrent DAQ run(s) could be the reason.",
		     (UInt_t)clusterMask,(UInt_t)fClusterMask));
	return kTRUE;
      }
      else {
	IlcError(Form("Wrong cluster mask from trigger classes (%x), expecting (%x)! Loaded trigger configuration is possibly wrong!",
		      (UInt_t)clusterMask,(UInt_t)fClusterMask));
	return kFALSE;
      }
    }
  }

  return kTRUE;
}
