#include <iostream>
#include <fstream>

#include <TAlienCollection.h>
#include <TFile.h>
#include <TGrid.h>
#include <TGridResult.h>
#include <TMath.h>
#include <TROOT.h>
#include <TString.h>
#include <TSystem.h>

#include "IlcCDBManager.h"
#include "IlcDAQ.h"
#include "IlcLog.h"
#include "IlcQA.h"
#include "IlcQADataMakerSteer.h"
#include "IlcRawReader.h"
#include "IlcRawReaderRoot.h"
#include "IlcTRDrawStreamBase.h"
#include "IlcGeomManager.h"

TString ClassName() { return "rawqa" ; } 

//________________________________qa______________________________________
void rawqa(const Int_t runNumber, Int_t maxFiles = 10, const char* year = "08") 
{	
//	char * kDefaultOCDBStorage = Form("alien://folder=/ilc/data/20%s/LHC%sd/OCDB/", year, year) ; 
	char * kDefaultOCDBStorage = Form("local://$ILC_ROOT/OCDB") ; 
	//IlcQA::SetQARefStorage(Form("%s%s/", IlcQA::GetQARefDefaultStorage(), year)) ;  
	IlcQA::SetQARefStorage("local://$ILC_ROOT/QAref") ;
	
	UInt_t maxEvents = 99999 ;
	if ( maxFiles < 0 ) {
		maxEvents = TMath::Abs(maxFiles) ; 
		maxFiles = 99 ; 
	}
	IlcLog::SetGlobalDebugLevel(0) ; 
	// connect to the grid 
	TGrid * grid = 0x0 ; 
//       	grid = TGrid::Connect("alien://") ; 
		
	Bool_t detIn[IlcDAQ::kNDetectors] = {kFALSE} ;
	TString detNameOff[IlcDAQ::kNDetectors] = {"ITS", "ITS", "ITS", "TPC", "TRD", "TOF", "HMPID", "PHOS", "PHOS", "PMD", "MUON", "MUON", "FMD", "T0", "VZERO", "ZDC", "ACORDE", "TRG", "EMCAL", "DAQ_TEST", "HLT"} ; 
	// make the file name pattern year and run number
	TString pattern;
	pattern.Form("%9d",runNumber);
	pattern.ReplaceAll(" ", "0") ; 
	pattern.Prepend(year);
	pattern.Append("*.root");
	// find the files associated to this run
	TGridResult * result = 0x0 ; 
	Bool_t local = kFALSE ; 
	if (grid) { // get the list of files from AliEn directly 
		TString collectionFile(pattern) ; 
		collectionFile.ReplaceAll("*.root", ".xml") ; 
		if ( gSystem->AccessPathName(collectionFile) == 0 ) { // get the list of files from an a-priori created collection file
			TAlienCollection collection(collectionFile.Data(), maxFiles) ; 
			result = collection.GetGridResult("", 0, 0); 
		} else { 
			TString baseDir; 
			baseDir.Form("/ilc/data/20%s/",year);
			result = grid->Query(baseDir, pattern) ;  
		}
	} else {
	   // get the list of files from the local current directory 
	    local = kTRUE ; 
	    char line[100] ; 
	    sprintf(line, ".! ls %s*.root > tempo.txt", pattern.Data()) ; 
	    gROOT->ProcessLine(line) ;
	}
	
	IlcLog::Flush();
	ifstream in ; 
	if (local) 
		in.open("tempo.txt", ifstream::in) ; 

	TString detectors  = ""; 
	TString detectorsW = ""; 
	UShort_t file = 0 ; 
	UShort_t filesProcessed = 0 ; 
	UShort_t eventsProcessed = 0 ; 
	IlcCDBManager* man = IlcCDBManager::Instance();
	man->SetDefaultStorage(kDefaultOCDBStorage) ;  
	man->SetRun(runNumber) ; 
	IlcQAManager qam("rec") ; 
  qam->SetDefaultStorage(IlcQA::GetQARefStorage()) ; 
  qam->SetRun(runNumber) ; 
    
	IlcGeomManager::LoadGeometry();
	for ( file = 0 ; file < maxFiles ; file++) {
		if ( qas.GetCurrentEvent() >= maxEvents) 
			break ;

		TString fileName ; 
		if ( local) {
			in >> fileName ;
		} 
		else 
			fileName = result->GetKey(file, "turl");
	       	if ( fileName == "" )  
		        break ;
		if ( fileName.Contains("tag") )
			continue; 
                filesProcessed++ ;
		char input[200] ; 
		if (local) 
			sprintf(input, "%s", fileName.Data()) ; 
		else 
			sprintf(input, "%s", result->GetKey(file, "turl")); 
		IlcInfo(Form("Proccessing file # %d --> %s", file, input)) ;
		IlcLog::Flush();
		// check which detectors are present 
		IlcRawReader * rawReader = new IlcRawReaderRoot(input);
		IlcTRDrawStreamBase::SetRawStreamVersion("TB");
		while ( rawReader->NextEvent() ) {
			man->SetRun(rawReader->GetRunNumber());
			qam->SetRun(rawReader->GetRunNumber());
			IlcLog::Flush();
			UChar_t * data ; 
			while (rawReader->ReadNextData(data)) {
				Int_t detID = rawReader->GetDetectorID();
				if (detID < 0 || detID >= IlcDAQ::kNDetectors) {
					IlcError("Wrong detector ID! Skipping payload...");
					continue;
				}
				detIn[detID] = kTRUE ; 
			}
			for (Int_t detID = 0; detID < IlcDAQ::kNDetectors ; detID++) {
				if (detIn[detID]) {
					if ( ! detectors.Contains(detNameOff[detID]) ) {
						detectors.Append(detNameOff[detID]) ;
						detectors.Append(" ") ;
					}
				}
			}
			if ( !detectors.IsNull() )
				break ; 
		}
		if ( !detectors.IsNull() ) {
			qam.SetMaxEvents(maxEvents) ; 	
			detectorsW = qam.Run(detectors, rawReader) ;
			qam.Reset() ;
		} else {
			IlcError("No valid detectors found") ; 
		} 
		delete rawReader ;
		eventsProcessed += qas.GetCurrentEvent() ; 
	}
	IlcLog::Flush();
	qam.Merge(runNumber) ; 
	
	IlcLog::Flush();
	// The summary 
	IlcInfo(Form("\n\n********** Summary for run %d **********", runNumber)) ; 
	printf("     detectors present in the run        : %s\n", detectors.Data()) ; 
	printf("     detectors present in the run with QA: %s\n", detectorsW.Data()) ; 
	printf("     number of files/events processed    : %d/%d\n", filesProcessed, eventsProcessed) ; 
	TFile * qaResult = TFile::Open(IlcQA::GetQAResultFileName()) ; 
	if ( qaResult ) {
		IlcQA * qa = dynamic_cast<IlcQA *>(qaResult->Get(IlcQA::GetQAName())) ; 
		if ( qa) {
			for (Int_t index = 0 ; index < IlcQA::kNDET ; index++)
				if (detectorsW.Contains(IlcQA::GetDetName(IlcQA::DETECTORINDEX_t(index)))) 
					qa->ShowStatus(IlcQA::DETECTORINDEX_t(index)) ;
		} else {
			IlcError(Form("%s not found in %s !", IlcQA::GetQAName(), IlcQA::GetQAResultFileName())) ; 
		}
	} else {
		IlcError(Form("%s has not been produced !", IlcQA::GetQAResultFileName())) ; 
	}
}
