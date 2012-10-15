/**************************************************************************
 * Author: Panos Christakoglou.                                           *
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

/* $Id: IlcTagAnalysis.cxx 46825 2011-01-29 18:07:46Z morsch $ */

//-----------------------------------------------------------------
//           IlcTagAnalysis class
//   This is the class to deal with the tag analysis
//   Origin: Panos Christakoglou, UOA-CERN, Panos.Christakoglou@cern.ch
//-----------------------------------------------------------------

//ROOT
#include <Riostream.h>
#include <TSystem.h>
#include <TChain.h>
#include <TFile.h>
#include <TEventList.h>
#include <TEntryList.h>
#include <TTreeFormula.h>
#include <TMap.h>

//ROOT-AliEn
#include <TGridResult.h>

#include "IlcLog.h"

#include "IlcRunTag.h"
#include "IlcEventTag.h"
#include "IlcTagAnalysis.h"
#include "IlcEventTagCuts.h"
#include "IlcDetectorTagCuts.h"
#include "IlcLHCTagCuts.h"
#include "IlcRunTagCuts.h"
#include "IlcXMLCollection.h"

class TTree;

ClassImp(IlcTagAnalysis)

//___________________________________________________________________________
IlcTagAnalysis::IlcTagAnalysis(): 
  TObject(),
  ftagresult(0x0),
  fTagDirName(),
  fChain(0x0),
  fAnalysisType(),
  fGlobalList(0) {
  //Default constructor for a IlcTagAnalysis
}

//___________________________________________________________________________
IlcTagAnalysis::IlcTagAnalysis(const char* type): 
  TObject(),
  ftagresult(0x0),
  fTagDirName(),
  fChain(0x0),
  fAnalysisType(type),
  fGlobalList(0) {
  //constructor for a IlcTagAnalysis
}

//___________________________________________________________________________
IlcTagAnalysis::~IlcTagAnalysis() {
  //Default destructor for a IlcTagAnalysis
  if(ftagresult) delete ftagresult;
  if(fChain) delete fChain;
  if(fGlobalList) delete fGlobalList;
}

//___________________________________________________________________________
Bool_t  
IlcTagAnalysis::AddTagsFile(const char* alienUrl, Bool_t checkFile) 
{
  /// Add a single tags file to the chain
  ///
  /// If checkFile=kTRUE (default) the file is opened to check
  /// it can be and that it contains data.
  /// It's safer but a lot longer...
  
  if (!fChain) fChain = new TChain("T");

  if ( checkFile )
  {
    return ( fChain->AddFile(alienUrl,-1) > 0 );
  }
  else
  {
    return ( fChain->AddFile(alienUrl) > 0 );
  }

}

//___________________________________________________________________________
void IlcTagAnalysis::ChainLocalTags(const char *dirname) {
  //Searches the entries of the provided direcory
  //Chains the tags that are stored locally
  fTagDirName = dirname;
  TString fTagFilename;
  
  if (! fChain)  fChain = new TChain("T");
  const char * tagPattern = 0x0;
  if(fAnalysisType == "ESD") tagPattern = "ESD.tag.root";
  else if(fAnalysisType == "AOD") tagPattern = "AOD.tag.root";
  else IlcFatal("Only ESD and AOD type is implemented!!!");

  // Open the working directory
  void * dirp = gSystem->OpenDirectory(fTagDirName);
  const char * name = 0x0;
  // Add all files matching *pattern* to the chain
  while((name = gSystem->GetDirEntry(dirp))) {
    if (tagPattern && strstr(name,tagPattern)) { 
      fTagFilename = fTagDirName;
      fTagFilename += "/";
      fTagFilename += name;
	  	
      fChain->Add(fTagFilename);  
      printf("Tag file %s\n", fTagFilename.Data());
      
    }//pattern check
  }//directory loop
  //IlcInfo(Form("Chained tag files: %d ",fChain->GetEntries()));
 // IlcDebug(Form("Chained tag files: %d ",fChain->GetEntries()));
  fChain->ls();
  
}


//___________________________________________________________________________
TChain * IlcTagAnalysis::ChainGridTags(TGridResult *res) {
  //Loops overs the entries of the TGridResult
   //Chains the tags that are stored in the GRID
  ftagresult = res;
  Int_t nEntries = ftagresult->GetEntries();
 
  if (! fChain)  fChain = new TChain("T");

  TString gridname = "alien://";
  TString alienUrl;
 
  for(Int_t i = 0; i < nEntries; i++) {
    alienUrl = ftagresult->GetKey(i,"turl");
    fChain->Add(alienUrl);
  }//grid result loop
  return fChain;
}


