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

/* $Id: IlcDigitizationInput.cxx 52261 2011-10-23 15:46:57Z hristov $ */

//_______________________________________________________________________
//
// IlcDigitizationInput.cxx
// RS: Created from former IlcDigitizationInput. It does not anymore manage digitization:
// (now it is steared by the IlcSimulation) but just provides input info for 
// IlcDetector::SDigits2Ditits.
//
// Manager object for merging/digitization
//
// Instance of this class manages the digitization and/or merging of
// Sdigits into Digits. 
//
// Only one instance of this class is created in the macro:
//   IlcDigitizationInput * manager = 
//      new IlcDigitizationInput(nInputStreams,SPERB);
// where nInputStreams is number of input streams and SPERB is
// signals per background variable, which determines how combinations
// of signal and background events are generated.
// Then instances of specific detector digitizers are created:
//   IlcMUONDigitizer *dMUON  = new IlcMUONDigitizer(manager)
// and the I/O configured (you have to specify input files 
// and an output file). The manager connects appropriate trees from 
// the input files according a combination returned by IlcMergeCombi 
// class. It creates TreeD in the output and runs once per 
// event Digitize method of all existing IlcDetDigitizers 
// (without any option). IlcDetDigitizers ask manager
// for a TTree with input (manager->GetInputTreeS(Int_t i),
// merge all inputs, digitize it, and save it in the TreeD 
// obtained by manager->GetTreeD(). Output events are stored with 
// numbers from 0, this default can be changed by 
// manager->SetFirstOutputEventNr(Int_t) method. The particle numbers
// in the output are shifted by MASK, which is taken from manager.
//
// The default output is to the signal file (stream 0). This can be 
// changed with the SetOutputFile(TString fn)  method.
//
// Single input file is permitted. Maximum MAXSTREAMSTOMERGE can be merged.
// Input from the memory (on-the-fly merging) is not yet 
// supported, as well as access to the input data by invoking methods
// on the output data.
//
// Access to the some data is via gIlc for now (supposing the 
// same geometry in all input files), gIlc is taken from the first 
// input file on the first stream.
//
// Example with MUON digitizer, no merging, just digitization
//
//  IlcDigitizationInput * manager = new IlcDigitizationInput(1,1);
//  manager->SetInputStream(0,"gilc.root");
//  IlcMUONDigitizer *dMUON  = new IlcMUONDigitizer(manager);
//  manager->Exec("");
//
// Example with MUON digitizer, merge all events from 
//   gilc.root (signal) file with events from bgr.root 
//   (background) file. Number of merged events is
//   min(number of events in gilc.root, number of events in bgr.root)
//
//  IlcDigitizationInput * manager = new IlcDigitizationInput(2,1);
//  manager->SetInputStream(0,"gilc.root");
//  manager->SetInputStream(1,"bgr.root");
//  IlcMUONDigitizer *dMUON  = new IlcMUONDigitizer(manager);
//  manager->Exec("");
//
// Example with MUON digitizer, save digits in a new file digits.root,
//   process only 1 event
//
//  IlcDigitizationInput * manager = new IlcDigitizationInput(2,1);
//  manager->SetInputStream(0,"gilc.root");
//  manager->SetInputStream(1,"bgr.root");
//  manager->SetOutputFile("digits.root");
//  IlcMUONDigitizer *dMUON  = new IlcMUONDigitizer(manager);
//  manager->SetNrOfEventsToWrite(1);
//  manager->Exec("");
//
//_______________________________________________________________________ 

// system includes

// ROOT includes

class TList;
#include <Riostream.h>
#include <TTree.h>

// IlcROOT includes

#include "IlcLog.h"
#include "IlcDigitizer.h"
#include "IlcHeader.h"
#include "IlcLoader.h"
#include "IlcMergeCombi.h"
#include "IlcRun.h"
#include "IlcDigitizationInput.h"
#include "IlcRunLoader.h"

ClassImp(IlcDigitizationInput)

const TString IlcDigitizationInput::fgkDefOutFolderName("Output");
const TString IlcDigitizationInput::fgkBaseInFolderName("Input");


