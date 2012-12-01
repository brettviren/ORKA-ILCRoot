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
/* $Id: IlcPVBARv0.cxx 47780 2011-02-24 15:37:59Z kharlov $ */

/* History of cvs commits:
 *
 * $Log$
 * Revision 1.94  2007/10/18 08:40:02  kharlov
 * Misalignment-related bug fixed
 *
 * Revision 1.93  2007/10/06 22:24:40  kharlov
 * Bug in strip unit geometry is corrected
 *
 * Revision 1.92  2007/07/04 16:38:19  policheh
 * Tracking2LocalCS matrices corrected for CPV.
 *
 * Revision 1.91  2007/07/02 14:50:49  policheh
 * Tracking2LocalCS matrices corrected.
 *
 * Revision 1.90  2007/05/24 13:04:05  policheh
 * AddAlignableVolumes: local to tracking CS transformation matrices creates for each
 * PVBAR supermodule
 *
 * Revision 1.89  2007/04/24 14:34:39  hristov
 * Additional protection: do not search for alignable object if the CPV is not in the geometry
 *
 * Revision 1.88  2007/04/19 15:28:30  kharlov
 * Modify strip unit geometry according to the final drawings (Timur)
 *
 * Revision 1.87  2007/04/01 07:37:10  kharlov
 * TGeo RS to Local RS transf matr added
 *
 * Revision 1.86  2007/03/06 06:55:46  kharlov
 * DP:Misalignment of CPV added
 *
 * Revision 1.85  2007/03/01 11:37:37  kharlov
 * Strip units changed from 8x1 to 8x2 (T.Pocheptsov)
 *
 * Revision 1.84  2006/12/20 16:56:43  kharlov
 * Optional geometry without CPV
 *
 * Revision 1.83  2006/11/14 17:11:15  hristov
 * Removing inheritances from TAttLine, TAttMarker and IlcRndm in IlcModule. The copy constructor and assignment operators are moved to the private part of the class and not implemented. The corresponding changes are propagated to the detectors
 *
 * Revision 1.82  2006/09/27 19:55:57  kharlov
 * Alignment object with symbolic volume names are introduced
 *
 * Revision 1.81  2006/03/04 20:25:56  kharlov
 * Set geom parameters from CDB
 *
 * Revision 1.80  2005/06/17 07:39:07  hristov
 * Removing GetDebug and SetDebug from IlcRun and IlcModule. Using IlcLog for the messages
 *
 * Revision 1.79  2005/05/28 14:19:05  schutz
 * Compilation warnings fixed by T.P.
 *
 */

//_________________________________________________________________________
// Implementation version v0 of PVBAR Manager class 
// An object of this class does not produce hits nor digits
// It is the one to use if you do not want to produce outputs in TREEH or TREED
//                  
//*-- Author: Yves Schutz (SUBATECH) & Dmitri Peressounko (RRC KI & SUBATECH)


// --- ROOT system ---

#include <TFolder.h>
#include <TGeometry.h>
#include <TROOT.h>
#include <TRandom.h>
#include <TTree.h>
#include <TVirtualMC.h>
#include <TGeoPhysicalNode.h>
#include <TGeoManager.h>
#include <TGeoMatrix.h>
#include <TVector3.h>
#include <TGeoTube.h>
#include <TGeoPgon.h>
#include <TBRIK.h>
#include <TTUBS.h>
#include <TNode.h>

// --- Standard library ---

#include <string.h>
#include <stdlib.h>

// --- IlcRoot header files ---

#include "IlcConst.h"
#include "IlcPVBARGeometry.h"
#include "IlcPVBARLoader.h"
#include "IlcPVBARv0.h"
#include "IlcRun.h"
#include "IlcLog.h"
#include "IlcGeomManager.h"

ClassImp(IlcPVBARv0)

//____________________________________________________________________________
IlcPVBARv0::IlcPVBARv0(const char *name, const char *title):
  IlcPVBAR(name,title)
{
  // ctor : title is used to identify the layout
  GetGeometry() ; 
}