//___________________________________________________________________________
TChain *IlcTagAnalysis::QueryTags(IlcRunTagCuts *runTagCuts, 
				  IlcLHCTagCuts *lhcTagCuts, 
				  IlcDetectorTagCuts *detTagCuts, 
				  IlcEventTagCuts *evTagCuts) {
  //Queries the tag chain using the defined 
  //event tag cuts from the IlcEventTagCuts object
  //and returns a TChain along with the associated TEventList
  IlcInfo(Form("Querying the tags........"));

  TString ilcFile;
  if(fAnalysisType == "ESD") ilcFile = "esdTree";
  else if(fAnalysisType == "AOD") ilcFile = "aodTree";
  else IlcFatal("Only ESD and AOD type is implemented!!!");

  //ESD file chain
  TChain *esdChain = new TChain(ilcFile.Data());
  //global entry list
  fGlobalList = new TEntryList();
  
  //Defining tag objects
  IlcRunTag   *tag     = new IlcRunTag;
  //  IlcEventTag *evTag   = 0x0;
  IlcFileTag  *flTag   = 0x0;

  fChain->SetBranchAddress("IlcTAG",&tag);

  TString guid;
  TString turl;
  TString path;

  TEntryList* localList = new TEntryList();

  Int_t iAccepted = 0;
  
  for(Int_t iEntry = 0; iEntry < fChain->GetEntries(); iEntry++) {
    fChain->GetEntry(iEntry);
    evTagCuts->InitializeTriggerClasses(tag->GetActiveTriggerClasses());

    if(runTagCuts->IsAccepted(tag)) {
      if(lhcTagCuts->IsAccepted(tag->GetLHCTag())) {
	if(detTagCuts->IsAccepted(tag->GetDetectorTags())) {
	  localList->Reset();
	  Int_t iEvents = tag->GetNEvents();
	  
	  for (int i = 0; i < iEvents; i++) {
	    //	    evTag = tag->GetEventTag(i);
	    flTag = tag->GetFileTagForEvent(i);
	    guid = flTag->GetGUID();
	    turl = flTag->GetTURL();
	    path = flTag->GetPath();
 	    localList->SetTreeName(ilcFile.Data());
 	    if(turl!="") localList->SetFileName(turl.Data());
 	    else localList->SetFileName(path.Data());

 	    if(evTagCuts->IsAccepted(tag->GetEventTag(i))) localList->Enter(i);
	  }

// 	  const TClonesArray *tagList = tag->GetEventTags();
// 	  for(Int_t i = 0; i < iEvents; i++) {
// 	    evTag = (IlcEventTag *) tagList->At(i);
// 	    guid = evTag->GetGUID(); 
// 	    turl = evTag->GetTURL(); 
// 	    path = evTag->GetPath();
// 	    localList->SetTreeName(ilcFile.Data());
// 	    if(turl!="") localList->SetFileName(turl.Data());
// 	    else localList->SetFileName(path.Data());
	    
// 	    if(evTagCuts->IsAccepted(evTag)) localList->Enter(i);
// 	  }//event loop
	  iAccepted += localList->GetN();
	  if(turl != "")      esdChain->AddFile(turl);
	  else if(path != "") esdChain->AddFile(path);
	  fGlobalList->Add(localList);
	}//detector tag cuts
      }//lhc tag cuts
    }//run tags cut
    tag->Clear();
  }//tag file loop
  IlcInfo(Form("Accepted events: %d", iAccepted));
  esdChain->ls();
  esdChain->SetEntryList(fGlobalList,"ne");
  delete tag;
  delete localList;
  
  return esdChain;
}

