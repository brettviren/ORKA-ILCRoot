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
// This class which defines defines the Trigger Configuration 
//
// Trigger Configuration defines the trigger setup for a particular run
// We have default configurations for each running mode (Pb-Pb, p-p, p-A, Calibration, etc).
// It keep:
//   All the information conained in the CTP configuration file used
//   online during the data taking
//
// Configurations could be created and stored in local file.
// By default the configuration is loaded from the corresponding GRP entry
// inside the OCDB. There one can have one and only one configuration per run.
//
//   Example how to create a Trigger Configuration:
//
//   IlcTriggerConfiguration config( "TEST", "Test Configuration" );
//
//   // Define a Cluster Detector
//   config.AddDetectorCluster( "VZERO ZDC MUON" );
//
//   // Define the trigger conditions (see IlcTriggerCondition.cxx)
//   config.AddCondition( "VZERO_TEST1_L0 & MUON_SPlus_LPt_L0 & ZDC_TEST2_L0", // condition
//                         "VO1_M1_ZDC2",      // short name
//                         "Dummy",            // short description
//                          0x0100 );          // class mask (set one bit)
//
//   config.AddCondition( "VZERO_TEST2_L0 & MUON_SMinus_HPt_L0 & ZDC_TEST1_L0",
//                         "VO2_M3_ZDC1",
//                         "Dummy",
//                          0x0200 );
//
//   config.AddCondition( "VZERO_TEST3_L0 | MUON_Unlike_LPt_L0 | ZDC_TEST3_L0",
//                         "VO3_M1_ZDC3",
//                         "Dummy",
//                          0x0400 );
//   config.CheckInputsConditions("Config.C");
//   config.Print();
//
//   // save the configuration to file 
//   // (default file name $ILC_ROOT/data/triggerConfigurations.root)
//   config.WriteConfiguration(); or config.WriteConfiguration( filename );
//
///////////////////////////////////////////////////////////////////////////////
#include <Riostream.h>

#include <TCint.h>
#include <TFile.h>
#include <TKey.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TObject.h>
#include <TROOT.h>
#include <TString.h>
#include <TSystem.h>
#include <TMath.h>

#include "IlcLog.h"
#include "IlcTriggerBCMask.h"
#include "IlcTriggerClass.h"
#include "IlcTriggerCluster.h"
#include "IlcTriggerConfiguration.h"
#include "IlcTriggerDescriptor.h"
#include "IlcTriggerInput.h"
#include "IlcTriggerInteraction.h"
#include "IlcTriggerPFProtection.h"

ClassImp(IlcTriggerConfiguration)

const TString IlcTriggerConfiguration::fgkConfigurationFileName("/data/triggerConfigurations.root");

//_____________________________________________________________________________
IlcTriggerConfiguration::IlcTriggerConfiguration():
  TNamed(),
  fInputs(),
  fInteractions(),
  fFunctions(),
  fPFProtections(),
  fMasks(),
  fDescriptors(),
  fClusters(),
  fClasses(),
  fVersion(0)
{
  // Default constructor
}

//_____________________________________________________________________________
IlcTriggerConfiguration::IlcTriggerConfiguration( TString & name, TString & description ):
  TNamed( name, description ),
  fInputs(),
  fInteractions(),
  fFunctions(),
  fPFProtections(),
  fMasks(),
  fDescriptors(),
  fClusters(),
  fClasses(),
  fVersion(0)
{
  // Constructor
}

//_____________________________________________________________________________
IlcTriggerConfiguration::~IlcTriggerConfiguration() 
{ 
  // Destructor
  fInputs.SetOwner();
  fInputs.Delete();
  fInteractions.SetOwner();
  fInteractions.Delete();
  fFunctions.SetOwner();
  fFunctions.Delete();
  fPFProtections.SetOwner();
  fPFProtections.Delete();
  fMasks.SetOwner();
  fMasks.Delete();
  fDescriptors.SetOwner();
  fDescriptors.Delete();
  fClusters.SetOwner(); 
  fClusters.Delete(); 
  fClasses.SetOwner(); 
  fClasses.Delete(); 
}