//____________________________________________________________________________
TList* IlcPVBARv0::BuildGeometry2()
{
  // Build the PVBAR geometry for the ROOT display

  TList *Nodes = new TList();       //List of geometry nodes

  new TMaterial("void","Vacuum",0,0,0);  //Everything is void
  TBRIK *brik = new TBRIK("S_ilc","ilc volume","void",2000,2000,3000);
  brik->SetVisibility(0);
  TNode *top = new TNode("ilc","ilc","S_ilc");


  IlcPVBARGeometry * geom = GetGeometry() ; 

  Double_t PVBARRmin = geom->GetPVBARRmin();
  Double_t PVBARRmax = geom->GetPVBARRmax();
  Double_t PVBARLength = geom->GetPVBARLength();
  Int_t    PVBARNSectorPhi = 48;
  
  Double_t PVBARparam[3] = {PVBARRmin, PVBARRmax, PVBARLength/2.};

  TTUBS *tubs = new TTUBS("sPVBAR","sPVBAR","void",PVBARparam[0],PVBARparam[1],PVBARparam[2], 0., 360.);
  tubs->SetNumberOfDivisions(PVBARNSectorPhi);
  top->cd();
  TNode *nNode = new TNode("PVBAR","PVBAR","sPVBAR",0.,0.,0.,"");
  nNode->SetLineColor(4);
  Nodes->Add(nNode);

  return Nodes;
}