//___________________________________________________________________________
TChain *IlcTagAnalysis::QueryTags(const char *fRunCut, 
				  const char *fLHCCut, 
				  const char *fDetectorCut, 
				  const char *fEventCut) { 	 
  //Queries the tag chain using the defined 	 
  //event tag cuts from the IlcEventTagCuts object 	 
  //and returns a TChain along with the associated TEventList 	 
  IlcInfo(Form("Querying the tags........")); 	 

  TString ilcFile;
  if(fAnalysisType == "ESD") ilcFile = "esdTree";
  else if(fAnalysisType == "AOD") ilcFile = "aodTree";
  else IlcFatal("Only ESD and AOD type is implemented!!!");


  //ESD file chain
  TChain *esdChain = new TChain(ilcFile.Data());
  //global entry list
  fGlobalList = new TEntryList();
  
  //Defining tag objects 	 
  IlcRunTag   *tag   = new IlcRunTag; 	 
  //  IlcEventTag *evTag = 0x0;
  fChain->SetBranchAddress("IlcTAG",&tag); 	 
  
  TString guid; 	 
  TString turl; 	 
  TString path; 	 
  
  TTreeFormula *fRunFormula = new TTreeFormula("fRun",fRunCut,fChain); 	 
  TTreeFormula *fLHCFormula = new TTreeFormula("fLHC",fLHCCut,fChain); 	 
  TTreeFormula *fDetectorFormula = new TTreeFormula("fDetector",fDetectorCut,fChain);
  TTreeFormula *fEventFormula = new TTreeFormula("fEvent",fEventCut,fChain);
  
  TEntryList* localList = new TEntryList();

  Int_t current = -1; 
  Int_t iAccepted = 0; 	 
  
  for(Int_t iTagFiles = 0; iTagFiles < fChain->GetEntries(); iTagFiles++) {
    fChain->GetEntry(iTagFiles); 	 
    if (current != fChain->GetTreeNumber()) { 	 
      fRunFormula->UpdateFormulaLeaves(); 	 
      fLHCFormula->UpdateFormulaLeaves(); 	 
      fDetectorFormula->UpdateFormulaLeaves(); 	 
      fEventFormula->UpdateFormulaLeaves(); 	 
      current = fChain->GetTreeNumber(); 	 
    } 	 
    
    if(fRunFormula->EvalInstance(iTagFiles) == 1) { 	 
      if(fLHCFormula->EvalInstance(iTagFiles) == 1) { 	 
	if(fDetectorFormula->EvalInstance(iTagFiles) == 1) {
          localList->Reset(); 	 
	  //	  Int_t iEvents = fEventFormula->GetNdata(); 	 
	  // *** FIXME ***

// 	  const TClonesArray *tagList = tag->GetEventTags(); 	 
// 	  for(Int_t i = 0; i < iEvents; i++) { 	 
// 	    evTag = (IlcEventTag *) tagList->At(i); 	 
// 	    guid = evTag->GetGUID(); 	 
// 	    turl = evTag->GetTURL(); 	 
// 	    path = evTag->GetPath(); 	 
// 	    localList->SetTreeName(ilcFile.Data());
// 	    localList->SetFileName(turl.Data());
// 	    if(fEventFormula->EvalInstance(i) == 1) localList->Enter(i);
// 	  }//event loop 	 

	  if(path != "")      esdChain->AddFile(path); 	 
	  else if(turl != "") esdChain->AddFile(turl); 	 
	  fGlobalList->Add(localList);
	  iAccepted += localList->GetN();
	}//detector tag cuts
      }//lhc tag cuts
    }//run tag cut 	 
    tag->Clear();
  }//tag file loop 	 
  IlcInfo(Form("Accepted events: %d", iAccepted)); 	 
  esdChain->SetEntryList(fGlobalList,"ne"); 	 

  delete tag;
  delete localList;
  return esdChain; 	 
}

