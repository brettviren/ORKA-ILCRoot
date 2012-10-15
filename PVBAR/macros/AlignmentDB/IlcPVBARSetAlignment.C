/* $Id: IlcPVBARSetAlignment.C 30923 2009-02-09 15:47:08Z hristov $*/

// Script to create alignment parameters and store them into CDB
// Three sets of alignment parameters can be created:
// 1) Ideal geometry
// 2) Geometry with disalignments and disorientations
// 3) Geometry small disalignments and disorientations

#if !defined(__CINT__)
#include "TControlBar.h"
#include "TString.h"
#include "TRandom.h"
#include "TClonesArray.h"

#include "IlcAlignObjParams.h"
#include "IlcCDBMetaData.h"
#include "IlcCDBId.h"
#include "IlcCDBEntry.h"
#include "IlcCDBManager.h"
#include "IlcCDBStorage.h"
#endif


void IlcPVBARSetAlignment()
{
  TControlBar *menu = new TControlBar("vertical","PVBAR alignment control");
  menu->AddButton("Help to run PVBAR alignment control","Help()",
		  "Explains how to use PVBAR alignment control menus");

  menu->AddButton("Ideal geometry","IdealAlignment()",
		  "Set ideal PVBAR geometry with zero displacement");
  menu->AddButton("Misaligned geometry","FullMisalignment()",
		  "Set PVBAR geometry with large displacement");
  menu->AddButton("Residual misaligned geometry","ResidualAlignment()",
		  "Set PVBAR geometry with small residual displacement");

  menu->Show();
}

//------------------------------------------------------------------------
void Help()
{
  char *string =
    "\n\n\nSet PVBAR alignment parameters and write them into ILC CDB.
Press button \"Ideal geometry\" to create PVBAR geometry with ideal geometry.
Press button \"Misaligned geometry\" to create PVBAR geometry with fully displaced and disorientated geometry.
Press button \"Residual misaligned geometry\" to create PVBAR geometry with infinitesimal displacement and disorientation\n\n\n";
  printf(string);
}

//------------------------------------------------------------------------
void IdealAlignment()
{
  // Create alignment objects for PVBAR with ideally aligned geometry,
  // i.e. with zero displacements and zero disorientations

  // *************************    1st step    ***************
  // Create TClonesArray of alignment objects for PVBAR

  TClonesArray *array = new TClonesArray("IlcAlignObjParams",11);
  TClonesArray &alobj = *array;
   
  IlcAlignObjParams a;

  Double_t dx=0., dy=0., dz=0., dpsi=0., dtheta=0., dphi=0.;
  // null shifts and rotations

  UShort_t iIndex=0;
  IlcGeomManager::ELayerID iLayer = IlcGeomManager::kInvalidLayer;
  UShort_t dvoluid = IlcGeomManager::LayerToVolUID(iLayer,iIndex); //dummy volume identity 

  TString basePath = "PVBAR/Module"; 
  const Int_t nModules=5;

  for (Int_t iModule = 1; iModule<=nModules; iModule++) {
    TString newPath = basePath;
    newPath += iModule;
    new(alobj[iModule-1]) IlcAlignObjParams(newPath.Data(),
					    dvoluid, dx, dy, dz, dpsi, dtheta, dphi, kTRUE);
  }

  // *************************    2nd step    ***************
  // Make CDB storage and put TClonesArray in
  // 
  IlcCDBManager *CDB = IlcCDBManager::Instance();
  CDB->SetDefaultStorage("local://$ILC_ROOT/OCDB");
  
  IlcCDBMetaData *md= new IlcCDBMetaData();
  md->SetResponsible("Yuri Kharlov");
  md->SetComment("Alignment objects for ideal geometry, i.e. applying them to TGeo has to leave geometry unchanged");
  IlcCDBId id("PVBAR/Align/Data",0,999999);
  CDB->Put(array,id, md);
}