//_____________________________________________________________________________
Bool_t IlcTriggerConfiguration::AddInput( IlcTriggerInput* input )
{
  // Add a trigger input to
  // the list of the trigger inputs
  if (fInputs.GetEntries() < kNMaxInputs) {
    fInputs.AddLast( input );
    return kTRUE;
  }
  else {
    IlcError("CTP can handle up to 60 inputs ! Impossible to add the required input !");
    return kFALSE;
  }
}

//_____________________________________________________________________________
IlcTriggerInput* IlcTriggerConfiguration::AddInput( TString &name, TString &det,
						    UChar_t level, UInt_t signature,
						    UChar_t number )
{
  // Add a trigger input to
  // the list of the trigger inputs
  IlcTriggerInput *input = new IlcTriggerInput(name,det,level,signature,number);
  if (!AddInput(input)) {
    delete input;
    return NULL;
  }
  else
    return input;
}

//_____________________________________________________________________________
IlcTriggerInteraction* IlcTriggerConfiguration::AddInteraction(TString &name, TString &logic)
{
  // Add a trigger interaction object to
  // the list of the trigger interactions
  IlcTriggerInteraction *interact = new IlcTriggerInteraction(name,logic);
  if (!AddInteraction(interact)) {
    delete interact;
    return NULL;
  }
  else
    return interact;
}

//_____________________________________________________________________________
Bool_t  IlcTriggerConfiguration::AddInteraction(IlcTriggerInteraction *interact)
{
  // Add a trigger interaction object to
  // the list of the trigger interactions
  if (fInteractions.GetEntries() < kNMaxInteractions) {
    if (interact->CheckInputs(fInputs)) {
      fInteractions.AddLast( interact );
      return kTRUE;
    }
    else
      IlcError("Invalid interaction ! Impossible to add it !");
  }
  else
    IlcError("CTP can handle up to 2 interactions ! Impossible to add the required interaction !");

  return kFALSE;
}

//_____________________________________________________________________________
IlcTriggerInteraction* IlcTriggerConfiguration::AddFunction(TString &name, TString &logic)
{
  // Add a trigger function object to
  // the list of the trigger functions
  IlcTriggerInteraction *func = new IlcTriggerInteraction(name,logic);
  if (!AddFunction(func)) {
    delete func;
    return NULL;
  }
  else
    return func;
}

//_____________________________________________________________________________
Bool_t  IlcTriggerConfiguration::AddFunction(IlcTriggerInteraction *func)
{
  // Add a trigger function object to
  // the list of the trigger functions
  if (fFunctions.GetEntries() < kNMaxFunctions) {
    if (func->CheckInputs(fInputs)) {
      fFunctions.AddLast( func );
      return kTRUE;
    }
    else
      IlcError("Invalid logical function ! Impossible to add it !");
  }
  else
    IlcError("CTP can handle up to 4 logical functions ! Impossible to add the required interaction !");

  return kFALSE;
}

//_____________________________________________________________________________
Bool_t IlcTriggerConfiguration::AddPFProtection( IlcTriggerPFProtection* pfp )
{
  // Add a trigger past-future protection object to
  // the list of the trigger past-future protections
  if (fPFProtections.GetEntries() < kNMaxPFProtections) {
    //if (pfp->CheckInteractions(fInteractions)) {
    if (1) {
      fPFProtections.AddLast( pfp );
      return kTRUE;
    }
    else
      IlcError("Invalid past-future protection ! Impossible to add it !");
  }
  else
    IlcError("CTP can handle up to 4 past-future protections ! Impossible to add the required protection !");

  return kFALSE;
}

//_____________________________________________________________________________
IlcTriggerBCMask* IlcTriggerConfiguration::AddMask( TString &name, TString &mask )
{
  // Add a trigger bunch-crossing mask object to
  // the list of the trigger bunch-crossing masks
  IlcTriggerBCMask *bcmask = new IlcTriggerBCMask(name,mask);
  if (!AddMask(bcmask)) {
    delete bcmask;
    return NULL;
  }
  else
    return bcmask;
}

