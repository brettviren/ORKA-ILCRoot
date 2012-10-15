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
//
// This class which defines the trigger descriptor objects
//
//
///////////////////////////////////////////////////////////////////////////////

#include <Riostream.h>
#include <TObjArray.h>
#include <TObjString.h>

#include "IlcLog.h"
#include "IlcTriggerDescriptor.h"
#include "IlcTriggerInput.h"
#include "IlcTriggerInteraction.h"

ClassImp(IlcTriggerDescriptor)

//_____________________________________________________________________________
IlcTriggerDescriptor::IlcTriggerDescriptor():
  TNamed()
{
  // Default constructor
}

//_____________________________________________________________________________
IlcTriggerDescriptor::IlcTriggerDescriptor( TString & name, TString &cond ):
  TNamed( name, cond )
{
  // Constructor
}
//_____________________________________________________________________________
IlcTriggerDescriptor::~IlcTriggerDescriptor() 
{ 
  // Destructor
}
//_____________________________________________________________________________
IlcTriggerDescriptor::IlcTriggerDescriptor( const IlcTriggerDescriptor& desc ):
  TNamed( desc )
{
   // Copy constructor
}

//______________________________________________________________________________
IlcTriggerDescriptor& IlcTriggerDescriptor::operator=(const IlcTriggerDescriptor& desc)
{
   // IlcTriggerDescriptor assignment operator.

   if (this != &desc) {
      TNamed::operator=(desc);
   }
   return *this;
}

//_____________________________________________________________________________
Bool_t IlcTriggerDescriptor::CheckInputsAndFunctions(const TObjArray &inputs, const TObjArray &functions) const
{
  // Check the existance of trigger inputs and functions
  // and the logic used.
  // Return false in case of wrong interaction
  // definition.

   TString condition( GetTitle() );
   TObjArray* tokens = condition.Tokenize(" !&|()\t");

   Bool_t IsInput = kFALSE;

   Int_t ntokens = tokens->GetEntriesFast();
   for( Int_t i=0; i<ntokens; i++ ) {
      TObjString* iname = (TObjString*)tokens->At( i );
      if (functions.FindObject(iname->String())) {
	// Logical function of the first 4 inputs
	if (IsInput) {
	  IlcError("Logical functions can not follow inputs, they are always declared first !");
	  delete tokens;
	  return kFALSE;
	}
	IsInput = kFALSE;
	continue;
      }
      if (inputs.FindObject(iname->String())) {
	// already a trigger input
	IsInput = kTRUE;
	continue;
      }
      IlcError(Form("Invalid trigger input or function (%s)",iname->String().Data()));
      delete tokens;
      return kFALSE;
   }

   delete tokens;
   return kTRUE;
}

//_____________________________________________________________________________
Bool_t IlcTriggerDescriptor::IsActive(const TObjArray &inputs, const TObjArray &functions) const
{
  // Check if the trigger inputs and functions
  // are active
  // Return false in case one or more inputs
  // are disabled
   TString condition( GetTitle() );
   TObjArray* tokens = condition.Tokenize(" !&|()\t");

   Int_t ntokens = tokens->GetEntriesFast();
   for( Int_t i=0; i<ntokens; i++ ) {
      TObjString* iname = (TObjString*)tokens->At( i );
      IlcTriggerInteraction *interact = (IlcTriggerInteraction *)functions.FindObject(iname->String());
      if (interact) {
	if (!interact->IsActive(inputs)) {
	  IlcWarning(Form("The descriptor (%s) will be disabled, because the function (%s) is disabled",
			  GetName(),iname->String().Data()));
	  delete tokens;
	  return kFALSE;
	}
	continue;
      }
      IlcTriggerInput *inp = (IlcTriggerInput *)inputs.FindObject(iname->String());
      if (inp) {
	if (!inp->IsActive()) {
	  IlcWarning(Form("The descriptor (%s) will be disabled, because the input (%s) is disabled",
			  GetName(),iname->String().Data()));
	  delete tokens;
	  return kFALSE;
	}
	continue;
      }
      IlcError(Form("Desciptor (%s) contains invalid trigger input or function (%s)",
		    GetName(),iname->String().Data()));
      delete tokens;
      return kFALSE;
   }

   delete tokens;
   return kTRUE;

}

//_____________________________________________________________________________
Bool_t IlcTriggerDescriptor::Trigger( const TObjArray &inputs, const TObjArray &functions) const
{
  // Check if the inputs and functions 
  // satify the descriptor conditions 

  TString condition( GetTitle() );
  TObjArray* tokens = condition.Tokenize(" !&|()\t");

  Int_t ntokens = tokens->GetEntriesFast();
  for( Int_t i=0; i<ntokens; i++ ) {
    TObjString* iname = (TObjString*)tokens->At( i );
    IlcTriggerInteraction *interact = (IlcTriggerInteraction *)functions.FindObject(iname->String());
    if (interact) {
      if (!interact->Trigger(inputs)) {
	delete tokens;
	return kFALSE;
      }
      continue;
    }
    IlcTriggerInput *inp = (IlcTriggerInput *)inputs.FindObject(iname->String());
    if (inp) {
      if (!inp->Status()) {
	delete tokens;
	return kFALSE;
      }
      continue;
    }
    IlcError(Form("Desciptor (%s) contains invalid trigger input or function (%s)",
		  GetName(),iname->String().Data()));
    delete tokens;
    return kFALSE;
  }

  delete tokens;
  return kTRUE;

}

//_____________________________________________________________________________
void IlcTriggerDescriptor::Print( const Option_t* ) const
{
   // Print
  cout << "Trigger Descriptor:" << endl;
  cout << "  Name:             " << GetName() << endl;
  cout << "  Logic:            " << GetTitle() << endl;
}
