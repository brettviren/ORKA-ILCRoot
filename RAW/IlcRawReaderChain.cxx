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
///
/// This is a class for reading raw data from a root chain.
/// There are two constructors available - one from a text file containing the
/// list of root raw-data files to be processed and one directly from
/// TFileCollection.
///
/// cvetan.cheshkov@cern.ch 29/07/2008
///
///////////////////////////////////////////////////////////////////////////////

#include <TChain.h>
#include <TFileCollection.h>
#include <TEntryList.h>
#include "TGridCollection.h"
#include <TPluginManager.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>
#include <TKey.h>
#include <TGrid.h>
#include <TGridResult.h>

#include "IlcRawReaderChain.h"
#include "IlcRawVEvent.h"
#include "IlcLog.h"

ClassImp(IlcRawReaderChain)

TString IlcRawReaderChain::fgSearchPath = "/ilc/data";

IlcRawReaderChain::IlcRawReaderChain() :
  IlcRawReaderRoot(),
  fChain(NULL)
{
  // default constructor
}

IlcRawReaderChain::IlcRawReaderChain(const char* fileName) :
  IlcRawReaderRoot(),
  fChain(NULL)
{
// create raw-reader objects which takes as an input a root chain
// either from the file list found in 'fileName' (IsCollection = true)
// or from entry list found in 'filename' (IsCollection = false)
// The entry-list syntax follows root convetion: filename.root/listname

  fChain = new TChain("RAW");

  TString fileNameStr = fileName;
  if (fileNameStr.EndsWith(".xml")) {

    TGridCollection *collection = NULL;
    TPluginManager* pluginManager = gROOT->GetPluginManager();
    TPluginHandler* pluginHandler = pluginManager->FindHandler("TGridCollection", "ilc");
    if (!pluginHandler) {
      pluginManager->AddHandler("TGridCollection", "ilc", 
				"IlcXMLCollection", "ANALYSISilc", "IlcXMLCollection(const char*)");
      pluginHandler = pluginManager->FindHandler("TGridCollection", "ilc");
    }
    gSystem->Load("libANALYSIS");
    if (pluginHandler && (pluginHandler->LoadPlugin() == 0)) {
      collection = (TGridCollection*)pluginHandler->ExecPlugin(1,fileNameStr.Data());
    }
    else {
      fIsValid = kFALSE;
      return;
    }
    collection->Reset();
    Bool_t elistsExist = kFALSE;
    TEntryList *elist = new TEntryList();
    while (collection->Next()) {
      fChain->Add(collection->GetTURL(""));
      TEntryList *list = (TEntryList *)collection->GetEntryList("");
      if (list) {
	list->SetTreeName("RAW");
	list->SetFileName(collection->GetTURL(""));
	elist->Add(list);
	elistsExist = kTRUE;
      }
    }
    if (elistsExist) {
      fChain->SetEntryList(elist,"ne");
    }
    else {
      Info("IlcRawReaderChain", "no entry lists found in %s. Using all entries", fileNameStr.Data());
      delete elist;
    }
  }
  else if (fileNameStr.EndsWith(".root")) {

    TDirectory* dir = gDirectory;
    TFile *listFile = TFile::Open(fileNameStr.Data());
    dir->cd();
    if (!listFile || !listFile->IsOpen()) {
      Error("IlcRawReaderChain", "could not open file %s", fileNameStr.Data());
      fIsValid = kFALSE;
      return;
    }

    TEntryList *elist = NULL;
    TKey *key = NULL;
    TIter nextkey(listFile->GetListOfKeys());
    while ((key=(TKey*)nextkey())){
      if (strcmp("TEntryList", key->GetClassName())==0){
        elist = (TEntryList*)key->ReadObj();
      }
    }
    if (!elist) {
      Error("IlcRawReaderChain", "no TEntryList found in %s", fileNameStr.Data());
      fIsValid = kFALSE;
      return;
    }

    TEntryList *templist = NULL;
    TList *elists = elist->GetLists();
    TIter next(elists);
    while((templist = (TEntryList*)next())){
      Info("IlcRawReaderChain", "%s added to the chain", templist->GetFileName());
      fChain->Add(templist->GetFileName());
    }
    fChain->SetEntryList(elist,"ne");
  }
  else {

    TFileCollection collection("RAW",
			       "Collection with raw-data files",
			       fileNameStr.Data());

    if (!fChain->AddFileInfoList((TCollection*)(collection.GetList()))) {
      Error("IlcRawReaderChain","Bad file list in collection, the chain is empty");
      fIsValid = kFALSE;
      return;
    }
  }

  fChain->SetBranchStatus("*",1);
  fChain->SetBranchAddress("rawevent",&fEvent,&fBranch);
}

IlcRawReaderChain::IlcRawReaderChain(TFileCollection *collection) :
  IlcRawReaderRoot(),
  fChain(NULL)
{
// create raw-reader objects which takes as an input a root chain
// from a root file collection

  fChain = new TChain("RAW");
  if (!fChain->AddFileInfoList((TCollection*)(collection->GetList()))) {
    Error("IlcRawReaderChain","Bad file list in collection, the chain is empty");
    fIsValid = kFALSE;
    return;
  }

  fChain->SetBranchStatus("*",1);
  fChain->SetBranchAddress("rawevent",&fEvent,&fBranch);
}

