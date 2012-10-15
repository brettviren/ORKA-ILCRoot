#include "ARVersion.h"
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "IlcCDBManager.h"
#include "IlcCDBStorage.h"
#include "IlcCDBId.h"
#include "IlcCDBMetaData.h"
#include <TTree.h>
#include <TList.h>
#include <TObjString.h>
#include <TString.h>
#endif

void makeCDBSnapshotFromUserInfo(const char* defaultStorage, const char* esdFile, const char* snapshotFile)
{
    // read UserInfo from an esd tree and build the corresponding single-key snapshot from it
    // ATTENTION: it works if we are happy that all objects will be taken from the default CDB
    // Example input esd file: "alien:///ilc/data/2011/LHC11h_2/000168984/ESDs/pass2/11000168984001.12/IlcESDs.root"
    //

    IlcCDBManager *cdb = IlcCDBManager::Instance();
    cdb->SetDefaultStorage(defaultStorage);
    
    TFile *f = TFile::Open(esdFile);
    if(!f){
	Printf("Unable to open file \"%s\". Exiting.",esdFile);
	return;
    }
    TTree *tree = (TTree*) f->Get("esdTree");
    if(!tree){
	Printf("Could not get tree from file. Exiting.");
	return;
    }
    TList *ui = (TList*) tree->GetUserInfo();
    if(!ui){
	Printf("Could not get user info from tree. Exiting.");
	return;
    }
    TList *ids = (TList*) ui->At(2);
    if(!ids){
	Printf("Could not get CDB objects' ids from user info. Exiting.");
	return;
    }
    
    TListIter lIter(ids);
    TObjString *oStr = 0;
    while ((oStr = dynamic_cast<TObjString*> (lIter.Next()))) {
	TString printedId = oStr->GetString();
	// add here lines like the following if you don't want a given object in the snapshot
	// this should not be needed because the specific storages overwrite the snapshot-mode
	//if(printedId.Contains("ITS/Align/Data")||printedId.Contains("MUON/Align/Data")) 
	//    continue;
	printedId.Remove(0,printedId.First('"')+1);
	TString path = printedId(0,printedId.First('"'));
	printedId.Remove(0,printedId.First('[')+1);
	TString fRun = printedId(0,printedId.First(','));
	printedId.Remove(0,printedId.First(',')+1);
	Int_t firstRun = fRun.Atoi();
	TString lRun = printedId(0,printedId.First(']'));
	Int_t lastRun = lRun.Atoi();
	printedId.Remove(0,printedId.First(':')+1);
	printedId.Remove(0,2);
	printedId.Remove(printedId.First('_'),printedId.Length());
	Int_t version = printedId.Atoi();
	IlcCDBId id(path.Data(),firstRun,lastRun,version);
	id.Print();
	IlcCDBEntry *e = cdb->Get(id,kTRUE);
    }

    cdb->DumpToSnapshotFile(snapshotFile,kFALSE);
}