//____________________________________________________________________________
void IlcPVBARv0::CreateGeometry()
{
  // Create the PVBAR geometry for Geant

  IlcPVBARv0 *PVBARtmp = dynamic_cast<IlcPVBARv0*>(gIlc->GetModule("PVBAR")) ;

  if ( PVBARtmp == NULL ) {
    
    fprintf(stderr, "PVBAR detector not found!\n") ;
    return;
    
  }

  
  // Create a PVBAR module.

  //Media
  TGeoMedium *pMedAir=gGeoManager->GetMedium("PVBAR_Air");
  TGeoMedium *pMedSF57HHT=gGeoManager->GetMedium("PVBAR_SF57HHT");
//   TGeoMedium *pMedScint=gGeoManager->GetMedium("PVBAR_Scint");
  TGeoMedium *pMedKevlar=gGeoManager->GetMedium("PVBAR_Kevlar");
  Int_t SF57Id=gGeoManager->GetMedium("PVBAR_SF57HHT")->GetId();
  Int_t ScintId=gGeoManager->GetMedium("PVBAR_Scint")->GetId();
//   Int_t KevlarId=gGeoManager->GetMedium("PVBAR_Kevlar")->GetId();

  
  TGeoVolume *ilc = gGeoManager->GetVolume("ILCM");

  IlcPVBARGeometry * geom = GetGeometry() ; 

  Double_t PVBARRmin = geom->GetPVBARRmin();
  Double_t PVBARRmax = geom->GetPVBARRmax();
  Double_t PVBARLength = geom->GetPVBARLength();
  Double_t PVBARTileScintThickness = geom->GetPVBARTileScintThickness();
  Double_t PVBARTileSF57Thickness = geom->GetPVBARTileSF57Thickness();
  Double_t WrapThick = geom->GetWrapThick(); //cm
  Int_t NTilesPerSubSector = geom->GetNTilesPerSubSector();
  Double_t PVBARdR = (PVBARTileScintThickness+PVBARTileSF57Thickness)*NTilesPerSubSector;
  const Int_t PVBARNSubSect = geom->GetPVBARNSubSect();
#ifdef WIN32
  Int_t *nSect = (Int_t *)malloc(PVBARNSubSect);
#else
  Int_t nSect[PVBARNSubSect];
#endif

  for(Int_t idx=0; idx<PVBARNSubSect; idx++) nSect[idx] = geom->GetPVBARNSectorsPhi()[idx];


  TGeoTube *PVBARtube = new TGeoTube(PVBARRmin, PVBARRmax, PVBARLength/2.);
  TGeoVolume *VolPVBARtube = new TGeoVolume("PVBAR_TUBE", PVBARtube, pMedAir);

#ifdef WIN32
  TGeoPgon **PVBARSect = (TGeoPgon **)malloc(PVBARNSubSect);
  TGeoVolume **VolPVBARSect = (TGeoVolume **)malloc(PVBARNSubSect);
  TGeoVolume **VolPVBARSubSect = (TGeoVolume **)malloc(PVBARNSubSect);
#else
  TGeoPgon *PVBARSect[PVBARNSubSect];
  TGeoVolume *VolPVBARSect[PVBARNSubSect];
  TGeoVolume *VolPVBARSubSect[PVBARNSubSect];
#endif


  for (Int_t idx = 0; idx<PVBARNSubSect; idx++){
    PVBARSect[idx] = new TGeoPgon(0., 360., nSect[idx], 2);
    PVBARSect[idx]->DefineSection(0, -PVBARLength/2., PVBARRmin, PVBARRmin+PVBARdR);
    PVBARSect[idx]->DefineSection(1,   PVBARLength/2, PVBARRmin, PVBARRmin+PVBARdR);
    
    VolPVBARSect[idx] = new TGeoVolume(Form("PVBAR_SECT%02d",idx), PVBARSect[idx], pMedKevlar);
    VolPVBARSect[idx]->SetLineColor(2+(idx%5));

    VolPVBARSubSect[idx] = PVBARSect[idx]->Divide(VolPVBARSect[idx], Form("PVBAR_SUBSECT%02d",idx), 2, nSect[idx], 0., 360./nSect[idx]);
    VolPVBARSubSect[idx]->SetMedium(pMedSF57HHT);
    VolPVBARSubSect[idx]->SetLineColor(2+(idx%5));
    
    gGeoManager->Matrix(1,90.,90.,0.,0.,90.,0.);
    TString PVBARSciTileName("PVBAR_BSCITILE");
    TString PVBARCerTileName("PVBAR_BCERTILE");
    Double_t PVBARSciTileParam[4];
    Double_t PVBARCerTileParam[4];
    TGeoVolume *VolPVBARSciTile = gGeoManager->Volume(PVBARSciTileName.Data(), "trd1", ScintId, PVBARSciTileParam, 0);
    VolPVBARSciTile->SetLineColor(5);
    TGeoVolume *VolPVBARCerTile = gGeoManager->Volume(PVBARCerTileName.Data(), "trd1", SF57Id, PVBARCerTileParam, 0);
    VolPVBARCerTile->SetLineColor(4);
    for(Int_t idx2=0; idx2<NTilesPerSubSector; idx2++){

      PVBARCerTileParam[0] = (PVBARRmin+idx2*(PVBARTileSF57Thickness+PVBARTileScintThickness))*TMath::Tan(360./nSect[idx]/2.*TMath::DegToRad()) - WrapThick;
      PVBARCerTileParam[1] = (PVBARRmin+PVBARTileSF57Thickness+idx2*(PVBARTileSF57Thickness+PVBARTileScintThickness))*TMath::Tan(360./nSect[idx]/2.*TMath::DegToRad()) - WrapThick;
      PVBARCerTileParam[2] = PVBARLength/2.;
      PVBARCerTileParam[3] = PVBARTileSF57Thickness/2.;
      gGeoManager->Node(VolPVBARCerTile->GetName(), idx2+1, VolPVBARSubSect[idx]->GetName(), PVBARRmin+PVBARTileSF57Thickness/2.+idx2*(PVBARTileSF57Thickness+PVBARTileScintThickness), 0., 0., 1, 1, PVBARCerTileParam, 4);

      PVBARSciTileParam[0] = (PVBARRmin+PVBARTileSF57Thickness+idx2*(PVBARTileSF57Thickness+PVBARTileScintThickness))*TMath::Tan(360./nSect[idx]/2.*TMath::DegToRad()) - WrapThick;
      PVBARSciTileParam[1] = (PVBARRmin+PVBARTileSF57Thickness+PVBARTileScintThickness+idx2*(PVBARTileSF57Thickness+PVBARTileScintThickness))*TMath::Tan(360./nSect[idx]/2.*TMath::DegToRad()) - WrapThick;
      PVBARSciTileParam[2] = PVBARLength/2.;
      PVBARSciTileParam[3] = PVBARTileScintThickness/2.;
      gGeoManager->Node(VolPVBARSciTile->GetName(), idx2+1, VolPVBARSubSect[idx]->GetName(), PVBARRmin+PVBARTileSF57Thickness+PVBARTileScintThickness/2.+idx2*(PVBARTileSF57Thickness+PVBARTileScintThickness), 0., 0., 1, 1, PVBARSciTileParam, 4);

    }

    TGeoRotation * rot = new TGeoRotation ("rot", 0., 0., idx*0.05);
    rot->RegisterYourself();

    VolPVBARtube->AddNode(VolPVBARSect[idx], idx+1, rot);

    PVBARRmin = (PVBARRmin+PVBARdR)/TMath::Cos(360./nSect[idx]*TMath::DegToRad()/2.);
    
  }

  ilc->AddNode(VolPVBARtube,1);


#ifdef WIN32
	if (!nSect) delete [] nSect;
	if (!PVBARSect) delete [] PVBARSect;
	if (!VolPVBARSect) delete [] VolPVBARSect;
	//free (nSect);		//it crashes ilcroot
	//free (PVBARSect);
	//free (VolPVBARSect);
	//free (VolPVBARSubSect);
#endif

}