//_____________________________________________________________________________
Bool_t IlcTriggerConfiguration::AddMask( IlcTriggerBCMask* mask )
{
  // Add a trigger bunch-crossing mask object to
  // the list of the trigger bunch-crossing masks
  if (fMasks.GetEntries() < (kNMaxMasks)) {  
      fMasks.AddLast( mask );
      return kTRUE;
  }
  else
    IlcError("CTP can handle up to 12 bunch-crossing masks ! Impossible to add the required mask !");

  return kFALSE;
}

//_____________________________________________________________________________
IlcTriggerCluster* IlcTriggerConfiguration::AddCluster( TString &name, UChar_t index, TString &detectors)
{
  // Add a trigger detector readout cluster to
  // the list of the trigger clusters
  IlcTriggerCluster *clust = new IlcTriggerCluster(name,index,detectors);
  if (!AddCluster(clust)) {
    delete clust;
    return NULL;
  }
  else
    return clust;

}

//_____________________________________________________________________________
Bool_t IlcTriggerConfiguration::AddCluster( IlcTriggerCluster* cluster )
{
  // Add a trigger detector readout cluster to
  // the list of the trigger clusters
  if (fClusters.GetEntries() < kNMaxClusters) {
    TString dets(cluster->GetDetectorsInCluster());
    if (!(dets.IsNull())) {
      fClusters.AddLast( cluster );
      return kTRUE;
    }
    else
      IlcError("Empty trigger cluster ! Impossible to add it !");
  }
  else
    IlcError("CTP can handle up to 6 different detector clusters ! Impossible to add the required cluster !");

  return kFALSE;
}

//_____________________________________________________________________________
TString IlcTriggerConfiguration::GetActiveDetectors() const
{
  // Return an string with all active detector
  // from each cluster

   TString activeDet = "";

   Int_t nclus = fClusters.GetEntriesFast();
   if( !nclus ) return activeDet;
   
   for( Int_t j=0; j<nclus; ++j ) {
      TString detStr = ((IlcTriggerCluster*)fClusters.At(j))->GetDetectorsInCluster();
      TObjArray* det = detStr.Tokenize(" ");
      Int_t ndet = det->GetEntriesFast();
      for( Int_t k=0; k<ndet; ++k ) {
         if( activeDet.Contains( ((TObjString*)det->At(k))->String() ) )continue;
         activeDet.Append( " " );
         activeDet.Append( ((TObjString*)det->At(k))->String() );
      }
   }
   return activeDet;
}

//_____________________________________________________________________________
TString IlcTriggerConfiguration::GetTriggeringDetectors() const
{
  // Return an string with all detectors
  // used for triggering

   TString trDet = "";

   Int_t ninputs = fInputs.GetEntriesFast();
   if( !ninputs ) return trDet;
   
   for( Int_t j=0; j<ninputs; j++ ) {
      TString detStr = ((IlcTriggerInput*)fInputs.At(j))->GetDetector();
      if( trDet.Contains( detStr ) ) continue;
      trDet.Append( " " );
      trDet.Append( detStr );
   }
   return trDet;
}

//_____________________________________________________________________________
TString IlcTriggerConfiguration::GetTriggeringModules() const
{
   // Return an string with all detectors (modules in the IlcRoot
  // simulation sense) used for triggering

   TString trDet = "";

   Int_t ninputs = fInputs.GetEntriesFast();
   if( !ninputs ) return trDet;
   
   for( Int_t j=0; j<ninputs; j++ ) {
      TString detStr = ((IlcTriggerInput*)fInputs.At(j))->GetModule();
      if( trDet.Contains( detStr ) ) continue;
      trDet.Append( " " );
      trDet.Append( detStr );
   }
   return trDet;
}

//_____________________________________________________________________________
IlcTriggerDescriptor* IlcTriggerConfiguration::AddDescriptor( TString &name, TString &cond )
{
  // Add a trigger descriptor to
  // the list of the trigger descriptors
  IlcTriggerDescriptor *desc = new IlcTriggerDescriptor(name,cond);
  if (!AddDescriptor(desc)) {
    delete desc;
    return NULL;
  }
  else
    return desc;
}

