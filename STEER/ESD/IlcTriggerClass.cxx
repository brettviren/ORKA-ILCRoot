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
// This class which defines the trigger classes objects
//
//
///////////////////////////////////////////////////////////////////////////////
#include <Riostream.h>
#include <TMath.h>

#include "IlcLog.h"
#include "IlcTriggerClass.h"
#include "IlcTriggerConfiguration.h"
#include "IlcTriggerDescriptor.h"
#include "IlcTriggerCluster.h"
#include "IlcTriggerPFProtection.h"
#include "IlcTriggerBCMask.h"

ClassImp(IlcTriggerClass)

//_____________________________________________________________________________
IlcTriggerClass::IlcTriggerClass():
  TNamed(),
  fClassMask(0),
  fIndex(0),
  fDescriptor(NULL),
  fCluster(NULL),
  fPFProtection(NULL),
  fPrescaler(0),
  fAllRare(kFALSE),
  fStatus(kFALSE),
  fTimeGroup(0),
  fTimeWindow(0)
{
  // Default constructor
  for(Int_t i = 0; i < kNMaxMasks; i++)fMask[i]=0; 
}

//_____________________________________________________________________________
IlcTriggerClass::IlcTriggerClass( TString & name, UChar_t index,
				  IlcTriggerDescriptor *desc, IlcTriggerCluster *clus,
				  IlcTriggerPFProtection *pfp, IlcTriggerBCMask *mask,
				  UInt_t prescaler, Bool_t allrare) :
  TNamed( name, name ),
  fClassMask( 1ull << ULong64_t(index-1)),
  fIndex(index),
  fDescriptor( desc ),
  fCluster( clus ),
  fPFProtection( pfp ),
  fPrescaler( prescaler ),
  fAllRare( allrare ),
  fStatus(kFALSE),
  fTimeGroup(0),
  fTimeWindow(0)
{
  // Constructor
  // This should be used with old version of config
  for(Int_t i = 0; i < kNMaxMasks; i++)fMask[i]=0; 
  fMask[0]=mask;
}