//_____________________________________________________________________________
void IlcPVBARv0::AddAlignableVolumes() const
{

  return; //FIXME

  //
  // Create entries for alignable volumes associating the symbolic volume
  // name with the corresponding volume path. Needs to be syncronized with
  // eventual changes in the geometry
  // Alignable volumes are:
  // 1) PVBAR modules as a whole
  // 2) Cradle
  // 3) Cradle wheels
  // 4) Strip units (group of 2x8 crystals)

  TString volpath, symname;

  // Alignable modules
  // Volume path /ILCM_1/PVBAR_<i> => symbolic name /PVBAR/Module<i>, <i>=1,2,3,4,5
  
  IlcGeomManager::ELayerID idPVBAR1 = IlcGeomManager::kPVBAR1;
  IlcGeomManager::ELayerID idPVBAR2 = IlcGeomManager::kPVBAR2;
  Int_t modUID, modnum = 0;
  TString physModulePath="/ILCM_1/PVBAR_";
  TString symbModuleName="PVBAR/Module";
  Int_t nModules = GetGeometry()->GetNModules();
  
  char im[5] ;
  for(Int_t iModule=1; iModule<=nModules; iModule++){
    snprintf(im,5,"%d",iModule) ;
    modUID = IlcGeomManager::LayerToVolUID(idPVBAR1,modnum++);
    if(strstr(GetTitle(),im)==0 && strcmp(GetTitle(),"ORKA")!=0 && strcmp(GetTitle(),"noCPV")!=0)
      continue ;
    volpath = physModulePath;
    volpath += iModule;
    //    volpath += "/PEMC_1/PCOL_1/PTIO_1/PCOR_1/PAGA_1/PTII_1";
 
   // Check the volume path if not all 5 modules exist
    if (!gGeoManager->CheckPath(volpath.Data())) {                                                                                         
      IlcError(Form("Volume path %s not valid!",volpath.Data()));                                                                          
      continue;                                                                                                                            
    }                                                                                                                                      
 
    symname = symbModuleName;
    symname += iModule;
    if(!gGeoManager->SetAlignableEntry(symname.Data(),volpath.Data(),modUID))
      continue ;
//      IlcFatal(Form("Alignable entry %s not created. Volume path %s not valid", symname.Data(),volpath.Data()));

    // Creates the Tracking to Local transformation matrix for PVBAR modules
//     TGeoPNEntry *alignableEntry = gGeoManager->GetAlignableEntryByUID(modUID) ;

//     Float_t angle = GetGeometry()->GetPVBARAngle(iModule);
//     TGeoHMatrix* globMatrix = alignableEntry->GetGlobalOrig();
// 
//     TGeoHMatrix *matTtoL = new TGeoHMatrix;
//     matTtoL->RotateZ(-90.+angle);
//     matTtoL->MultiplyLeft(&(globMatrix->Inverse()));
//     alignableEntry->SetMatrix(matTtoL);
  }

  //Aligning of CPV should be done for volume PCPV_1
  symbModuleName="PVBAR/Module";
  modnum=0;
  for(Int_t iModule=1; iModule<=nModules; iModule++){
    if(strstr(GetTitle(),"noCPV"))
      continue ;
    snprintf(im,5,"%d",iModule) ;
    modUID = IlcGeomManager::LayerToVolUID(idPVBAR2,modnum++);
    if(strstr(GetTitle(),im)==0 && strcmp(GetTitle(),"ORKA")!=0)
      continue ;
    volpath = physModulePath;
    volpath += iModule;
    volpath += "/PCPV_1";
    // Check the volume path
    if (!gGeoManager->CheckPath(volpath.Data())) {
      IlcError(Form("Volume path %s not valid!",volpath.Data()));
      continue;
    }

    symname = symbModuleName;
    symname += iModule;
    symname += "/CPV";
    if(!gGeoManager->SetAlignableEntry(symname.Data(),volpath.Data(),modUID))
      IlcFatal(Form("Alignable entry %s not created. Volume path %s not valid", symname.Data(),volpath.Data()));
          
    // Creates the TGeo Local to Tracking transformation matrix ...
//     TGeoPNEntry *alignableEntry = gGeoManager->GetAlignableEntryByUID(modUID) ;

//     Float_t angle = GetGeometry()->GetPVBARAngle(iModule);
//     TGeoHMatrix* globMatrix = alignableEntry->GetGlobalOrig();
// 
//     TGeoHMatrix *matTtoL = new TGeoHMatrix;
//     matTtoL->RotateZ(-90.+angle);
//     matTtoL->MultiplyLeft(&(globMatrix->Inverse()));
//     alignableEntry->SetMatrix(matTtoL);
    
  }
 

  // Alignable cradle walls
  // Volume path /ILCM_1/PCRA_<i> => symbolic name /PVBAR/Cradle<i>, <i>=0,1

  TString physCradlePath="/ILCM_1/PCRA_";
  TString symbCradleName="PVBAR/Cradle";
  Int_t nCradles = 2;

  for(Int_t iCradle=0; iCradle<nCradles; iCradle++){
    volpath = physCradlePath;
    volpath += iCradle;
    symname = symbCradleName;
    symname += iCradle;
    gGeoManager->SetAlignableEntry(symname.Data(),volpath.Data());
  }

  // Alignable wheels
  // Volume path /ILCM_1/PWHE_<i> => symbolic name /PVBAR/Wheel<i>, i=0,1,2,3

  TString physWheelPath="/ILCM_1/PWHE_";
  TString symbWheelName="PVBAR/Wheel";
  Int_t nWheels = 4;

  for(Int_t iWheel=0; iWheel<nWheels; iWheel++){
    volpath = physWheelPath;
    volpath += iWheel;
    symname = symbWheelName;
    symname += iWheel;
    gGeoManager->SetAlignableEntry(symname.Data(),volpath.Data());
  }

  //Physical strip path is a combination of: physModulePath + module number + 
  //physStripPath + strip number == ILCM_1/PVBAR_N/..../PSTR_M
  const Int_t nStripsX = GetGeometry()->GetEMCAGeometry()->GetNStripX();
  const Int_t nStripsZ = GetGeometry()->GetEMCAGeometry()->GetNStripZ();
  TString partialPhysStripName(100);
  TString fullPhysStripName(100);
  TString partialSymbStripName(100);
  TString fullSymbStripName(100);

  for(Int_t module = 1; module <= nModules; ++module){

    snprintf(im,5,"%d",module) ;
    if(strstr(GetTitle(),im)==0 && strcmp(GetTitle(),"ORKA")!=0 && strcmp(GetTitle(),"noCPV")!=0)
      continue ;

    volpath = physModulePath;
    volpath += module;
    // Check the volume path if not all 5 modules exist
    if (!gGeoManager->CheckPath(volpath.Data())) {
      IlcError(Form("Volume path %s does not exist",volpath.Data())) ;
      continue;
    }

    partialPhysStripName  = physModulePath;
    partialPhysStripName += module;
    partialPhysStripName += "/PEMC_1/PCOL_1/PTIO_1/PCOR_1/PAGA_1/PTII_1/PSTR_";

    partialSymbStripName  = symbModuleName;
    partialSymbStripName += module;
    partialSymbStripName += "/Strip_";

    for(Int_t i = 0, ind1D = 1; i < nStripsX; ++i){//ind1D starts from 1 (PSTR_1...PSTR_224...)
      for(Int_t j = 0; j < nStripsZ; ++j, ++ind1D){
         fullPhysStripName = partialPhysStripName;
         fullPhysStripName += ind1D;
         
         fullSymbStripName  = partialSymbStripName;
         fullSymbStripName += i;//ind1D;
         fullSymbStripName += '_';
         fullSymbStripName += j;

         gGeoManager->SetAlignableEntry(fullSymbStripName.Data(), fullPhysStripName.Data());

         // Creates the TGeo Local to Tracking transformation matrix ...
         TGeoPNEntry *alignableEntry = gGeoManager->GetAlignableEntry(fullSymbStripName.Data()) ;
         const char *path = alignableEntry->GetTitle();
         if (!gGeoManager->cd(path))
           IlcFatal(Form("Volume path %s not valid!",path));
         TGeoHMatrix matLtoT = *gGeoManager->GetCurrentMatrix() ;
         Double_t refl[3]={-1.,-1.,-1.} ;
         matLtoT.SetScale(refl) ;
         TGeoHMatrix *matTtoL = new TGeoHMatrix(matLtoT.Inverse());
 
         char PVBARPath[50] ;
         snprintf(PVBARPath,50,"/ILCM_1/PVBAR_%d",module) ;
         if (!gGeoManager->cd(PVBARPath)){
            IlcFatal("Geo manager can not find path \n");
         }
         TGeoHMatrix *mPVBAR = gGeoManager->GetCurrentMatrix();
         if (mPVBAR) 
           matTtoL->Multiply(mPVBAR);
         else{
           IlcFatal("Geo matrixes are not loaded \n") ;
         }
         //Switch y<->z
         Double_t rot[9]={1.,0.,0.,  0.,1.,0., 0.,0.,1.} ;
         matTtoL->SetRotation(rot) ;
         alignableEntry->SetMatrix(matTtoL);

/*
  //Check poisition of corner cell of the strip
  IlcPVBARGeometry * geom = IlcPVBARGeometry::GetInstance() ;
  Int_t relid[4] ; 
  relid[0] = module ;
  relid[1] = 0 ;
  Int_t iStrip=ind1D ;
  Int_t icell=1 ;
  Int_t raw = geom->GetEMCAGeometry()->GetNCellsXInStrip()*((iStrip-1)/geom->GetEMCAGeometry()->GetNStripZ()) +
                1 + (icell-1)/geom->GetEMCAGeometry()->GetNCellsZInStrip() ;
  Int_t col = geom->GetEMCAGeometry()->GetNCellsZInStrip()*(1+(iStrip-1)%geom->GetEMCAGeometry()->GetNStripZ()) - 
                (icell-1)%geom->GetEMCAGeometry()->GetNCellsZInStrip() ;
  if(col==0) col=geom->GetNZ() ;
  relid[2] = raw ;
  relid[3] = col ;
  Float_t xG,zG ; 
  geom->RelPosInModule(relid, xG, zG) ;
printf("============\n") ;
printf("Geometry: x=%f, z=%f \n",xG,zG) ;
  Int_t absid ; 
  geom->RelToAbsNumbering(relid,absid) ;
  Double_t pos[3]= {-2.2*3.5,0.0,1.1}; //Position incide the strip (Y coordinalte is not important)
  Double_t posC[3]={0.0,0.0,0.}; //Global position
 
  matTtoL->MasterToLocal(pos,posC);
printf("Matrix:   x=%f, z=%f, y=%f \n",posC[0],posC[2],posC[1]) ;
*/
      }
    }
  }
}