//_______________________________________________________________________
IlcDigitizationInput::IlcDigitizationInput(): TNamed("IlcDigitizationInput","The manager for Merging"),
 fkMASKSTEP(0),
 fOutputFileName(0),
 fOutputDirName(0),
 fEvent(0),
 fNrOfEventsToWrite(0),
 fNrOfEventsWritten(0),
 fCopyTreesFromInput(0),
 fNinputs(0),
 fNinputsGiven(0),
 fRegionOfInterest(kFALSE),
 fInputStreams(0x0),
 fOutRunLoader(0x0),
 fOutputInitialized(kFALSE),
 fCombi(0),
 fCombination(0),
 fCombinationFileName(0)
{
  //
  // root requires default ctor, where no new objects can be created
  // do not use this ctor, it is supplied only for root needs
  for (Int_t i=0; i<MAXSTREAMSTOMERGE; ++i) fkMASK[i]=0;
}

//_______________________________________________________________________
IlcDigitizationInput::IlcDigitizationInput(Int_t nInputStreams, Int_t sperb):
 TNamed("IlcDigitizationInput","The manager for Merging"),
 fkMASKSTEP(10000000),
 fOutputFileName(""),
 fOutputDirName("."),
 fEvent(0),
 fNrOfEventsToWrite(-1),
 fNrOfEventsWritten(0),
 fCopyTreesFromInput(-1),
 fNinputs(nInputStreams),
 fNinputsGiven(0),
 fRegionOfInterest(kFALSE),
 fInputStreams(new TClonesArray("IlcStream",nInputStreams)),
 fOutRunLoader(0x0),
 fOutputInitialized(kFALSE),
 fCombi(new IlcMergeCombi(nInputStreams,sperb)),
 fCombination(MAXSTREAMSTOMERGE),
 fCombinationFileName(0)

{
  //
  // ctor which should be used to create 
  // a manager for merging/digitization
  //
  if (nInputStreams == 0) 
   {//kidding
    IlcFatal("Specify nr of input streams");
    return;
   }
  Int_t i;
  
  fkMASK[0] = 0;
  
  for (i=1;i<MAXSTREAMSTOMERGE;i++) 
   {
    fkMASK[i] = fkMASK[i-1] + fkMASKSTEP;
   }
  
  TClonesArray &lInputStreams = *fInputStreams;
  
  new(lInputStreams[0]) IlcStream(fgkBaseInFolderName+(Long_t)0,"UPDATE");
  for (i=1;i<nInputStreams;i++) {
    new(lInputStreams[i]) IlcStream(fgkBaseInFolderName+(Long_t)i,"READ");
  }
}
//_______________________________________________________________________

void IlcDigitizationInput::Copy(TObject&) const
{
  //
  // Non implemented copy function
  //
  IlcFatal("Not installed");
}

//_______________________________________________________________________

IlcDigitizationInput::~IlcDigitizationInput() 
{
  //
  // dtor
  //
  delete fInputStreams;
  delete fCombi;
  delete fOutRunLoader;
}

//_______________________________________________________________________
void IlcDigitizationInput::SetInputStream(Int_t i, const char *inputFile, TString foldername)
{
//
// Sets the name of the input file
//
  if (i > fInputStreams->GetLast()) {
    IlcError("Input stream number too high");
    return;
  }
  IlcStream * stream = static_cast<IlcStream*>(fInputStreams->At(i)) ; 
  if ( !foldername.IsNull() ) {
    if ( i > 0 ) 
      foldername += i ; // foldername should stay unchanged for the default output 
    stream->SetFolderName(foldername) ;
  } 
  stream->AddFile(inputFile);
}

//_______________________________________________________________________
Bool_t IlcDigitizationInput::ConnectInputTrees()
{
  //
  // loads events 
  //
  Int_t eventNr[MAXSTREAMSTOMERGE], delta[MAXSTREAMSTOMERGE];
  fCombi->Combination(eventNr, delta);
  for (Int_t i=0;i<fNinputs;i++) 
   {
    if (delta[i] == 1)
     {
      IlcStream *iStream = static_cast<IlcStream*>(fInputStreams->At(i));//gets the "i" defined  in combination
      if (!iStream->NextEventInStream()) return kFALSE; //sets serial number
     } 
    else if (delta[i] != 0) 
     {
      IlcError("Only delta 0 or 1 is implemented");
      return kFALSE;
     }
   }

  return kTRUE;
}


//_______________________________________________________________________

