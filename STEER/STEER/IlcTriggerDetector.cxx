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

/* $Id: IlcTriggerDetector.cxx 50615 2011-07-16 23:19:19Z hristov $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  Base Class for Detector specific Trigger                                 //                                                                           //
//                                                                           //
//                                                                           //
//                                                                           //
//                                                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <Riostream.h>
#include <TNamed.h>
#include <TString.h>
#include <TObjArray.h>
#include <TRandom.h>


#include "IlcLog.h"
#include "IlcRun.h"
#include "IlcRunLoader.h"
#include "IlcLoader.h"

#include "IlcTriggerInput.h"
#include "IlcTriggerDetector.h"



ClassImp( IlcTriggerDetector )

//_____________________________________________________________________________
IlcTriggerDetector::IlcTriggerDetector() :
  TNamed(),
  fMask(0),
  fInputs()
{
   // Default constructor
}

//_____________________________________________________________________________
IlcTriggerDetector::IlcTriggerDetector(const IlcTriggerDetector & de ):
  TNamed(de),
  fMask(de.fMask),
  fInputs(de.fInputs)
{
  // Copy constructor
  
}

//_____________________________________________________________________________
IlcTriggerDetector::~IlcTriggerDetector()
{
  // Destructor
  // Delete only inputs that are not
  // associated to the CTP
  Int_t ninputs = fInputs.GetEntriesFast();
  for(Int_t i = 0; i < ninputs; i++) {
    IlcTriggerInput *inp = (IlcTriggerInput *)fInputs.At(i);
    if (inp->GetSignature() == -1 &&
	inp->GetMask() == 0)
      delete fInputs.RemoveAt(i);
  }
}

//_____________________________________________________________________________
void IlcTriggerDetector::AssignInputs(const TObjArray &inputs)
{
  // Cross-check the trigger inputs provided by the detector trigger
  // processor (TP) and the inputs defined in CTP
  // a) If the input is defined in the TP, but not in CTP it
  // will be generated but not used by CTP. It will be possibly stored
  // in the detector raw data if the hardware allows this.
  // b) If hte input is not defined in the TP, but is defined in CTP
  // then a warning is issued and the CTP simulation is working
  // with this input disabled.
 
   // Check if we have to create the inputs first
   if( fInputs.GetEntriesFast() == 0 ) {
     // Create the inputs that the detector can provide
     CreateInputs();
   }

   TString name = GetName();

   // Pointer to the available inputs provided by the detector
   TObjArray* availInputs = &fInputs;

   Int_t ninputs = inputs.GetEntriesFast();
   for( Int_t j=0; j<ninputs; j++ ) {
     IlcTriggerInput *inp = (IlcTriggerInput*)inputs.At(j);
     if ( name.CompareTo(inp->GetModule().Data()) ) continue;

     TObject *tempObj = availInputs->FindObject(inp->GetInputName().Data());
     if ( tempObj ) {
       Int_t tempIndex = availInputs->IndexOf(tempObj);
       delete availInputs->Remove(tempObj);
       fInputs.AddAt( inp, tempIndex );
       inp->Enable();
       IlcDebug(1,Form("Trigger input (%s) is found in the CTP configuration. Therefore it is enabled for trigger detector (%s)",
		    inp->GetInputName().Data(),name.Data()));
     }
     else {
       IlcWarning(Form("Trigger Input (%s) is not implemented for the trigger detector (%s) ! It will be disabled !",
		       inp->GetInputName().Data(),name.Data()));
     }
   }

   for( Int_t j=0; j<fInputs.GetEntriesFast(); j++ ) {
     IlcTriggerInput *inp = (IlcTriggerInput *)fInputs.At(j);
     if (inp->GetSignature() == -1 &&
	 inp->GetMask() == 0) {
       inp->Enable();
       IlcDebug(1,Form("Trigger input (%s) was not found in the CTP configuration. Therefore it will be run in a stand-alone mode",
		    inp->GetInputName().Data()));
     }
   }

   fInputs.SetOwner(kFALSE);
}

//_____________________________________________________________________________
void IlcTriggerDetector::CreateInputs()
{
   // Define the inputs to the Central Trigger Processor
   // This is a dummy version 
   
   // Do not create inputs again!!
   if( fInputs.GetEntriesFast() > 0 ) return;
   
   fInputs.AddLast( new IlcTriggerInput( "TEST1_L0", "TEST", 0 ) );
   fInputs.AddLast( new IlcTriggerInput( "TEST2_L0", "TEST", 0 ) );
   fInputs.AddLast( new IlcTriggerInput( "TEST3_L0", "TEST", 0 ) );
}

//_____________________________________________________________________________
void IlcTriggerDetector::Trigger()
{
   // This is a dummy version set all inputs in a random way

   IlcWarning( Form( "Triggering dummy detector %s", GetName() ) );

   //  ********** Get Digits for the current event **********
   IlcRunLoader* runLoader = IlcRunLoader::Instance();
   IlcInfo( Form( "Event %d", runLoader->GetEventNumber() ) );
   
   TString loadername = GetName(); 
   loadername.Append( "Loader" );
   IlcLoader * loader = runLoader->GetLoader( loadername.Data() );
   if( loader ) {
      loader->LoadDigits( "READ" );
      TTree* digits = loader->TreeD();
      // Do something with the digits !!!
      if( digits ) { 
//         digits->Print();
      }   
      loader->UnloadDigits();
   }
   //  ******************************************************

   // set all inputs in a random way
   Int_t nInputs = fInputs.GetEntriesFast();
   for( Int_t j=0; j<nInputs; j++ ) {
      IlcTriggerInput* in = (IlcTriggerInput*)fInputs.At( j );
      if( gRandom->Rndm() > 0.5 ) {
         in->Set();
         fMask |= in->GetValue();
      }
   }
}

//_____________________________________________________________________________
void IlcTriggerDetector::SetInput( TString& name )
{
   // Set Input by name
   SetInput( name.Data() );
}

//_____________________________________________________________________________
void IlcTriggerDetector::SetInput( const char * name )
{
   // Set Input by name
   IlcTriggerInput* in = ((IlcTriggerInput*)fInputs.FindObject( name ));
   if( in ) {
      in->Set();
      fMask |= in->GetValue();
   } else
      IlcError( Form( "There is not input named %s", name ) );
}

//_____________________________________________________________________________
void IlcTriggerDetector::Print( const Option_t* opt ) const
{
   // Print
   cout << "Trigger Detector : " << GetName() << endl;
   cout << "  Trigger Class Mask: 0x" << hex << GetMask() << dec << endl;
   Int_t nInputs = fInputs.GetEntriesFast();
   for( Int_t j=0; j<nInputs; j++ ) {
      IlcTriggerInput* in = (IlcTriggerInput*)fInputs.At( j );
      in->Print( opt );
   }
}