//_____________________________________________________________________________
Bool_t IlcTriggerConfiguration::AddDescriptor( IlcTriggerDescriptor *desc )
{
  // Add a trigger descriptor to
  // the list of the trigger descriptors
  if (fDescriptors.GetEntries() < kNMaxClasses) {
    if (desc->CheckInputsAndFunctions(fInputs,fFunctions)) {
      fDescriptors.AddLast( desc );
      return kTRUE;
    }
    else
      IlcError("Invalid trigger desciptor ! Impossible to add it !");
  }
  else
    IlcError("CTP can handle up to 50 different descriptors ! Impossible to add the required descriptor !");

  return kFALSE;
}

//_____________________________________________________________________________
Bool_t IlcTriggerConfiguration::AddClass( IlcTriggerClass *trclass )
{
  // Add a trigger class to
  // the list of the trigger classes
  if (fClasses.GetEntries() < kNMaxClasses) {
    if (trclass->CheckClass(this)) {
      fClasses.AddLast( trclass );
      return kTRUE;
    }
    else
      IlcError("Invalid trigger class ! Impossible to add it !");
  }
  else
    IlcError("CTP can handle up to 50 different classes ! Impossible to add the required class !");

  return kFALSE;
}

//_____________________________________________________________________________
IlcTriggerClass *IlcTriggerConfiguration::AddClass( TString &name, UChar_t index,
						    IlcTriggerDescriptor *desc, IlcTriggerCluster *clus,
						    IlcTriggerPFProtection *pfp, IlcTriggerBCMask *mask,
						    UInt_t prescaler, Bool_t allrare)
{
  // Add a trigger class to
  // the list of the trigger classes
  if (!fDescriptors.FindObject(desc)) {
    IlcError("Invalid descriptor ! Impossible to add the class !");
    return NULL;
  }
  if (!fClusters.FindObject(clus)) {
    IlcError("Invalid cluster ! Impossible to add the class !");
    return NULL;
  }
  if (!fPFProtections.FindObject(pfp)) {
    IlcError("Invalid past-future protection ! Impossible to add the class !");
    return NULL;
  }
  if (!fMasks.FindObject(mask)) {
    IlcError("Invalid bunch-crossing mask ! Impossible to add the class !");
    return NULL;
  }
  IlcTriggerClass* trclass = new IlcTriggerClass( name,index,desc,clus,pfp,mask,prescaler,allrare );
  if (!AddClass(trclass)) {
    delete trclass;
    return NULL;
  }
  else
    return trclass;
}

//_____________________________________________________________________________
IlcTriggerClass *IlcTriggerConfiguration::AddClass( TString &name, UChar_t index,
						    TString &desc, TString &clus,
						    TString &pfp, TString &mask,
						    UInt_t prescaler, Bool_t allrare)
{
   // Add a new trigger class
  if (!fDescriptors.FindObject(desc)) {
    IlcError("Invalid descriptor ! Impossible to add the class !");
    return NULL;
  }
  if (!fClusters.FindObject(clus)) {
    IlcError("Invalid cluster ! Impossible to add the class !");
    return NULL;
  }
  if (!fPFProtections.FindObject(pfp)) {
    IlcError("Invalid past-future protection ! Impossible to add the class !");
    return NULL;
  }
  if (!fMasks.FindObject(mask)) {
    IlcError("Invalid bunch-crossing mask ! Impossible to add the class !");
    return NULL;
  }
  IlcTriggerClass* trclass = new IlcTriggerClass( this, name,index,desc,clus,pfp,mask,prescaler,allrare );
  if (!AddClass(trclass)) {
    delete trclass;
    return NULL;
  }
  else
    return trclass;
}