void IlcDigitizationInput::SetOutputFile(TString fn)
{
  //
  // The output will be to separate file, 
  // not to the signal file here should be protection 
  //to avoid setting the same file as any input 
  //
  IlcInfo(Form("Setting Output File Name %s ",fn.Data()));
  fOutputFileName = fn;
  // InitOutputGlobal();
}

//_______________________________________________________________________
Bool_t IlcDigitizationInput::InitOutputGlobal()
{
  //
  // Creates the output file, called by InitEvent()
  // Needs to be called after all inputs are opened  
  //
  if (fOutputInitialized) return kTRUE;
  
  if ( !fOutputFileName.IsNull())
   {
    fOutRunLoader = IlcRunLoader::Open(fOutputFileName,fgkDefOutFolderName,"recreate");
    
    if (fOutRunLoader == 0x0)
     {
       IlcError("Can not open output");
       return kFALSE;
     }
    Info("InitOutputGlobal", " 1 %s = ", GetInputFolderName(0).Data()) ; 
    IlcRunLoader* inrl = IlcRunLoader::GetRunLoader(GetInputFolderName(0));
    if (inrl == 0x0)
     {
       IlcError("Can not get Run Loader Input 0. Maybe yet not initialized?");
       return kFALSE;
     }
    IlcDebug(2, Form(" 2 %p = ", inrl)) ; 

    //Copy all detector loaders from input 0 to output
    const TObjArray* inloaders = inrl->GetArrayOfLoaders();
    TIter next(inloaders);
    IlcLoader *loader;
    while((loader = (IlcLoader*)next()))
     {
       IlcLoader* cloneloader = (IlcLoader*)loader->Clone();
       cloneloader->Register(fOutRunLoader->GetEventFolder());//creates folders for this loader in Output Folder Structure
       GetOutRunLoader()->AddLoader(cloneloader);//adds a loader for output
     }

    fOutRunLoader->MakeTree("E");
    
    IlcDebug(3,Form("file %s was opened.",fOutputFileName.Data()));
   }
  fOutputInitialized = kTRUE; 
  return kTRUE;
}
//_______________________________________________________________________

void IlcDigitizationInput::InitEvent()
{
  //
  // redirects output properly
  //
  IlcDebug(3,Form("fEvent = %d",fEvent));
  IlcDebug(3,Form("fOutputFileName \"%s\"",fOutputFileName.Data()));
  if (fOutputInitialized == kFALSE) InitOutputGlobal();
  
  // if fOutputFileName was not given, write output to signal directory
}
//_______________________________________________________________________

void IlcDigitizationInput::FinishEvent()
{
  //
  // called at the end of loop over digitizers
  //
  
  if (GetOutRunLoader() == 0x0)
   {
     IlcError("fOutRunLoader is null");
     return;
   }
  
  fEvent++;
  fNrOfEventsWritten++;
  
  if (fOutRunLoader)
  {
     IlcRunLoader* inrl = IlcRunLoader::GetRunLoader(GetInputFolderName(0));
     IlcHeader* outheader = fOutRunLoader->GetHeader();
     IlcHeader* inheader = inrl->GetHeader();
     if (inheader == 0x0)
     {
       inrl->LoadHeader();
       inheader = inrl->GetHeader();
       if (inheader == 0x0) IlcFatal("Can not get header from input 0");
     }
     
     outheader->SetNprimary(inheader->GetNprimary());
     outheader->SetNtrack(inheader->GetNtrack());
     outheader->SetEvent(inheader->GetEvent());
     outheader->SetEventNrInRun(inheader->GetEventNrInRun());
     outheader->SetNvertex(inheader->GetNvertex());
     fOutRunLoader->TreeE()->Fill();
  }
      
  if (fCopyTreesFromInput > -1) 
   {
    //this is sensless since no information would be coherent in case of merging
    //
    IlcWarning("Copy trees from input: Copy or link files manually");
    return;
   }
}
//_______________________________________________________________________

void IlcDigitizationInput::FinishGlobal()
{
  //
  // called at the end of Exec
  // save unique objects to the output file
  //
  if (GetOutRunLoader() == 0x0)
   {
     IlcError("Can not get RunLoader from Output Stream folder");
     return;
   }
  GetOutRunLoader()->CdGAFile();
  this->Write(NULL, TObject::kOverwrite);
  if (fOutRunLoader)
   {
     fOutRunLoader->WriteHeader("OVERWRITE");
     fOutRunLoader->WriteRunLoader("OVERWRITE");
     TFolder* outfolder = fOutRunLoader->GetEventFolder();
     if (outfolder == 0x0)
     {
       IlcError("Can not get Event Folder");
       return;
     }    
  
     IlcRunLoader* inRN = IlcRunLoader::GetRunLoader(GetInputFolderName(0));
     outfolder->Add(inRN->GetIlcRun());
     fOutRunLoader->WriteIlcRun("OVERWRITE");
   }
   
  if (fCopyTreesFromInput > -1) 
   {
     //copy files manually
   }
}
//_______________________________________________________________________