//___________________________________________________________________________
Bool_t 
IlcTagAnalysis::CreateXMLCollection(const char* name, 
                                    IlcRunTagCuts *runTagCuts, 
                                    IlcLHCTagCuts *lhcTagCuts, 
                                    IlcDetectorTagCuts *detTagCuts, 
                                    IlcEventTagCuts *evTagCuts) 
{
  /// Queries the tag chain using the defined run, lhc, detector and event tag objects
  /// and create a XML collection named "name.xml"
  /// if any of the runTagCuts, lhcTagCuts, detTagCuts or evTagCuts is NULL
  /// check on that object will be skipped.
  
  IlcInfo(Form("Creating the collection........"));
  
  if (!fChain) 
  {
    IlcError("fChain is NULL. Cannot make a collection from that !");
    return kFALSE;
  }
  
  IlcXMLCollection collection;
  collection.SetCollectionName(name);
  collection.WriteHeader();
  
  TString guid;
  TString turl;
  TString lfn;
  
  TEntryList localList;
  Int_t iAccepted = 0;
    
  Int_t iRejectedRun = 0;
  Int_t iRejectedLHC = 0;
  Int_t iRejectedDet = 0;
  Int_t iRejectedEvt = 0;
  
  Int_t iTotalEvents = 0;
  
  Int_t iAcceptedEvtInFile = 0;
  Int_t iRejectedEvtInFile = 0;
  
  //Defining tag objects
  IlcRunTag* tag = new IlcRunTag;
  fChain->SetBranchAddress("IlcTAG",&tag);

  Int_t iTagFiles = 0;
  
  //  IlcEventTag *evTag = 0x0;
  IlcFileTag  *flTag = 0x0;

  //  for(Int_t iTagFiles = 0; iTagFiles < fChain->GetListOfFiles()->GetEntries(); ++iTagFiles) 
  for(Int_t iRunTags = 0; iRunTags < fChain->GetEntries(); ++iRunTags) 
  {
    fChain->GetEntry(iRunTags);
    //Event list
    iTotalEvents += tag->GetNEvents();
    localList.Reset();
    
    evTagCuts->InitializeTriggerClasses(tag->GetActiveTriggerClasses());
    
    if ( !runTagCuts || ( runTagCuts && runTagCuts->IsAccepted(tag) ) ) 
      {
	if ( !lhcTagCuts || ( lhcTagCuts && lhcTagCuts->IsAccepted(tag->GetLHCTag())) ) 
	  {
	    if ( !detTagCuts || ( detTagCuts && detTagCuts->IsAccepted(tag->GetDetectorTags())) )
	      {
		for (int iChunk = 0; iChunk < tag->GetNFiles(); iChunk++, iTagFiles++) 
		  {
		    iRejectedEvtInFile = 0;
		    iAcceptedEvtInFile = 0;

		    localList.Reset();
		    
		    flTag = tag->GetFileTag(iChunk);
		    guid = flTag->GetGUID();
		    turl = flTag->GetTURL();
		    lfn = turl(8,turl.Length());
		    
		    for (int i = 0; i<flTag->GetNEvents(); i++) 
		      {
			//			evTag = flTag->GetEventTag(i);
			
			if( !evTagCuts || ( evTagCuts && evTagCuts->IsAccepted(flTag->GetEventTag(i))) )
			  {
			    localList.Enter(i);
			    iAcceptedEvtInFile++;
			  }
			else 
			  {
			    ++iRejectedEvt;
			    ++iRejectedEvtInFile;
			  }
		      }
		    // *** FIXME ***
//		Int_t i(0);

// 		TIter next(tag->GetEventTags());
// 		IlcEventTag* evTag(0x0);
// 		iRejectedEvtInFile = 0;
// 		iAcceptedEvtInFile = 0;
// 		while ( ( evTag = static_cast<IlcEventTag*>(next()) ) )
// 		  {
// 		    guid = evTag->GetGUID(); 
// 		    turl = evTag->GetTURL(); 
// 		    lfn = turl(8,turl.Length());
// 		    if( !evTagCuts || ( evTagCuts && evTagCuts->IsAccepted(evTag)) )
// 		      {
// 			localList.Enter(i);
// 			iAcceptedEvtInFile++;
// 		      }
// 		    else 
// 		      {
// 			++iRejectedEvt;
// 			++iRejectedEvtInFile;
// 		      }
// 		    ++i;
// 		  }//event loop
		    iAccepted += localList.GetN();
		    collection.WriteBody(iTagFiles+1,guid,lfn,turl,&localList,iAcceptedEvtInFile,iRejectedEvtInFile);
		  } // chunk loop
	      }//detector tag cuts
	    else {
	      iRejectedDet += tag->GetNEvents();
	    }
	  }//lhc tag cuts 
	else {
	  iRejectedLHC += tag->GetNEvents();
	}
      }//run tag cuts
    else {
      iRejectedRun += tag->GetNEvents();
    }
    tag->Clear();
  } //tag file loop
  
  collection.WriteSummary(iTotalEvents, iAccepted, iRejectedRun, iRejectedLHC, iRejectedDet, iRejectedEvt);
  collection.Export();
  
  return kTRUE;
}

