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
// This class which defines the trigger interaction objects
//
//
///////////////////////////////////////////////////////////////////////////////

#include <Riostream.h>
#include <TObjArray.h>
#include <TObjString.h>

#include "IlcTriggerInteraction.h"
#include "IlcTriggerInput.h"
#include "IlcExpression.h"
#include "IlcLog.h"

ClassImp(IlcTriggerInteraction)

//_____________________________________________________________________________
IlcTriggerInteraction::IlcTriggerInteraction():
  TNamed()
{
  // Default constructor
}

//_____________________________________________________________________________
IlcTriggerInteraction::IlcTriggerInteraction( TString & name, TString &logic ):
  TNamed( name, logic )
{
  // Constructor
}
//_____________________________________________________________________________
IlcTriggerInteraction::~IlcTriggerInteraction() 
{ 
  // Destructor
}
//_____________________________________________________________________________
IlcTriggerInteraction::IlcTriggerInteraction( const IlcTriggerInteraction& interact ):
  TNamed( interact )
{
   // Copy constructor
}

//______________________________________________________________________________
IlcTriggerInteraction& IlcTriggerInteraction::operator=(const IlcTriggerInteraction& interact)
{
   // IlcTriggerInteraction assignment operator.

   if (this != &interact) {
      TNamed::operator=(interact);
   }
   return *this;
}

//_____________________________________________________________________________
Bool_t IlcTriggerInteraction::CheckInputs(const TObjArray &inputs) const
{
  // Check the existance of trigger inputs
  // and the logic used.
  // Return false in case of wrong interaction
  // definition.

  TString logic( GetTitle() );
  TObjArray* tokens = logic.Tokenize(" !&|()\t");

  Int_t ntokens = tokens->GetEntriesFast();
  for( Int_t i=0; i<ntokens; i++ ) {
    TObjString* iname = (TObjString*)tokens->At( i );

    IlcTriggerInput *inp = (IlcTriggerInput*)inputs.FindObject(iname->String().Data());
    if (!inp) {
      IlcError( Form( "The trigger input (%s) is not available for Interaction (%s)",
		      iname->String().Data(), GetName() ) );
      delete tokens;
      return kFALSE;
    }
    if (inp->GetMask() == 0 || inp->GetMask() > (1<<24)) { // New l0f can use all inputs
      IlcError( Form( "The trigger input (%s) is not among the first 4 trigger inputs used to create interactions. Interaction (%s) is invalid",
		      iname->String().Data(), GetName() ) ); 
      delete tokens;
      return kFALSE;
    }
  }

  delete tokens;
  return kTRUE;
}

//_____________________________________________________________________________
Bool_t IlcTriggerInteraction::IsActive(const TObjArray &inputs) const
{
  // Check if the trigger inputs
  // are active
  // Return false in one or more inputs
  // are disabled

  TString logic( GetTitle() );
  TObjArray* tokens = logic.Tokenize(" !&|()\t");

  Int_t ntokens = tokens->GetEntriesFast();
  for( Int_t i=0; i<ntokens; i++ ) {
    TObjString* iname = (TObjString*)tokens->At( i );

    IlcTriggerInput *inp = (IlcTriggerInput *)inputs.FindObject(iname->String());
    if (!inp) {
      IlcError( Form( "The trigger input (%s) is not available for Interaction (%s)",
		      iname->String().Data(), GetName() ) );
      delete tokens;
      return kFALSE;
    }
    else {
      if (!inp->IsActive()) {
	IlcWarning(Form("The interaction/function (%s) will be disabled, because the input (%s) is disabled",
			GetName(),iname->String().Data()));
	delete tokens;
	return kFALSE;
      }
    }
  }

  delete tokens;
  return kTRUE;
}

//_____________________________________________________________________________
Bool_t IlcTriggerInteraction::Trigger(const TObjArray& inputs ) const
{
  // Check if the inputs satify the interaction expression condition 
  IlcExpression* exp = new IlcExpression( GetTitle() );
  Bool_t status = exp->Value( inputs );
  delete exp;
  return status;
}

//_____________________________________________________________________________
void IlcTriggerInteraction::Print( const Option_t* ) const
{
   // Print
  cout << "Trigger Interaction:" << endl;
  cout << "  Name:              " << GetName() << endl;
  cout << "  Logic:             " << GetTitle() << endl;
}
