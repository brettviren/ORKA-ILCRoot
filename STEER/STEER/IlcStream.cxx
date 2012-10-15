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

/* $Id: IlcStream.cxx 50615 2011-07-16 23:19:19Z hristov $ */

#include <TROOT.h>
#include <TFile.h>
#include <TObjString.h>
#include <TTree.h>

#include "IlcLog.h"
#include "IlcLoader.h"
#include "IlcRun.h"
#include "IlcStream.h"

////////////////////////////////////////////////////////////////////////
//
// IlcStream.cxx
//
// - store file names associated with a given stream
// - open and close files
// - return serial event number of the next event in the stream
// and the TFile pointer for a proper file
//  Author: Jiri Chudoba (CERN), 2001
//
////////////////////////////////////////////////////////////////////////

ClassImp(IlcStream)

IlcStream::IlcStream():
  fLastEventSerialNr(-1),
  fLastEventNr(0),
  fCurrentFileIndex(-1),
  fEvents(0),
  fMode(0),
  fFileNames(0x0),
  fEventFolderName(0)
{
  // root requires default ctor, where no new objects can be created
  // do not use this ctor, it is supplied only for root needs
}
//_______________________________________________________________________

IlcStream::IlcStream(const char* foldername,Option_t *option):
  fLastEventSerialNr(-1),
  fLastEventNr(0),
  fCurrentFileIndex(-1),
  fEvents(0),
  fMode(option),
  fFileNames(new TObjArray(1)),
  fEventFolderName(foldername)
{
// ctor
}
//_______________________________________________________________________

IlcStream::IlcStream(const IlcStream &as):
  TNamed(as),
  fLastEventSerialNr(-1),
  fLastEventNr(0),
  fCurrentFileIndex(-1),
  fEvents(0),
  fMode(0),
  fFileNames(0x0),
  fEventFolderName(" ")
{
  //
  // Copy ctor
  //
  as.Copy(*this);
}
//_______________________________________________________________________

IlcStream::~IlcStream()
{
// default dtor
  delete IlcRunLoader::GetRunLoader(fEventFolderName); //clear the eventuall session
  if (fFileNames) delete fFileNames;
}
//_______________________________________________________________________

void IlcStream::Copy(TObject &) const
{
  //
  // Copy function
  //
  IlcFatal("Not implemented!");
}
//_______________________________________________________________________

void IlcStream::AddFile(const char *fileName)
{
// stores the name of the file
  TObjString *name = new TObjString(fileName);
  fFileNames->Add(name);
}
//_______________________________________________________________________

Bool_t IlcStream::NextEventInStream()
{
// returns kFALSE if no more events
// returns kTRUE and the serial nr of the next event
// fCurrentFile points to the file containing offered event

// no files given:
  if (fFileNames->GetLast() < 0) return kFALSE;
  
  IlcRunLoader* currentloader = IlcRunLoader::GetRunLoader(fEventFolderName);
  if (currentloader == 0x0) 
   {
    IlcDebug(1, Form(
         "Can not get RL from folder named %s. Attempting to open next file",
         fEventFolderName.Data()));
    Int_t res = OpenNextFile();
    if ( res == 0) return kFALSE;
    currentloader = IlcRunLoader::GetRunLoader(fEventFolderName);
   }
  
  if (fLastEventSerialNr+1 >= fEvents) 
   {
    if (!OpenNextFile()) return kFALSE;
   }
  IlcDebug(1, Form("Trying to get event %d",fLastEventSerialNr+1));
  currentloader->GetEvent(++fLastEventSerialNr);
  return kTRUE;
}
//_______________________________________________________________________

void IlcStream::ChangeMode(Option_t* option)
{
  // set the mode to READ or UPDATE, reopen file with the new mode
  // only change from UPDATE to READ have sense in the current scheme,
  // other changes are possible but not usefull

  fMode = option;
  IlcRunLoader* currentloader = IlcRunLoader::GetRunLoader(fEventFolderName);
  if (currentloader) {
    delete currentloader;
    fCurrentFileIndex--;
    OpenNextFile();
  }
}
//_______________________________________________________________________

Bool_t IlcStream::OpenNextFile()
{
  //
  // Opens next file in the list
  //
  if (++fCurrentFileIndex > fFileNames->GetLast()) {
    IlcInfo("No more files in the stream") ;
    return kFALSE;
  }

  const char* filename =   static_cast<TObjString*>(fFileNames->At(fCurrentFileIndex))->GetName();

// check if the file was already opened by some other code
  TFile *f = (TFile *)(gROOT->GetListOfFiles()->FindObject(filename));
  if (f) f->Close();

  IlcRunLoader* currentloader = IlcRunLoader::GetRunLoader(fEventFolderName);
  
  if (currentloader) 
   {
     delete currentloader;
   }
  
  currentloader = IlcRunLoader::Open(filename,fEventFolderName,fMode);
  

  if (currentloader == 0x0) 
   {
// cannot open file specified on input. Do not skip it silently.
    IlcError("Cannot open session ");
    return kFALSE;
   }
   
// find nr of events in the given file  
  
  if ( IlcLoader::TestFileOption(fMode) )//tests if file is opened in read or update mode
   {
    Int_t res = currentloader->LoadHeader();
    if (res)
     {
       IlcError("Problems with loading header");
       return kFALSE;
     }
    fEvents = static_cast<Int_t>(currentloader->TreeE()->GetEntries());
   }
  else
    {
     //if it is new, create or recreate there is no chance to find header in file
      fEvents = 0;
    }
   
  fLastEventSerialNr = -1;
  return kTRUE;
}
//_______________________________________________________________________

Bool_t IlcStream::ImportgIlc()
{
  //
  // Imports gIlc object from file
  //
  if (fFileNames->GetLast() < 0) return kFALSE;
  
  IlcRunLoader* currentloader = IlcRunLoader::GetRunLoader(fEventFolderName);
  if (!currentloader) 
   {
    if (!OpenNextFile()) return kFALSE;
    currentloader = IlcRunLoader::GetRunLoader(fEventFolderName);
   }
  currentloader->LoadgIlc();
  gIlc = currentloader->GetIlcRun();
  if (!gIlc)  return kFALSE;
  return kTRUE;
}

//_______________________________________________________________________
TString IlcStream::GetFileName(Int_t order) const
{
  // returns name of the order-th file
  // returns empty string if such file does not exist
  // first file in the input stream is 0
  TString fileName("");
  if (order > fFileNames->GetLast()) return fileName;
  TObjString *fileNameStored = dynamic_cast<TObjString*>(fFileNames->At(order));
  if (fileNameStored) fileName = fileNameStored->GetString();
  return fileName;
}