IlcRawReaderChain::IlcRawReaderChain(TChain *chain) :
  IlcRawReaderRoot(),
  fChain(chain)
{
// create raw-reader objects which takes as an input a root chain
// from a root file collection

  if (!fChain) {
    fIsValid = kFALSE;
    return;
  }

  fChain->SetBranchStatus("*",1);
  fChain->SetBranchAddress("rawevent",&fEvent,&fBranch);
}

IlcRawReaderChain::IlcRawReaderChain(TEntryList *elist) :
  IlcRawReaderRoot(),
  fChain(NULL)
{
// create raw-reader objects which takes as an input a root chain
// from a root file collection

  if (!elist) {
    fIsValid = kFALSE;
    return;
  }

  fChain = new TChain("RAW");

  TEntryList *templist = NULL;
  TList *elists = elist->GetLists();
  TIter next(elists);
  while((templist = (TEntryList*)next())){
    fChain->Add(templist->GetFileName());
  }
  fChain->SetEntryList(elist,"ne");

  fChain->SetBranchStatus("*",1);
  fChain->SetBranchAddress("rawevent",&fEvent,&fBranch);
}

IlcRawReaderChain::IlcRawReaderChain(Int_t runNumber) :
  IlcRawReaderRoot(),
  fChain(NULL)
{
// create raw-reader objects which takes as an input a root chain
// with the raw-data files for a given run
// It queries alien FC in order to do that and therefore
// it needs alien API to be enabled

  if (runNumber <= 0) {
    Error("IlcRawReaderChain","Bad run number:%d",runNumber);
    fIsValid = kFALSE;
  }

  if (!gGrid) TGrid::Connect("alien://");
  if (!gGrid) {
    fIsValid = kFALSE;
    return;
  }

  if (fgSearchPath.IsNull()) fgSearchPath = "/ilc/data";
  TGridResult *res = gGrid->Query(fgSearchPath.Data(),Form("%09d/raw/*%09d*.root",runNumber,runNumber));
  Int_t nFiles = res->GetEntries();
  if (!nFiles) {
    Error("IlcRawReaderChain","No raw-data files found for run %d",runNumber);
    fIsValid = kFALSE;
    delete res;
    return;
  }

  fChain = new TChain("RAW");
  for (Int_t i = 0; i < nFiles; i++) {
    TString filename = res->GetKey(i, "turl");
    if(filename == "") continue;
    fChain->Add(filename.Data());
  }
  delete res;

  fChain->SetBranchStatus("*",1);
  fChain->SetBranchAddress("rawevent",&fEvent,&fBranch);
}


IlcRawReaderChain::IlcRawReaderChain(const IlcRawReaderChain& rawReader) :
  IlcRawReaderRoot(rawReader),
  fChain(rawReader.fChain)
{
// copy constructor
}

IlcRawReaderChain& IlcRawReaderChain::operator = (const IlcRawReaderChain& 
						  rawReader)
{
// assignment operator

  this->~IlcRawReaderChain();
  new(this) IlcRawReaderChain(rawReader);
  return *this;
}

IlcRawReaderChain::~IlcRawReaderChain()
{
// delete objects and close root file

  if (fChain) {
    delete fChain;
    fChain = NULL;
  }
}

Bool_t IlcRawReaderChain::NextEvent()
{
// go to the next event in the root file

  if (!fChain || !fChain->GetListOfFiles()->GetEntriesFast()) return kFALSE;

  do {
    delete fEvent;
    fEvent = NULL;
    fEventHeader = NULL;
    Long64_t treeEntry = fChain->LoadTree(fEventIndex+1);
    if (!fBranch)
      return kFALSE;
    if (fBranch->GetEntry(treeEntry) <= 0)
      return kFALSE;
    fEventHeader = fEvent->GetHeader();
    fEventIndex++;
  } while (!IsEventSelected());
  fEventNumber++;
  return Reset();
}

Bool_t IlcRawReaderChain::RewindEvents()
{
// go back to the beginning of the root file

  fEventIndex = -1;
  delete fEvent;
  fEvent = NULL;
  fEventHeader = NULL;
  fEventNumber = -1;
  return Reset();
}

Bool_t  IlcRawReaderChain::GotoEvent(Int_t event)
{
  // go to a particular event
  // Uses the absolute event index inside the
  // chain with raw data

  if (!fChain || !fChain->GetListOfFiles()->GetEntriesFast()) return kFALSE;

  delete fEvent;
  fEvent = NULL;
  fEventHeader = NULL;
  Long64_t treeEntry = fChain->LoadTree(event);
   if (!fBranch)
    return kFALSE;
  if (fBranch->GetEntry(treeEntry) <= 0)
    return kFALSE;
  fEventHeader = fEvent->GetHeader();
  fEventIndex = event;
  fEventNumber++;
  return Reset();
}

Int_t IlcRawReaderChain::GetNumberOfEvents() const
{
  // Get the total number of events in the chain
  // of raw-data files

  if (!fChain) return -1;

  return fChain->GetEntries();
}

void IlcRawReaderChain::SetSearchPath(const char* path)
{
  // set alien query search path
  IlcInfoGeneral("SetSearchPath",Form("Setting search path to \"%s\" (was \"%s\")",path,fgSearchPath.Data()));
  fgSearchPath = path;
}
