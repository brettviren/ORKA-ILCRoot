#include "ARVersion.h"
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "IlcCDBManager.h"
#include "IlcCDBStorage.h"
#include "IlcCDBId.h"
#include "IlcCDBMetaData.h"
#include "IlcGeomManager.h"
#include "IlcMC.h"
#include <TROOT.h>
#include "IlcRun.h"
#include <TGeoManager.h>
#include <TString.h>
#include <TInterpreter.h>
#endif

void UpdateCDBIdealGeom(const char* cdbUri, const char* cfgFile){
	// Produce the ideal geometry and store it in the specified CDB
	// The second argument allows to specify the config file to be used
	// in particular for giving the choice to generate either a full or
	// a partial geometry.
	//

	IlcCDBManager* cdb = IlcCDBManager::Instance();
	// we set the default storage to the repository because some dets require
	// already at the time of geometry creation to find calibration objects in the cdb
	IlcCDBStorage* storage = 0;
	if(!cdb->IsDefaultStorageSet()) cdb->SetDefaultStorage("local://$ILC_ROOT/OCDB");
	storage = cdb->GetStorage(cdbUri);
	if(!storage) 
	{
		Printf("unable to create valid storage from: %s", cdbUri);
		return;
	}
	cdb->SetRun(0);
	IlcCDBId id("GRP/Geometry/Data",0,IlcCDBRunRange::Infinity());
	IlcCDBMetaData *md= new IlcCDBMetaData();

	// Get root and IlcRoot versions
	const char* rootv = gROOT->GetVersion();
	TString av(ILCROOT_SVN_BRANCH);
	Int_t revnum = ILCROOT_SVN_REVISION;

	Printf("root version: %s.  IlcRoot %s, revision number %d",rootv,av.Data(),revnum);

	md->SetIlcRootVersion(av.Data());
	md->SetComment(Form("Geometry produced with root version %s and IlcRoot %s, revision number %d",rootv,av.Data(),revnum));
	md->AddDateToComment();

	//gSystem->Exec("if [ -e geometry.root ]; then \necho deleting existing geometry.root \nrm -rf geometry.root \nfi");
	if(!gSystem->AccessPathName("geometry.root")){
		Printf("Deleting existing \"geometry.root\"");
		gSystem->Exec("rm -rf geometry.root");
	}

	gROOT->LoadMacro(cfgFile);
	gInterpreter->ProcessLine(gIlc->GetConfigFunction());

	gIlc->GetMCApp()->Init();

	if(!gGeoManager){
		Printf("Unable to produce a valid geometry to be put in the CDB!");
		return;
	}

	/*
	if (gSystem->Exec("if [ ! -e geometry.root ]; then \n return 1  \nfi")) {
		Printf("Did not find freshly written geometry.root file");
		return;
	} */
	if(gSystem->AccessPathName("geometry.root")){
		Printf("Did not find freshly written \"geometry.root\" file. Exiting ...");
		return;
	}

	Printf("Reloading freshly written geometry.root file");
	if (TGeoManager::IsLocked()) TGeoManager::UnlockGeometry();
	IlcGeomManager::LoadGeometry("geometry.root");

	gGeoManager->DefaultColors(); // assign default colors according to Z of material
	// (many colors turned into dark gray nuances some time ago, when the root palette was changed)

	Printf("Storing in CDB geometry produced with root version %s and IlcRoot version %s",rootv,av.Data());
	storage->Put(gGeoManager,id,md);
	// This is to allow macros lauched after this one in the same session to find the
	// newly produced geometry.
	storage->QueryCDB(cdb->GetRun());

}