//_____________________________________________________________________________
Bool_t IlcTriggerConfiguration::ProcessConfigurationLine(const char* line, Int_t& level)
{
    // processes one line of configuration

     TString strLine(line);

     if (strLine.BeginsWith("#")) return kTRUE;
     if (strLine.BeginsWith("PARTITION:")) {
       strLine.ReplaceAll("PARTITION:","");
       SetName(strLine.Data());
       return kTRUE;
     }
     if (strLine.BeginsWith("VERSION:")) {
       strLine.ReplaceAll("VERSION:","");
       fVersion = strLine.Atoi();
       return kTRUE;
     }
     if (strLine.BeginsWith("INPUTS:")) {
       level = 1;
       return kTRUE;
     }
     if (strLine.BeginsWith("INTERACTIONS:")) {
       level = 2;
       return kTRUE;
     }
     if (strLine.BeginsWith("DESCRIPTORS:")) {
       level = 3;
       return kTRUE;
     }
     if (strLine.BeginsWith("CLUSTERS:")) {
       level = 4;
       return kTRUE;
     }
     if (strLine.BeginsWith("PFS:")) {
       level = 5;
       return kTRUE;
     }
     if (strLine.BeginsWith("BCMASKS:")) {
       level = 6;
       return kTRUE;
     }
     if (strLine.BeginsWith("CLASSES:")) {
       level = 7;
       return kTRUE;
     }

     strLine.ReplaceAll("*",'!');
     strLine.ReplaceAll("~",'!');
     TObjArray *tokens = strLine.Tokenize(" \t");
     Int_t ntokens = tokens->GetEntriesFast();
     if (ntokens == 0)
     {
       delete tokens;
       return kTRUE;
     }
     switch (level) {
     case 1:
       // Read inputs
       if (ntokens != 5) {
	 IlcError(Form("Invalid trigger input syntax (%s)!",strLine.Data()));
	 return kFALSE;
       }
       AddInput(((TObjString*)tokens->At(0))->String(),
		     ((TObjString*)tokens->At(1))->String(),
		     ((TObjString*)tokens->At(2))->String().Atoi(),
		     ((TObjString*)tokens->At(3))->String().Atoi(),
		     ((TObjString*)tokens->At(4))->String().Atoi());
       break;
     case 2:
       // Read interaction
       if (ntokens != 2) {
	 IlcError(Form("Invalid trigger interaction syntax (%s)!",strLine.Data()));
	 return kFALSE;
       }
       AddInteraction(((TObjString*)tokens->At(0))->String(),
			   ((TObjString*)tokens->At(1))->String());
       break;
     case 3:
       // Read logical functions and descriptors
       if (ntokens < 2) {
	 if ((((TObjString*)tokens->At(0))->String().CompareTo("EMPTY") == 0) ||
	     (((TObjString*)tokens->At(0))->String().CompareTo("DTRUE") == 0) ||
	     (((TObjString*)tokens->At(0))->String().CompareTo("DEMPTY") == 0)) {
	   AddDescriptor(((TObjString*)tokens->At(0))->String(),
			 strLine.ReplaceAll(((TObjString*)tokens->At(0))->String(),""));
	   break;
	 }
	 else {
	   IlcError(Form("Invalid trigger descriptor syntax (%s)!",strLine.Data()));
	   return kFALSE;
	 }
       }
       if (((TObjString*)tokens->At(0))->String().BeginsWith("l0f")) {
	 // function
	 if(!AddFunction(((TObjString*)tokens->At(0))->String(),
			  strLine.ReplaceAll(((TObjString*)tokens->At(0))->String(),""))) return kFALSE;
       }
       else {
	 if(!AddDescriptor(((TObjString*)tokens->At(0))->String(),
			    strLine.ReplaceAll(((TObjString*)tokens->At(0))->String(),""))) return kFALSE;
       }
       break;
     case 4:
       {
         if (ntokens < 2) {
           IlcError(Form("Invalid trigger cluster syntax (%s)!",strLine.Data()));
           return kFALSE;
         }
	 if (((TObjString*)tokens->At(1))->String().Atoi() <= 0) {
           IlcError(Form("Invalid trigger cluster syntax (%s)!",strLine.Data()));
           return kFALSE;
         }
	 TString strTemp;
	 for(Int_t i = 2; i < ntokens; i++) {
	   strTemp += ((TObjString*)tokens->At(i))->String();
	   strTemp += " ";
	 }
	 AddCluster(((TObjString*)tokens->At(0))->String(),
			 ((TObjString*)tokens->At(1))->String().Atoi(),
			 strTemp);
       }
       break;
     case 5:
       {
	 IlcTriggerPFProtection *pfp = NULL;
	 if ((((TObjString*)tokens->At(0))->String().CompareTo("NONE") == 0) ||
	     (((TObjString*)tokens->At(0))->String().CompareTo("NOPF") == 0)) {
	   pfp = new IlcTriggerPFProtection(((TObjString*)tokens->At(0))->String());
	 }
	 else {
           if (ntokens == 10){ 
	    pfp = new IlcTriggerPFProtection(((TObjString*)tokens->At(0))->String(),
					    ((TObjString*)tokens->At(1))->String(),
					    ((TObjString*)tokens->At(2))->String(),
					    ((TObjString*)tokens->At(3))->String());
	    pfp->SetNa1(((TObjString*)tokens->At(4))->String().Atoi());
	    pfp->SetNa2(((TObjString*)tokens->At(5))->String().Atoi());
	    pfp->SetNb1(((TObjString*)tokens->At(6))->String().Atoi());
	    pfp->SetNb2(((TObjString*)tokens->At(7))->String().Atoi());
	    pfp->SetTa(((TObjString*)tokens->At(8))->String().Atoi());
	    pfp->SetTb(((TObjString*)tokens->At(9))->String().Atoi());
	  }else if(ntokens == 13){
	    UInt_t pfdef[12];
	    for(Int_t i=0;i<12;i++){
	       TString ss(((TObjString*)tokens->At(i+1))->String());
	       ss.Remove(0,2);
	       UInt_t num=0;
	       for(Int_t j=ss.Length()-1;j>=0;j--){
	        UInt_t nn=ss[j];
		if(nn >= (UInt_t)'0' && nn <= (UInt_t)'9')nn=nn-(UInt_t)'0'; else 
		if(nn >= (UInt_t)'A' && nn <= (UInt_t)'F')nn=10+nn-(UInt_t)'A'; else
		if(nn >= (UInt_t)'a' && nn <= (UInt_t)'f')nn=10+nn-(UInt_t)'a'; else{
  	     		IlcError(Form("Invalid trigger pfs syntax (%s)!",strLine.Data()));
	     		//return kFALSE;
		}
		num=num+(1<<(ss.Length()-1-j)*4)*nn;
	        //cout << ss[j] << " 2 " << nn << " "  << num << endl;
	       }
	       pfdef[i]=num;
	    }   
	    pfp = new IlcTriggerPFProtection(((TObjString*)tokens->At(0))->String(),pfdef);
	  }else{
  	     IlcError(Form("Invalid trigger pfs syntax (%s)!",strLine.Data()));
	     //return kFALSE;
          }
	 }
	 AddPFProtection(pfp);
       }
       break;
     case 6:
         if (ntokens > 2) {
  	   IlcError(Form("Invalid trigger bcmasks syntax (%s)!",strLine.Data()));
	   return kFALSE;
         }
       if (((TObjString*)tokens->At(0))->String().CompareTo("NONE") == 0)
       {	 
         if(!AddMask(new IlcTriggerBCMask(((TObjString*)tokens->At(0))->String()))) return kFALSE;
       }
       else {
	 if(!AddMask(((TObjString*)tokens->At(0))->String(),((TObjString*)tokens->At(1))->String())) return kFALSE;
       }
       break;
     case 7:
       {
         if ((ntokens !=8) && (ntokens != 10)) {
  	   IlcError(Form("Invalid trigger class syntax (%s)!",strLine.Data()));
	   return kFALSE;
         }
	 IlcTriggerClass *trclass=0;
	 if(ntokens == 8)trclass = new IlcTriggerClass(this,
			((TObjString*)tokens->At(0))->String(),((TObjString*)tokens->At(1))->String().Atoi(),
			((TObjString*)tokens->At(2))->String(),((TObjString*)tokens->At(3))->String(),
			((TObjString*)tokens->At(4))->String(),((TObjString*)tokens->At(5))->String(),
			((TObjString*)tokens->At(6))->String().Atoi(),(Bool_t)(((TObjString*)tokens->At(7))->String().Atoi()));
	 else{ trclass = new IlcTriggerClass(this,
			((TObjString*)tokens->At(0))->String(),((TObjString*)tokens->At(1))->String().Atoi(),
			((TObjString*)tokens->At(2))->String(),((TObjString*)tokens->At(3))->String(),
			((TObjString*)tokens->At(4))->String(),
			((TObjString*)tokens->At(6))->String().Atoi(),(Bool_t)(((TObjString*)tokens->At(7))->String().Atoi()),
			(((TObjString*)tokens->At(8))->String().Atoi()),(((TObjString*)tokens->At(9))->String().Atoi()));
		if(!trclass->SetMasks(this,((TObjString*)tokens->At(5))->String())) return kFALSE;
         }
         AddClass(trclass);
       }
     default:
       break;
     }
     delete tokens;

     return kTRUE;
}

