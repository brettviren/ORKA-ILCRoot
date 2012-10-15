// This macro transfers OCDB data of one single ("detName") or all ILC detectors from one location to another, 
// It is possible to set new run range, path etc... 
// draining "align" objects is in general to be avoided, since those present in the repository might not be updated
// some paths might be missing hereafter: this can be checked in advance by means of "listCdbEntries.sh" run on the
// starting OCDB folder
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "IlcCDBManager.h"
#include "IlcCDBStorage.h"
#include "IlcCDBEntry.h"
#include "IlcCDBId.h"
#include <TString.h>
#include <TList.h>
#endif

void CDBToGrid(const char *detName="", const char* fromUri="local://$ILC_ROOT/OCDB", 
	const char* toUri="local://newOCDB")
{

	IlcCDBManager *man = IlcCDBManager::Instance();
	man->SetDefaultStorage(fromUri);
	IlcCDBStorage *dest = man->GetStorage(toUri);
	man->SetRun(0);

	TList* calib=0;
	TList* align=0;
	TList* config=0;

	if(detName == "")
	{
		// drain by cdb-path
		calib = man->GetAll("*/Calib/*");
		//align = man->GetAll("*/Align/Data");
		config = man->GetAll("*/Config/*");

		// drain everything, really!
		//calib = man->GetAll("*");

	} else {
		// drain calibration and alignment for detector "detName"

		TString calibPath = detName;
		TString alignPath = calibPath;
		calibPath+="/Calib/*";	
		alignPath+="/Align/Data";

		//calib = man->GetAll(calibPath);
		//align = man->GetAll(alignPath);

		// drain everything, really!
		calib = man->GetAll(Form("%s/*",detName));
	}

	IlcCDBEntry *entry;

	Int_t ok=0; TString failed="";
	if(calib){
		ok=0; failed="";
		for(int i=0;i<calib->GetEntries();i++){

			entry = (IlcCDBEntry*) calib->At(i);
			entry->GetId().SetRunRange(0,IlcCDBRunRange::Infinity());

			TString path=entry->GetId().GetPath();

			printf("%s\n",path.Data());

			if (path == "ITS/Resp/RespSDD") entry->GetId().SetPath("ITS/Calib/RespSDD"); // bug in ITS/Resp/RespSDD

			if(path == "TOF/Calib/Par" || path == "TOF/Calib/SimPar" || path.Contains("TOF/CDB")) continue;

			if (dest->Put(entry)) {
				ok++;
			} else {
				failed += path.Data(); failed += " "; 
			}
		}
		printf("\n************ CALIB *********** \n");
		printf("************ Stored %d objects over %d *********** \n", ok, calib->GetEntries());
		if(failed != ""){
			printf("***** List of failed objects: %s \n", failed.Data());
		}
	}


	if(align){
		ok=0; failed="";
		for(int i=0;i<align->GetEntries();i++){

			entry = (IlcCDBEntry*) align->At(i);
			entry->GetId().SetRunRange(0,IlcCDBRunRange::Infinity());
			TString path=entry->GetId().GetPath();

			if (dest->Put(entry)) {
				ok++;
			} else {
				failed += path.Data(); failed += " "; 
			}
		}
		printf("\n************ ALIGN *********** \n");
		printf("************ Stored %d objects over %d *********** \n", ok, align->GetEntries());
		if(failed != ""){
			printf("***** List of failed objects: %s \n", failed.Data());
		}
	}

	if(config){
		ok=0; failed = "";
		for(int i=0;i<config->GetEntries();i++){

			entry = (IlcCDBEntry*) config->At(i);
			entry->GetId().SetRunRange(0,IlcCDBRunRange::Infinity());
			TString path=entry->GetId().GetPath();

			if (dest->Put(entry)) {
				ok++;
			} else {
				failed += path.Data(); failed += " "; 
			}
		}
		printf("\n************ CONFIG *********** \n");
		printf("************ Stored %d objects over %d *********** \n", ok, config->GetEntries());
		if(failed != ""){
			printf("***** List of failed objects: %s \n", failed.Data());
		}
	}

	man->Destroy();

}