//------------------------------------------------------------------------
void ResidualAlignment()
{
  // Create alignment objects for PVBAR with residual alignment,
  // i.e. with infinitesimal displacement and disorientation

  // *************************    1st step    ***************
  // Create TClonesArray of alignment objects for PVBAR

  TClonesArray *array = new TClonesArray("IlcAlignObjParams",11);
  TClonesArray &alobj = *array;
   
  IlcAlignObjParams a;

  Double_t dpsi=0., dtheta=0., dphi=0.;
  Double_t displacement = 0.2;

  UShort_t iIndex=0;
  IlcGeomManager::ELayerID iLayer = IlcGeomManager::kInvalidLayer;
  UShort_t dvoluid = IlcGeomManager::LayerToVolUID(iLayer,iIndex); //dummy volume identity 

  // Alignment for 5 PVBAR modules
  new(alobj[0]) IlcAlignObjParams("PVBAR/Module1",
				  dvoluid, -0.20, -0.1, +0.0, dpsi, dtheta, 0.2, kTRUE);
  new(alobj[1]) IlcAlignObjParams("PVBAR/Module2",
				  dvoluid, -0.10, +0.0, -0.2, dpsi, dtheta, 0.2, kTRUE);
  new(alobj[2]) IlcAlignObjParams("PVBAR/Module3",
				  dvoluid,  0.05, -0.1,  0.2, dpsi, dtheta, 0.0, kTRUE);
  new(alobj[3]) IlcAlignObjParams("PVBAR/Module4",
				  dvoluid, +0.10, -0.0, -0.1, dpsi, dtheta, 0.1, kTRUE);
  new(alobj[4]) IlcAlignObjParams("PVBAR/Module5",
				  dvoluid, +0.20, -0.1,  0.1, dpsi, dtheta, 0.2, kTRUE);

  // Alignment for PVBAR cradle
  new(alobj[5]) IlcAlignObjParams("PVBAR/Cradle0",
				  dvoluid, 0., 0., -displacement, dpsi, dtheta, dphi, kTRUE);
  new(alobj[6]) IlcAlignObjParams("PVBAR/Cradle1",
				  dvoluid, 0., 0., +displacement, dpsi, dtheta, dphi, kTRUE);

  // Alignment for cradle wheels
  new(alobj[7])  IlcAlignObjParams("PVBAR/Wheel0",
				   dvoluid, 0., 0., -displacement, dpsi, dtheta, dphi, kTRUE);
  new(alobj[8])  IlcAlignObjParams("PVBAR/Wheel1",
				   dvoluid, 0., 0., -displacement, dpsi, dtheta, dphi, kTRUE);
  new(alobj[9])  IlcAlignObjParams("PVBAR/Wheel2",
				   dvoluid, 0., 0., +displacement, dpsi, dtheta, dphi, kTRUE);
  new(alobj[10]) IlcAlignObjParams("PVBAR/Wheel3",
				   dvoluid, 0., 0., +displacement, dpsi, dtheta, dphi, kTRUE);

  // *************************    2nd step    ***************
  // Make CDB storage and put TClonesArray in
  // 
  IlcCDBManager *CDB = IlcCDBManager::Instance();
  CDB->SetDefaultStorage("local://$ILC_ROOT/OCDB");
  
  IlcCDBMetaData *md= new IlcCDBMetaData();
  md->SetResponsible("Yuri Kharlov");
  md->SetComment("Alignment objects for slightly misaligned geometry, i.e. applying them to TGeo has to distirbes geometry very little (resisual misalignment");
  IlcCDBId id("PVBAR/Align/Data",1000000,1999999);
  CDB->Put(array,id, md);
}

//------------------------------------------------------------------------
void FullMisalignment()
{
  // Create alignment objects for PVBAR with fully misaligned geometry

  // *************************    1st step    ***************
  // Create TClonesArray of alignment objects for PVBAR

  TClonesArray *array = new TClonesArray("IlcAlignObjParams",11);
  TClonesArray &alobj = *array;
   
  IlcAlignObjParams a;

  Double_t dpsi=0., dtheta=0., dphi=0.;
  Double_t displacement = 10;

  UShort_t iIndex=0;
  IlcGeomManager::ELayerID iLayer = IlcGeomManager::kInvalidLayer;
  UShort_t dvoluid = IlcGeomManager::LayerToVolUID(iLayer,iIndex); //dummy volume identity 

  // Alignment for 5 PVBAR modules
  new(alobj[0]) IlcAlignObjParams("PVBAR/Module1",
				  dvoluid, -20., -10.,   0., dpsi, dtheta, 5, kTRUE);
  new(alobj[1]) IlcAlignObjParams("PVBAR/Module2",
				  dvoluid, -10.,   0., -10., dpsi, dtheta, 2, kTRUE);
  new(alobj[2]) IlcAlignObjParams("PVBAR/Module3",
				  dvoluid,   5., -10.,  10., dpsi, dtheta, 0, kTRUE);
  new(alobj[3]) IlcAlignObjParams("PVBAR/Module4",
				  dvoluid, +10.,  -0., -10., dpsi, dtheta, 2, kTRUE);
  new(alobj[4]) IlcAlignObjParams("PVBAR/Module5",
				  dvoluid, +20., -10.,   0., dpsi, dtheta, 5, kTRUE);

  // Alignment for PVBAR cradle
  new(alobj[5]) IlcAlignObjParams("PVBAR/Cradle0",
				  dvoluid, 0., 0., -displacement, dpsi, dtheta, dphi, kTRUE);
  new(alobj[6]) IlcAlignObjParams("PVBAR/Cradle1",
				  dvoluid, 0., 0., +displacement, dpsi, dtheta, dphi, kTRUE);

  // Alignment for cradle wheels
  new(alobj[7]) IlcAlignObjParams("PVBAR/Wheel0",
				  dvoluid, 0., 0., -displacement, dpsi, dtheta, dphi, kTRUE);
  new(alobj[8]) IlcAlignObjParams("PVBAR/Wheel1",
				  dvoluid, 0., 0., -displacement, dpsi, dtheta, dphi, kTRUE);
  new(alobj[9]) IlcAlignObjParams("PVBAR/Wheel2",
				  dvoluid, 0., 0., +displacement, dpsi, dtheta, dphi, kTRUE);
  new(alobj[10]) IlcAlignObjParams("PVBAR/Wheel3",
				  dvoluid, 0., 0., +displacement, dpsi, dtheta, dphi, kTRUE);

  // *************************    2nd step    ***************
  // Make CDB storage and put TClonesArray in
  // 
  IlcCDBManager *CDB = IlcCDBManager::Instance();
  CDB->SetDefaultStorage("local://$ILC_ROOT/OCDB");
  
  IlcCDBMetaData *md= new IlcCDBMetaData();
  md->SetResponsible("Yuri Kharlov");
  md->SetComment("Alignment objects for fully misaligned geometry, i.e. applying them to TGeo has to distirbes geometry very much");
  IlcCDBId id("PVBAR/Align/Data",2000000,2999999);
  CDB->Put(array,id, md);
}