//_____________________________________________________________________________
IlcTriggerConfiguration* IlcTriggerConfiguration::LoadConfiguration(TString & configuration)
{
   // Load one pre-created Configurations from database/file that match
   // with the input string 'configuration'
   // Ej: "Pb-Pb" or "p-p-DIMUON CALIBRATION-CENTRAL-BARREL"
  // By default the files are stored in GRP/CTP folder.
  // The filename is constructed as: GRP/CTP/<configuration>.cfg

   // Load the selected configuration
  TString filename;
  if (configuration.EndsWith(".cfg") ||
      configuration.EndsWith(".shuttle")) {
    filename = configuration;
  }
  else {
    filename = gSystem->Getenv("ILC_ROOT");
    filename += "/GRP/CTP/";
    filename += configuration;
    filename += ".cfg";
  }

   if( gSystem->AccessPathName( filename.Data() ) ) {
      IlcErrorClass( Form( "file (%s) not found", filename.Data() ) );
      return NULL;
   }


   ifstream *file = new ifstream ( filename.Data() );
   if (!*file) {
     IlcErrorClass(Form("Error opening file (%s) !",filename.Data()));
     file->close();
     delete file;
     return NULL;
   }

   IlcTriggerConfiguration *cfg = new IlcTriggerConfiguration();

   Int_t level = 0;

   TString strLine;
   while (strLine.ReadLine(*file)) {
     if (cfg->ProcessConfigurationLine(strLine, level) == kFALSE)
     {
        delete cfg;
        cfg = 0;
        break;
     }
   }

   file->close();
   delete file;

   return cfg;
}