//___________________________________________________________________________
Bool_t IlcTagAnalysis::CreateXMLCollection(const char* name, 
					   const char *fRunCut, 
					   const char *fLHCCut, 
					   const char *fDetectorCut, 
					   const char *fEventCut) {
  //Queries the tag chain using the defined 
  //event tag cuts from the IlcEventTagCuts object
  //and returns a XML collection
  IlcInfo(Form("Creating the collection........"));


  IlcXMLCollection *collection = new IlcXMLCollection();
  collection->SetCollectionName(name);
  collection->WriteHeader();

  TString guid;
  TString turl;
  TString lfn;
  TEntryList* localList = new TEntryList();
  
  Int_t iAccepted = 0;

  Int_t iRejectedRun = 0;
  Int_t iRejectedLHC = 0;
  Int_t iRejectedDet = 0;
  Int_t iRejectedEvt = 0;

  Int_t iTotalEvents = 0;

  Int_t iAcceptedEvtInFile = 0;
  Int_t iRejectedEvtInFile = 0;

  //Defining tag objects
  IlcRunTag *tag     = new IlcRunTag;
  //  IlcEventTag *evTag = 0x0;
  fChain->SetBranchAddress("IlcTAG",&tag);

  TTreeFormula *fRunFormula = new TTreeFormula("fRun",fRunCut,fChain);
  TTreeFormula *fLHCFormula = new TTreeFormula("fLHC",fLHCCut,fChain); 	 
  TTreeFormula *fDetectorFormula = new TTreeFormula("fDetector",fDetectorCut,fChain);
  TTreeFormula *fEventFormula = new TTreeFormula("fEvent",fEventCut,fChain);
  
  Int_t current = -1;

  for(Int_t iTagFiles = 0; iTagFiles < fChain->GetEntries(); iTagFiles++) {

    fChain->GetEntry(iTagFiles);
    if (current != fChain->GetTreeNumber()) {
      fRunFormula->UpdateFormulaLeaves();
      fLHCFormula->UpdateFormulaLeaves();
      fDetectorFormula->UpdateFormulaLeaves();
      fEventFormula->UpdateFormulaLeaves();
      current = fChain->GetTreeNumber();
     }
 
   //Event list
    iTotalEvents += tag->GetNEvents();
    localList->Reset();
    if(fRunFormula->EvalInstance(iTagFiles) == 1) {
      if(fLHCFormula->EvalInstance(iTagFiles) == 1) { 	 
	if(fDetectorFormula->EvalInstance(iTagFiles) == 1) { 	 
	  //	  Int_t iEvents = fEventFormula->GetNdata();
	  // *** FIXME ***


// 	  const TClonesArray *tagList = tag->GetEventTags();
// 	  iRejectedEvtInFile = 0;
// 	  iAcceptedEvtInFile = 0;
// 	  for(Int_t i = 0; i < iEvents; i++) {
// 	    evTag = (IlcEventTag *) tagList->At(i);
// 	    guid = evTag->GetGUID(); 
// 	    turl = evTag->GetTURL(); 
// 	    lfn = turl(8,turl.Length());
// 	    if(fEventFormula->EvalInstance(i) == 1) {
// 	      localList->Enter(i);
// 	      iAcceptedEvtInFile++;
// 	    }
// 	    else {
// 	      iRejectedEvt++;
// 	      iRejectedEvtInFile++;
// 	    }
// 	  }//event loop

	  collection->WriteBody(iTagFiles+1,guid,lfn,turl,localList,iAcceptedEvtInFile, iRejectedEvtInFile);
	  iAccepted += localList->GetN();
	}//detector tag cuts
	else {
	  iRejectedDet += tag->GetNEvents();
	}
      }//lhc tag cuts 
      else {
	iRejectedLHC += tag->GetNEvents();
      }
    }//run tag cuts
    else {
      iRejectedRun += tag->GetNEvents();
    }
  }//tag file loop
  collection->WriteSummary(iTotalEvents, iAccepted, iRejectedRun, iRejectedLHC, iRejectedDet, iRejectedEvt);
  collection->Export();
  
  delete tag;
  return kTRUE;
}