//_____________________________________________________________________________
IlcTriggerClass::IlcTriggerClass( IlcTriggerConfiguration *config,
				  TString & name, UChar_t index,
				  TString &desc, TString &clus,
				  TString &pfp, TString &mask,
				  UInt_t prescaler, Bool_t allrare) :
  TNamed( name, name ),
  fClassMask( 1ull << ULong64_t(index-1)),
  fIndex(index),
  fDescriptor( NULL ),
  fCluster( NULL ),
  fPFProtection( NULL ),
  fPrescaler( prescaler ),
  fAllRare( allrare ),
  fStatus(kFALSE),
  fTimeGroup(0),
  fTimeWindow(0)
{
  // This should be used with old version of config
  fDescriptor = (IlcTriggerDescriptor*)config->GetDescriptors().FindObject(desc);
  fCluster = (IlcTriggerCluster*)config->GetClusters().FindObject(clus);
  pfp.ReplaceAll("{","");
  pfp.ReplaceAll("}","");
  fPFProtection = (IlcTriggerPFProtection*)config->GetPFProtections().FindObject(pfp);
  // BC masks
  for(Int_t i = 0; i < kNMaxMasks; i++)fMask[i]=0;
  mask.ReplaceAll("{","");
  mask.ReplaceAll("}","");
  fMask[0] = (IlcTriggerBCMask*)config->GetMasks().FindObject(mask);
}
//_____________________________________________________________________________
IlcTriggerClass::IlcTriggerClass( IlcTriggerConfiguration *config,
				  TString & name, UChar_t index,
				  TString &desc, TString &clus,
				  TString &pfp,
				  UInt_t prescaler, Bool_t allrare,
				  UInt_t timegroup,UInt_t timewindow) :
  TNamed( name, name ),
  fClassMask( 1ull << ULong64_t(index-1)),
  fIndex(index),
  fDescriptor( NULL ),
  fCluster( NULL ),
  fPFProtection( NULL ),
  fPrescaler( prescaler ),
  fAllRare( allrare ),
  fStatus(kFALSE),
  fTimeGroup(timegroup),
  fTimeWindow(timewindow)
{
  fDescriptor = (IlcTriggerDescriptor*)config->GetDescriptors().FindObject(desc);
  fCluster = (IlcTriggerCluster*)config->GetClusters().FindObject(clus);
  pfp.ReplaceAll("{","");
  pfp.ReplaceAll("}","");
  fPFProtection = (IlcTriggerPFProtection*)config->GetPFProtections().FindObject(pfp);
  // masks are added by seter
  for(Int_t i = 0; i < kNMaxMasks; i++)fMask[i]=0;
}
//_____________________________________________________________________________
IlcTriggerClass::~IlcTriggerClass() 
{ 
  // Destructor
}
//_____________________________________________________________________________
IlcTriggerClass::IlcTriggerClass( const IlcTriggerClass& trclass ):
  TNamed( trclass ),
  fClassMask(trclass.fClassMask),
  fIndex(trclass.fIndex),
  fDescriptor(trclass.fDescriptor),
  fCluster(trclass.fCluster),
  fPFProtection(trclass.fPFProtection),
  fPrescaler(trclass.fPrescaler),
  fAllRare(trclass.fAllRare),
  fStatus(trclass.fStatus),
  fTimeGroup(trclass.fTimeGroup),
  fTimeWindow(trclass.fTimeWindow)
{
   // Copy constructor
   for(Int_t i = 0; i < kNMaxMasks; i++)fMask[i]=trclass.fMask[i];
}
//______________________________________________________________________________
IlcTriggerClass& IlcTriggerClass::operator=(const IlcTriggerClass& trclass)
{
   // IlcTriggerClass assignment operator.

   if (this != &trclass) {
      TNamed::operator=(trclass);
      fClassMask = trclass.fClassMask;
      fIndex=trclass.fIndex;
      fDescriptor = trclass.fDescriptor;
      fCluster = trclass.fCluster;
      fPFProtection = trclass.fPFProtection;
      for(Int_t i=0; i< kNMaxMasks; i++)fMask[i]=trclass.fMask[i];
      fPrescaler = trclass.fPrescaler;
      fAllRare = trclass.fAllRare;
      fStatus = trclass.fStatus;
      fTimeGroup = trclass.fTimeGroup;
      fTimeWindow = trclass.fTimeWindow;
   }
   return *this;
}
//_____________________________________________________________________________
Bool_t IlcTriggerClass::SetMasks(IlcTriggerConfiguration* config,TString& masks)
{
 masks.ReplaceAll("{","");
 masks.ReplaceAll("}","");
 masks.ReplaceAll(" ","");
 masks.ReplaceAll("\t","");
 TObjArray *tokens = masks.Tokenize(",");
 Int_t ntokens = tokens->GetEntriesFast();
 if(ntokens==0){
   delete tokens;
   IlcError(Form("The class (%s) has invalid mask pattern: (%s)",GetName(),masks.Data()));
   return kFALSE;
 }
 Int_t nmask=0;
 while(fMask[nmask])nmask++;
 if(nmask+ntokens>=kNMaxMasks){
   delete tokens;
   IlcError(Form("The class (%s) exceeds %i masks",GetName(),kNMaxMasks));
   return kFALSE;
 }
 for(Int_t i=nmask; i<nmask+ntokens; i++){
    fMask[i] = (IlcTriggerBCMask*)config->GetMasks().FindObject(((TObjString*)tokens->At(i-nmask))->String());
    if(!fMask[i]){
      IlcError(Form("The class (%s) unknown mask %s",GetName(),(((TObjString*)tokens->At(i-nmask))->String().Data())));
      return kFALSE;
    }
 }
 delete tokens;
 return kTRUE;
}
//_____________________________________________________________________________
Bool_t IlcTriggerClass::CheckClass(IlcTriggerConfiguration* config) const
{
  // Check the existance of trigger inputs and functions
  // and the logic used.
  // Return false in case of wrong class
  // definition.

  if (!fClassMask) {
    IlcError(Form("The class (%s) has invalid mask pattern !",GetName()));
    return kFALSE;
  }

  // check comsistency of index and mask

  if (!config->GetDescriptors().FindObject(fDescriptor)) {
    IlcError(Form("The class (%s) contains invalid descriptor !",GetName()));
    return kFALSE;
  }
  else {
    if (!(fDescriptor->CheckInputsAndFunctions(config->GetInputs(),config->GetFunctions()))) {
      IlcError(Form("The class (%s) contains bad descriptor !",GetName()));
      return kFALSE;
    }
  }

  if (!config->GetClusters().FindObject(fCluster)) {
    IlcError(Form("The class (%s) contains invalid cluster !",GetName()));
    return kFALSE;
  }

  if (!config->GetPFProtections().FindObject(fPFProtection)) {
    IlcError(Form("The class (%s) contains invalid past-future protection !",GetName()));
    return kFALSE;
  }
  
  for(Int_t i=0; i< kNMaxMasks; i++){
     if(fMask[i]){
        if (!config->GetMasks().FindObject(fMask[i])) {
           IlcError(Form("The class (%s) contains invalid BC mask !",GetName()));
           return kFALSE;
       }
     }
  }
  return kTRUE;
}

//_____________________________________________________________________________
void IlcTriggerClass::Trigger( const TObjArray& inputs , const TObjArray& functions)
{
   // Check if the inputs satify the trigger class conditions
  fStatus = fDescriptor->Trigger(inputs,functions);
}

//_____________________________________________________________________________
Bool_t IlcTriggerClass::IsActive( const TObjArray& inputs, const TObjArray& functions) const
{
   // Check if the inputs satify the trigger class conditions
  if (fDescriptor)
    return fDescriptor->IsActive(inputs,functions);

  return kFALSE;
}

//_____________________________________________________________________________
void IlcTriggerClass::Print( const Option_t* ) const
{
   // Print
  cout << "Trigger Class:" << endl;
  cout << "  Name:         " << GetName() << endl;
  cout << "  ClassBit:     0x" << hex << fClassMask << dec << endl;
  cout << "  Index:        " <<  (UInt_t)fIndex <<  endl;
  cout << "  Descriptor:   " << fDescriptor->GetName() << endl;
  cout << "  Cluster:      " << fCluster->GetName() << endl;
  cout << "  PF Protection:" << fPFProtection->GetName() << endl;
  cout << "  BC Mask:      " ;
  for(Int_t i=0; i< kNMaxMasks; i++)if(fMask[i])cout << fMask[i]->GetName() << " ";
  cout << endl;
  cout << "  Prescaler:    " << fPrescaler << endl;
  cout << "  AllRare:      " << fAllRare << endl;
  cout << "  Time Group:      " << fTimeGroup << endl;
  cout << "  Time Window:      " << fTimeWindow << endl;
  if (fStatus)
     cout << "   Class is fired      " << endl;
   else
     cout << "   Class is not fired  " << endl;
}