//_____________________________________________________________________________
IlcTriggerConfiguration* IlcTriggerConfiguration::LoadConfigurationFromString(const char* configuration)
{
   // Loads configuration given as parameter <configuration>

   if (!configuration)
     return 0;

   IlcTriggerConfiguration *cfg = new IlcTriggerConfiguration();

   Int_t level = 0;

   TObjArray* tokens = TString(configuration).Tokenize("\n");
   for (Int_t i=0; i<tokens->GetEntries(); i++)
   {
     TObjString* str = dynamic_cast<TObjString*>(tokens->At(i));
     if (!str)
       continue;

     if (cfg->ProcessConfigurationLine(str->String(), level) == kFALSE)
     {
        delete cfg;
        cfg = 0;
        break;
     }
   }

   delete tokens;

   return cfg;
}

//_____________________________________________________________________________
TObjArray* IlcTriggerConfiguration::GetAvailableConfigurations( const char* filename )
{
   // Return an array of configuration in the file

   TString path;
   if( !filename[0] ) {
      path += gSystem->Getenv( "ILC_ROOT" );
      path += fgkConfigurationFileName;
   }
   else
      path += filename;

   if( gSystem->AccessPathName( path.Data() ) ) {
      IlcErrorGeneral( "IlcTriggerConfiguration", Form( "file (%s) not found", path.Data() ) );
      return NULL;
   }

   TObjArray* desArray = new TObjArray();

   TFile file( path.Data(), "READ" );
   if( file.IsZombie() ) {
      IlcErrorGeneral( "IlcTriggerConfiguration", Form( "Error opening file (%s)", path.Data() ) );
      return NULL;
   }

   file.ReadAll();

   TKey* key;
   TIter next( file.GetListOfKeys() );
   while( (key = (TKey*)next()) ) {
      TObject* obj = key->ReadObj();
      if( obj->InheritsFrom( "IlcTriggerConfiguration" ) ) {
         desArray->AddLast( obj );
      }
   }
   file.Close();

   return desArray;
}