//___________________________________________________________________________
TChain *IlcTagAnalysis::GetInputChain(const char* system, const char *wn) {
  //returns the chain+event list - used in batch sessions
  // this function will be removed once the new root 
  // improvements are committed
  TString fsystem = system;
  Int_t iAccepted = 0;

  TChain *fAnalysisChain = 0;
  if(fAnalysisType == "ESD") fAnalysisChain = new TChain("esdTree");
  else if(fAnalysisType == "AOD") fAnalysisChain = new TChain("aodTree");
  else IlcFatal("Only ESD and AOD type is implemented!!!");
  
  //Event list
  TEventList *fEventList = new TEventList();
  IlcXMLCollection *collection = IlcXMLCollection::Open(wn);

  collection->Reset();
  while (collection->Next()) {
    IlcInfo(Form("Adding: %s",collection->GetTURL("")));
    if (fAnalysisChain) fAnalysisChain->Add(collection->GetTURL(""));
    TEntryList *list = (TEntryList *)collection->GetEventList("");
    for(Int_t i = 0; i < list->GetN(); i++) fEventList->Enter(iAccepted+list->GetEntry(i));

    if(fsystem == "pp") iAccepted += 100;
    else if(fsystem == "PbPb") iAccepted += 1;
  }

  if (fAnalysisChain) fAnalysisChain->SetEventList(fEventList);
  
  IlcInfo(Form("Number of selected events: %d",fEventList->GetN()));

  return fAnalysisChain;
}

//___________________________________________________________________________
TChain*
IlcTagAnalysis::CreateChainFromCollection(const char* collectionname, const char* treename)
{
  /// Build a TChain (with its TEntryList object attached) from an XML collection.
  /// Returned chain must be deleted by the client.

  TString streename(treename);
  if ( streename != "esdTree" && streename != "aodTree" )
  {
    IlcErrorClass("Only esdTree and aodTree implemented so far...");
    return 0x0;
  }
  
  TChain* chain = new TChain(streename.Data());

  // create the event list for the chain. Will be attached to the chain
  // which thus becomes the owner of it.
  TEntryList* elist = new TEntryList; 
  
  IlcXMLCollection* collection = IlcXMLCollection::Open(collectionname);

  // Tag selection summary per file
  TMap* tagCutSummary = new TMap();
  tagCutSummary->SetName("TagCutSumm");

  Int_t iAccepted = 0;
  
  collection->Reset();
  
  while (collection->Next()) 
  {
    IlcDebugClass(1,Form("Adding: %s",collection->GetTURL("")));
    chain->Add(collection->GetTURL(""));
    TEntryList *list = collection->GetEventList("");
    list->SetTreeName(streename.Data());
    list->SetFileName(collection->GetTURL(""));
    elist->Add(list);
    iAccepted += list->GetN();
    if (collection->GetCutSumm())
    {
      tagCutSummary->Add(new TObjString(collection->GetTURL("")), new TObjString(collection->GetCutSumm()));
    }
  }

  chain->SetEntryList(elist,"ne"); // ne => do not expand tree name and/or file names
  
  IlcDebugClass(1,Form("Number of selected events: %d",iAccepted));

  TList *aUserInfo = chain->GetUserInfo();
  aUserInfo->Add(tagCutSummary);

  Int_t iAccEv;
  Int_t iTotalEvents;
  Int_t iRejRun;
  Int_t iRejLHC;
  Int_t iRejDet;
  Int_t iRejEvt;

  collection->GetCollectionSummary(&iTotalEvents, &iAccEv, &iRejRun, &iRejLHC, &iRejDet, &iRejEvt);
 
  char nstr[2000];

  snprintf(nstr, 2000, "TotalEvents=%i", iTotalEvents);
  TObjString *iTotStr = new TObjString(nstr);
  aUserInfo->Add(iTotStr);

  snprintf(nstr, 2000, "AcceptedEvents=%i", iAccepted);
  TObjString *iAccStr = new TObjString(nstr);
  aUserInfo->Add(iAccStr);

  snprintf(nstr, 2000, "RejectedRun=%i", iRejRun);
  TObjString *iRejRunStr = new TObjString(nstr);
  aUserInfo->Add(iRejRunStr);

  snprintf(nstr, 2000, "RejectedLHC=%i", iRejLHC);
  TObjString *iRejLHCStr = new TObjString(nstr);
  aUserInfo->Add(iRejLHCStr);

  snprintf(nstr, 2000, "RejectedDet=%i", iRejDet);
  TObjString *iRejDetStr = new TObjString(nstr);
  aUserInfo->Add(iRejDetStr);

  snprintf(nstr, 2000, "RejectedEvt=%i", iRejEvt);
  TObjString *iRejEvtStr = new TObjString(nstr);
  aUserInfo->Add(iRejEvtStr);

  return chain;
}