//____________________________________________________________________________
Float_t IlcPVBARv0::ZMin(void) const
{
  // Overall dimension of the PVBAR (min)

  IlcPVBARGeometry * geom = GetGeometry() ; 

  return -geom->GetOuterBoxSize(2)/2.;
}

//____________________________________________________________________________
Float_t IlcPVBARv0::ZMax(void) const
{
  // Overall dimension of the PVBAR (max)

  IlcPVBARGeometry * geom = GetGeometry() ; 

  return  geom->GetOuterBoxSize(2)/2.;
}

//____________________________________________________________________________
void IlcPVBARv0::Init(void)
{
  // Just prints an information message
  
  Int_t i;

  if(IlcLog::GetGlobalDebugLevel()>0) {
    TString st ; 
    for(i=0;i<35;i++) 
      st += "*";
    Info("Init", "%s", st.Data()) ;  
    // Here the PVBAR initialisation code (if any!)
    
    IlcPVBARGeometry * geom = GetGeometry() ; 

    if (geom!=0)  
      Info("Init", "IlcPVBAR%s: PVBAR geometry intialized for %s", Version().Data(), geom->GetName()) ;
    else
      Info("Init", "IlcPVBAR%s: PVBAR geometry initialization failed !", Version().Data()) ;       

    Info("Init", "%s", st.Data()) ;  
  }
}