Int_t  IlcDigitizationInput::GetNParticles(Int_t event) const
{
// return number of particles in all input files for a given
// event (as numbered in the output file)
// return -1 if some file cannot be accessed

  Int_t sum = 0;
  Int_t sumI;
  for (Int_t i = 0; i < fNinputs; i++) {
    sumI = GetNParticles(GetInputEventNumber(event,i), i);
    if (sumI < 0) return -1;
    sum += sumI;
  }
  return sum;
}
//_______________________________________________________________________

Int_t  IlcDigitizationInput::GetNParticles(Int_t /*event*/, Int_t /*input*/) const
{
// return number of particles in input file input for a given
// event (as numbered in this input file)
// return -1 if some error

// Must be revised in the version with IlcStream

  return -1;

}

//_______________________________________________________________________
Int_t* IlcDigitizationInput::GetInputEventNumbers(Int_t event) const
{
// return pointer to an int array with input event numbers which were
// merged in the output event event

// simplified for now, implement later
  Int_t * a = new Int_t[MAXSTREAMSTOMERGE];
  for (Int_t i = 0; i < fNinputs; i++) {
    a[i] = event;
  }
  return a;
}
//_______________________________________________________________________
Int_t IlcDigitizationInput::GetInputEventNumber(Int_t event, Int_t /*input*/) const
{
// return an event number of an eventInput from input file input
// which was merged to create output event event

// simplified for now, implement later
  return event;
}
//_______________________________________________________________________
TParticle* IlcDigitizationInput::GetParticle(Int_t i, Int_t event) const
{
// return pointer to particle with index i (index with mask)

// decode the MASK
  Int_t input = i/fkMASKSTEP;
  return GetParticle(i,input,GetInputEventNumber(event,input));
}

//_______________________________________________________________________
TParticle* IlcDigitizationInput::GetParticle(Int_t /*i*/, Int_t /*input*/, Int_t /*event*/) const
{
// return pointer to particle with index i in the input file input
// (index without mask)
// event is the event number in the file input
// return 0 i fit does not exist

// Must be revised in the version with IlcStream

  return 0;
}

//_______________________________________________________________________
const TString& IlcDigitizationInput::GetInputFolderName(Int_t i) const
{
  //
  // Get the input Folder Name
  //
  IlcStream* stream = dynamic_cast<IlcStream*>(fInputStreams->At(i));
  if (stream == 0x0)
    IlcFatal(Form("Can not get the input stream. Index = %d. Exiting",i));
  return stream->GetFolderName();
}
//_______________________________________________________________________

const char* IlcDigitizationInput::GetOutputFolderName()
{
  //
  // Get output folder name
  //
  return GetOutRunLoader()->GetEventFolder()->GetName();
}
//_______________________________________________________________________

IlcRunLoader* IlcDigitizationInput::GetOutRunLoader()
{
  //
  // Returns Run Loader
  //
  if (fOutRunLoader) return fOutRunLoader;
  
  if ( fOutputFileName.IsNull() )
   {//guard that sombody calls it without settting file name
    IlcDebug(1,"Output file name is empty. Using Input 0 for output");
    return IlcRunLoader::GetRunLoader(GetInputFolderName(0));
   }
//  InitOutputGlobal();
  return fOutRunLoader;
}
//_______________________________________________________________________

TString IlcDigitizationInput::GetInputFileName(Int_t input, Int_t order) const 
{
// returns file name of the order-th file in the input stream input
// returns empty string if such file does not exist
// first input stream is 0
// first file in the input stream is 0
  TString fileName("");
  if (input >= fNinputs) return fileName;
  IlcStream * stream = static_cast<IlcStream*>(fInputStreams->At(input));
  if (order > stream->GetNInputFiles()) return fileName;
  fileName = stream->GetFileName(order);
  return fileName;
}