//_____________________________________________________________________________
void IlcTriggerConfiguration::WriteConfiguration( const char* filename )
{
   // Write the configuration
   TString path;
   if( !filename[0] ) {
      path += gSystem->Getenv("ILC_ROOT");
      path += fgkConfigurationFileName;
   }
   else
      path += filename;

   TFile file( path.Data(), "UPDATE" );
   if( file.IsZombie() ) {
      IlcErrorGeneral( "IlcTriggerConfiguration", 
                        Form( "Can't open file (%s)", path.Data() ) );
      return;
   }

   Bool_t result = (Write( GetName(), TObject::kOverwrite ) != 0);
   if( !result )
      IlcErrorGeneral( "IlcTriggerConfiguration",
                        Form( "Can't write entry to file <%s>!", path.Data() ) );
   file.Close();
}

//_____________________________________________________________________________
Int_t IlcTriggerConfiguration::GetClassIndexFromName(const char* className) const
{
   //const TObjArray& classes = cfg->GetClasses();
   Int_t nclasses = (Int_t)fClasses.GetEntriesFast();
   for (Int_t i=0;i<nclasses;i++) {
       IlcTriggerClass* trgclass = (IlcTriggerClass*)fClasses.At(i);
       if (TString(trgclass->GetName()).CompareTo(className) == 0) { 
          ULong64_t classmask = (ULong64_t)trgclass->GetMask();
          return TMath::Nint(TMath::Log2(classmask))+1;
       }
   }
   return -1;
}
//_____________________________________________________________________________
const char* IlcTriggerConfiguration::GetClassNameFromIndex(Int_t classIndex) const
{
   Int_t nclasses = (Int_t)fClasses.GetEntriesFast();
   for (Int_t i=0;i<nclasses;i++) {
       IlcTriggerClass* trgclass = (IlcTriggerClass*)fClasses.At(i);
       ULong64_t classmask = (ULong64_t)trgclass->GetMask();
       if (TMath::Nint(TMath::Log2(classmask))+1 == classIndex) return trgclass->GetName();
   }
   return 0;
}
//_____________________________________________________________________________
IlcTriggerClass* IlcTriggerConfiguration::GetTriggerClass(Int_t classIndex) const
{
   Int_t nclasses = (Int_t)fClasses.GetEntriesFast();
   for (Int_t i=0;i<nclasses;i++) {
       IlcTriggerClass* trgclass = (IlcTriggerClass*)fClasses.At(i);
       ULong64_t classmask = (ULong64_t)trgclass->GetMask();
       if (TMath::Nint(TMath::Log2(classmask))+1 == classIndex) return trgclass;
   }
   return 0;
}
//_____________________________________________________________________________
void IlcTriggerConfiguration::Reset()
{
   for( Int_t j=0; j<fInputs.GetEntriesFast(); j++ )
     ((IlcTriggerInput*)fInputs.At(j))->Reset();

   for( Int_t j=0; j<fClasses.GetEntriesFast(); j++ )
     ((IlcTriggerClass*)fClasses.At(j))->Reset();
}

//_____________________________________________________________________________
void IlcTriggerConfiguration::Print( const Option_t*  ) const
{
   // Print
  cout << "#################################################" << endl;
   cout << "Trigger Configuration:"  << endl;
   cout << "  Name:              " << GetName() << endl; 
   cout << "  Description:       " << GetTitle() << endl;
   cout << "  Version:           " << GetVersion() << endl;
   cout << "  Active Detectors:  " << GetActiveDetectors() << endl;
   cout << "  Trigger Detectors: " << GetTriggeringDetectors() << endl;

   cout << "#################################################" << endl;
   fInputs.Print();
   cout << "#################################################" << endl;
   fInteractions.Print();
   cout << "#################################################" << endl;
   fFunctions.Print();
   cout << "#################################################" << endl;
   fDescriptors.Print();
   cout << "#################################################" << endl;
   fClusters.Print();
   cout << "#################################################" << endl;
   fPFProtections.Print();
   cout << "#################################################" << endl;
   fMasks.Print();
   cout << "#################################################" << endl;
   fClasses.Print();
   cout << "#################################################" << endl;

   cout << endl;
}


//////////////////////////////////////////////////////////////////////////////
// Helper method

//_____________________________________________________________________________
Bool_t IlcTriggerConfiguration::IsSelected( TString detName, TString& detectors ) const
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
