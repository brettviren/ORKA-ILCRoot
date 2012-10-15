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
  //BEGIN_HTML
  /*
   *    <H2>
   *     PVBAR in ILC displayed by root
   *    </H2>
   *    <UL>
   *    <LI> All Views
   *    <P>
   *    <CENTER>
   *    <IMG Align=BOTTOM ALT="All Views" SRC="../images/IlcPVBARv0AllViews.gif">
   *    </CENTER></P></LI>
   *    <LI> Front View
   *    <P>
   *    <CENTER>
   *    <IMG Align=BOTTOM ALT="Front View" SRC="../images/IlcPVBARv0FrontView.gif">
   *    </CENTER></P></LI>
   *     <LI> 3D View 1
   *    <P>
   *    <CENTER>
   *    <IMG Align=BOTTOM ALT="3D View 1" SRC="../images/IlcPVBARv03DView1.gif">
   *    </CENTER></P></LI>
   *    <LI> 3D View 2
   *    <P>
   *    <CENTER>
   *    <IMG Align=BOTTOM ALT="3D View 2" SRC="../images/IlcPVBARv03DView2.gif">
   *    </CENTER></P></LI>
   *    </UL>
   */
  //END_HTML

  //===============================================================
  // --- ILC EMC ---
  return this->BuildGeometryforEMC() ;
  //===============================================================

}

//===============================================================
// --- ILC EMC ---

//____________________________________________________________________________
TList* IlcPVBARv0::BuildGeometryforEMC(void)
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
  Int_t    PVBARNSlicePhi = geom->GetPVBARNSlicePhi();
  
  Double_t PVBARparam[3] = {PVBARRmin, PVBARRmax, PVBARLength/2.};

  TTUBS *tubs = new TTUBS("sPVBAR","sPVBAR","void",PVBARparam[0],PVBARparam[1],PVBARparam[2], 0., 360.);
  tubs->SetNumberOfDivisions(PVBARNSlicePhi);
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

//   IlcPVBARGeometry * geom = GetGeometry() ; 

  // Get pointer to the array containing media indeces
//   Int_t *idtmed = fIdtmed->GetArray() - 699 ;

  // Create a PVBAR module.
  TGeoMedium *Air=gGeoManager->GetMedium("PVBAR_Air          $");
//   TGeoMedium *Scint=gGeoManager->GetMedium("PVBAR_CPV scint.   $");
  Int_t ScintId=gGeoManager->GetMedium("PVBAR_CPV scint.   $")->GetId();
  Int_t LeadGlassId=gGeoManager->GetMedium("PVBAR_LeadGlass")->GetId();
//   TGeoMedium *Scint=gGeoManager->GetMedium("PVBAR_ArCo2      $");
//   TGeoMedium *Scint=gGeoManager->GetMedium("PVBAR_Air          $");

  TGeoVolume *ilc = gGeoManager->GetVolume("ILCM");

  IlcPVBARGeometry * geom = GetGeometry() ; 

  Double_t PVBARRmin = geom->GetPVBARRmin();
  Double_t PVBARRmax = geom->GetPVBARRmax();
  Double_t PVBARLength = geom->GetPVBARLength();
  Int_t PVBARNSlicePhi = geom->GetPVBARNSlicePhi();
  Double_t PVBARTileScintThickness = geom->GetPVBARTileScintThickness();
  Double_t PVBARTileSF57Thickness = geom->GetPVBARTileSF57Thickness();
  Int_t PVBARNTiles = Int_t((PVBARRmax-PVBARRmin)/(PVBARTileScintThickness+PVBARTileSF57Thickness)+0.00001);

  Double_t PVBARparam[3] = {PVBARRmin, PVBARRmax, PVBARLength/2.};

  TGeoTube *PVBARtube = new TGeoTube(PVBARparam[0], PVBARparam[1], PVBARparam[2]);
  TGeoVolume *VolPVBARtube = new TGeoVolume("PVBAR_TUBE", PVBARtube, Air);

  Double_t CellAngleDegPhi=360./PVBARNSlicePhi;
  VolPVBARtube->Divide("PVBAR_BSTRIPPHI", 2, PVBARNSlicePhi, 0., CellAngleDegPhi,LeadGlassId);
  
  TGeoVolume *VolPVBARStripPhi = gGeoManager->GetVolume("PVBAR_BSTRIPPHI");

  TString PVBARTileName("PVBAR_BSCITILES");
  
  Double_t PVBARTileParam[5];
  TGeoVolume *VolPVBARTile = gGeoManager->Volume(PVBARTileName.Data(), "tubs", ScintId, PVBARTileParam, 0);
  for(Int_t idx=0; idx<PVBARNTiles; idx++){
    PVBARTileParam[0] = PVBARRmin+PVBARTileSF57Thickness+idx*(PVBARTileScintThickness+PVBARTileSF57Thickness);
    PVBARTileParam[1] = PVBARTileParam[0]+PVBARTileScintThickness;
    PVBARTileParam[2] = PVBARLength/2.;
    PVBARTileParam[3] = -CellAngleDegPhi/2.;
    PVBARTileParam[4] = CellAngleDegPhi/2.;
    gGeoManager->Node(VolPVBARTile->GetName(), idx+1, VolPVBARStripPhi->GetName(), 0., 0., 0., 0, 1, PVBARTileParam, 5);
  }
  
  ilc->AddNode(VolPVBARtube, 1);


}

//____________________________________________________________________________
void IlcPVBARv0::CreateGeometryforEMC()
{
  // Create the PVBAR-EMC geometry for GEANT
  // Author: Dmitri Peressounko August 2001
  // The used coordinate system: 
  //   1. in Module: X along longer side, Y out of beam, Z along shorter side (along beam)
  //   2. In Strip the same: X along longer side, Y out of beam, Z along shorter side (along beam)


    //BEGIN_HTML
  /*
    <H2>
    Geant3 geometry tree of PVBAR-EMC in ILC
    </H2>
    <P><CENTER>
    <IMG Align=BOTTOM ALT="EMC geant tree" SRC="../images/EMCinIlc.gif"> 
    </CENTER><P>
  */
  //END_HTML  
  
  // Get pointer to the array containing media indexes
  Int_t *idtmed = fIdtmed->GetArray() - 699 ;

  IlcPVBARGeometry * geom = GetGeometry() ; 
  IlcPVBAREMCAGeometry * emcg = geom->GetEMCAGeometry() ;
  Float_t par[4];
  Int_t  ipar;

  // ======= Define the strip ===============

  for (ipar=0; ipar<3; ipar++) par[ipar] = *(emcg->GetStripHalfSize() + ipar);
  gMC->Gsvolu("PSTR", "BOX ", idtmed[716], par, 3) ;  //Made of steel
   
  // --- define steel volume (cell of the strip unit)
  for (ipar=0; ipar<3; ipar++) par[ipar] = *(emcg->GetAirCellHalfSize() + ipar);
  gMC->Gsvolu("PCEL", "BOX ", idtmed[798], par, 3);

  // --- define wrapped crystal and put it into steel cell

  for (ipar=0; ipar<3; ipar++) par[ipar] = *(emcg->GetWrappedHalfSize() + ipar);
  gMC->Gsvolu("PWRA", "BOX ", idtmed[702], par, 3);
  const Float_t * pin    = emcg->GetAPDHalfSize() ; 
  const Float_t * preamp = emcg->GetPreampHalfSize() ;
  Float_t y = (emcg->GetAirGapLed()-2*pin[1]-2*preamp[1])/2;
  gMC->Gspos("PWRA", 1, "PCEL", 0.0, y, 0.0, 0, "ONLY") ;
    
  // --- Define crystal and put it into wrapped crystall ---
  for (ipar=0; ipar<3; ipar++) par[ipar] = *(emcg->GetCrystalHalfSize() + ipar);
  gMC->Gsvolu("PXTL", "BOX ", idtmed[699], par, 3) ;
  gMC->Gspos("PXTL", 1, "PWRA", 0.0, 0.0, 0.0, 0, "ONLY") ;
      
  // --- define APD/PIN preamp and put it into AirCell
 
  for (ipar=0; ipar<3; ipar++) par[ipar] = *(emcg->GetAPDHalfSize() + ipar);
  gMC->Gsvolu("PPIN", "BOX ", idtmed[705], par, 3) ;
  const Float_t * crystal = emcg->GetCrystalHalfSize() ;
  y = crystal[1] + emcg->GetAirGapLed() /2 - preamp[1]; 
  gMC->Gspos("PPIN", 1, "PCEL", 0.0, y, 0.0, 0, "ONLY") ;
  for (ipar=0; ipar<3; ipar++) par[ipar] = *(emcg->GetPreampHalfSize() + ipar);
  gMC->Gsvolu("PREA", "BOX ", idtmed[711], par, 3) ;   // Here I assumed preamp as a printed Circuit
  y = crystal[1] + emcg->GetAirGapLed() /2 + pin[1]  ;    // May it should be changed
  gMC->Gspos("PREA", 1, "PCEL", 0.0, y, 0.0, 0, "ONLY") ; // to ceramics?
  
  
  // --- Fill strip with wrapped cristals in steel cells
  
  const Float_t* splate = emcg->GetSupportPlateHalfSize();  
  y = -splate[1] ;
  const Float_t* acel = emcg->GetAirCellHalfSize() ;
  
  for(Int_t lev = 2, icel = 1; 
      icel <= emcg->GetNCellsXInStrip()*emcg->GetNCellsZInStrip(); 
      icel += 2, lev += 2) {
    Float_t x = (2*(lev / 2) - 1 - emcg->GetNCellsXInStrip())* acel[0] ;
    Float_t z = acel[2];
    gMC->Gspos("PCEL", icel, "PSTR", x, y, +z, 0, "ONLY") ;
    gMC->Gspos("PCEL", icel + 1, "PSTR", x, y, -z, 0, "ONLY") ;
  }

  // --- define the support plate, hole in it and position it in strip ----
  for (ipar=0; ipar<3; ipar++) par[ipar] = *(emcg->GetSupportPlateHalfSize() + ipar);
  gMC->Gsvolu("PSUP", "BOX ", idtmed[701], par, 3) ;
  
  for (ipar=0; ipar<3; ipar++) par[ipar] = *(emcg->GetSupportPlateInHalfSize() + ipar);
  gMC->Gsvolu("PSHO", "BOX ", idtmed[798], par, 3) ;
  Float_t z = emcg->GetSupportPlateThickness()/2 ;
  gMC->Gspos("PSHO", 1, "PSUP", 0.0, 0.0, z, 0, "ONLY") ;

  y = acel[1] ;
  gMC->Gspos("PSUP", 1, "PSTR", 0.0, y, 0.0, 0, "ONLY") ;

  
  // ========== Fill module with strips and put them into inner thermoinsulation=============
  for (ipar=0; ipar<3; ipar++) par[ipar] = *(emcg->GetInnerThermoHalfSize() + ipar);
  gMC->Gsvolu("PTII", "BOX ", idtmed[706], par, 3) ;     
  
  const Float_t * inthermo = emcg->GetInnerThermoHalfSize() ;
  const Float_t * strip    = emcg->GetStripHalfSize() ;
  y = inthermo[1] - strip[1] ;
  Int_t irow;
  Int_t nr = 1 ;
  Int_t icol ;
  
  for(irow = 0; irow < emcg->GetNStripX(); irow ++){
    Float_t x = (2*irow + 1 - emcg->GetNStripX())* strip[0] ;
    for(icol = 0; icol < emcg->GetNStripZ(); icol ++){
      z = (2*icol + 1 - emcg->GetNStripZ()) * strip[2] ;
      gMC->Gspos("PSTR", nr, "PTII", x, y, z, 0, "ONLY") ;
      nr++ ;
    }
  }
  
  
  // ------- define the air gap between thermoinsulation and cooler
  for (ipar=0; ipar<3; ipar++) par[ipar] = *(emcg->GetAirGapHalfSize() + ipar);
  gMC->Gsvolu("PAGA", "BOX ", idtmed[798], par, 3) ;   
  const Float_t * agap = emcg->GetAirGapHalfSize() ;
  y = agap[1] - inthermo[1]  ;
  
  gMC->Gspos("PTII", 1, "PAGA", 0.0, y, 0.0, 0, "ONLY") ;


  // ------- define the Al passive cooler 
  for (ipar=0; ipar<3; ipar++) par[ipar] = *(emcg->GetCoolerHalfSize() + ipar);
  gMC->Gsvolu("PCOR", "BOX ", idtmed[701], par, 3) ;   
  const Float_t * cooler = emcg->GetCoolerHalfSize() ;
  y = cooler[1] - agap[1]  ;
  
  gMC->Gspos("PAGA", 1, "PCOR", 0.0, y, 0.0, 0, "ONLY") ;
  
  // ------- define the outer thermoinsulating cover
  for (ipar=0; ipar<4; ipar++) par[ipar] = *(emcg->GetOuterThermoParams() + ipar);
  gMC->Gsvolu("PTIO", "TRD1", idtmed[706], par, 4) ;        
  const Float_t * outparams = emcg->GetOuterThermoParams() ; 
  
  Int_t idrotm[99] ;
  IlcMatrix(idrotm[1], 90.0, 0.0, 0.0, 0.0, 90.0, 270.0) ;
  // Frame in outer thermoinsulation and so on: z out of beam, y along beam, x across beam
  
  z = outparams[3] - cooler[1] ;
  gMC->Gspos("PCOR", 1, "PTIO", 0., 0.0, z, idrotm[1], "ONLY") ;
  
  // -------- Define the outer Aluminium cover -----
  for (ipar=0; ipar<4; ipar++) par[ipar] = *(emcg->GetAlCoverParams() + ipar);
  gMC->Gsvolu("PCOL", "TRD1", idtmed[701], par, 4) ;        
  const Float_t * covparams = emcg->GetAlCoverParams() ; 
  z = covparams[3] - outparams[3] ;
  gMC->Gspos("PTIO", 1, "PCOL", 0., 0.0, z, 0, "ONLY") ;

  // --------- Define front fiberglass cover -----------
  for (ipar=0; ipar<3; ipar++) par[ipar] = *(emcg->GetFiberGlassHalfSize() + ipar);
  gMC->Gsvolu("PFGC", "BOX ", idtmed[717], par, 3) ;  
  z = - outparams[3] ;
  gMC->Gspos("PFGC", 1, "PCOL", 0., 0.0, z, 0, "ONLY") ;

  //=============This is all with cold section==============
  

  //------ Warm Section --------------
  for (ipar=0; ipar<3; ipar++) par[ipar] = *(emcg->GetWarmAlCoverHalfSize() + ipar);
  gMC->Gsvolu("PWAR", "BOX ", idtmed[701], par, 3) ; 
  const Float_t * warmcov = emcg->GetWarmAlCoverHalfSize() ;
  
  // --- Define the outer thermoinsulation ---
  for (ipar=0; ipar<3; ipar++) par[ipar] = *(emcg->GetWarmThermoHalfSize() + ipar);
  gMC->Gsvolu("PWTI", "BOX ", idtmed[706], par, 3) ; 
  const Float_t * warmthermo = emcg->GetWarmThermoHalfSize() ;
  z = -warmcov[2] + warmthermo[2] ;
  
  gMC->Gspos("PWTI", 1, "PWAR", 0., 0.0, z, 0, "ONLY") ;     
  
  // --- Define cables area and put in it T-supports ---- 
  for (ipar=0; ipar<3; ipar++) par[ipar] = *(emcg->GetTCables1HalfSize() + ipar);
  gMC->Gsvolu("PCA1", "BOX ", idtmed[718], par, 3) ; 
  const Float_t * cbox = emcg->GetTCables1HalfSize() ;
  
  for (ipar=0; ipar<3; ipar++) par[ipar] = *(emcg->GetTSupport1HalfSize() + ipar);
  gMC->Gsvolu("PBE1", "BOX ", idtmed[701], par, 3) ;
  const Float_t * beams = emcg->GetTSupport1HalfSize() ;
  Int_t isup ;
  for(isup = 0; isup < emcg->GetNTSuppots(); isup++){
    Float_t x = -cbox[0] + beams[0] + (2*beams[0]+emcg->GetTSupportDist())*isup ;
    gMC->Gspos("PBE1", isup, "PCA1", x, 0.0, 0.0, 0, "ONLY") ;
  }
  
  z = -warmthermo[2] + cbox[2];
  gMC->Gspos("PCA1", 1, "PWTI", 0.0, 0.0, z, 0, "ONLY") ;     
  
  for (ipar=0; ipar<3; ipar++) par[ipar] = *(emcg->GetTCables2HalfSize() + ipar);
  gMC->Gsvolu("PCA2", "BOX ", idtmed[718], par, 3) ; 
  const Float_t * cbox2 = emcg->GetTCables2HalfSize() ;
  
  for (ipar=0; ipar<3; ipar++) par[ipar] = *(emcg->GetTSupport2HalfSize() + ipar);
  gMC->Gsvolu("PBE2", "BOX ", idtmed[701], par, 3) ;
  for(isup = 0; isup < emcg->GetNTSuppots(); isup++){
    Float_t x = -cbox[0] + beams[0] + (2*beams[0]+emcg->GetTSupportDist())*isup ;
    gMC->Gspos("PBE2", isup, "PCA2", x, 0.0, 0.0, 0, "ONLY") ;
  }
  
  z = -warmthermo[2] + 2*cbox[2] + cbox2[2];
  gMC->Gspos("PCA2", 1, "PWTI", 0.0, 0.0, z, 0, "ONLY") ;     
  
  
  // --- Define frame ---
  for (ipar=0; ipar<3; ipar++) par[ipar] = *(emcg->GetFrameXHalfSize() + ipar);
  gMC->Gsvolu("PFRX", "BOX ", idtmed[716], par, 3) ; 
  const Float_t * posit1 = emcg->GetFrameXPosition() ;
  gMC->Gspos("PFRX", 1, "PWTI", posit1[0],  posit1[1], posit1[2], 0, "ONLY") ;
  gMC->Gspos("PFRX", 2, "PWTI", posit1[0], -posit1[1], posit1[2], 0, "ONLY") ;
  
  for (ipar=0; ipar<3; ipar++) par[ipar] = *(emcg->GetFrameZHalfSize() + ipar);
  gMC->Gsvolu("PFRZ", "BOX ", idtmed[716], par, 3) ; 
  const Float_t * posit2 = emcg->GetFrameZPosition() ;
  gMC->Gspos("PFRZ", 1, "PWTI",  posit2[0], posit2[1], posit2[2], 0, "ONLY") ;
  gMC->Gspos("PFRZ", 2, "PWTI", -posit2[0], posit2[1], posit2[2], 0, "ONLY") ;

 // --- Define Fiber Glass support ---
  for (ipar=0; ipar<3; ipar++) par[ipar] = *(emcg->GetFGupXHalfSize() + ipar);
  gMC->Gsvolu("PFG1", "BOX ", idtmed[717], par, 3) ; 
  const Float_t * posit3 = emcg->GetFGupXPosition() ;
  gMC->Gspos("PFG1", 1, "PWTI", posit3[0],  posit3[1], posit3[2], 0, "ONLY") ;
  gMC->Gspos("PFG1", 2, "PWTI", posit3[0], -posit3[1], posit3[2], 0, "ONLY") ;

  for (ipar=0; ipar<3; ipar++) par[ipar] = *(emcg->GetFGupZHalfSize() + ipar);
  gMC->Gsvolu("PFG2", "BOX ", idtmed[717], par, 3) ; 
  const Float_t * posit4 = emcg->GetFGupZPosition();
  gMC->Gspos("PFG2", 1, "PWTI",  posit4[0], posit4[1], posit4[2], 0, "ONLY") ;
  gMC->Gspos("PFG2", 2, "PWTI", -posit4[0], posit4[1], posit4[2], 0, "ONLY") ;

  for (ipar=0; ipar<3; ipar++) par[ipar] = *(emcg->GetFGlowXHalfSize() + ipar);
  gMC->Gsvolu("PFG3", "BOX ", idtmed[717], par, 3) ; 
  const Float_t * posit5 = emcg->GetFGlowXPosition() ;
  gMC->Gspos("PFG3", 1, "PWTI", posit5[0],  posit5[1], posit5[2], 0, "ONLY") ;
  gMC->Gspos("PFG3", 2, "PWTI", posit5[0], -posit5[1], posit5[2], 0, "ONLY") ;

  for (ipar=0; ipar<3; ipar++) par[ipar] = *(emcg->GetFGlowZHalfSize() + ipar);
  gMC->Gsvolu("PFG4", "BOX ", idtmed[717], par, 3) ; 
  const Float_t * posit6 = emcg->GetFGlowZPosition() ;
  gMC->Gspos("PFG4", 1, "PWTI",  posit6[0], posit6[1], posit6[2], 0, "ONLY") ;
  gMC->Gspos("PFG4", 2, "PWTI", -posit6[0], posit6[1], posit6[2], 0, "ONLY") ;

  // --- Define Air Gap for FEE electronics ----- 
  
  for (ipar=0; ipar<3; ipar++) par[ipar] = *(emcg->GetFEEAirHalfSize() + ipar);
  gMC->Gsvolu("PAFE", "BOX ", idtmed[798], par, 3) ; 
  const Float_t * posit7 = emcg->GetFEEAirPosition() ;
  gMC->Gspos("PAFE", 1, "PWTI",  posit7[0], posit7[1], posit7[2], 0, "ONLY") ;
  
  // Define the EMC module volume and combine Cool and Warm sections
  
  for (ipar=0; ipar<4; ipar++) par[ipar] = *(emcg->GetEMCParams() + ipar);
  gMC->Gsvolu("PEMC", "TRD1", idtmed[798], par, 4) ;        
  z =  - warmcov[2] ;
  gMC->Gspos("PCOL", 1, "PEMC",  0., 0., z, 0, "ONLY") ;
  z = covparams[3] ;
  gMC->Gspos("PWAR", 1, "PEMC",  0., 0., z, 0, "ONLY") ;
  
  
  // Put created EMC geometry into PVBAR volume
  
  z = geom->GetCPVBoxSize(1) / 2. ;
  gMC->Gspos("PEMC", 1, "PVBAR", 0., 0., z, 0, "ONLY") ; 
  
}

//____________________________________________________________________________
void IlcPVBARv0::CreateGeometryforCPV()
{
  // Create the PVBAR-CPV geometry for GEANT
  // Author: Yuri Kharlov 11 September 2000
  //BEGIN_HTML
  /*
    <H2>
    Geant3 geometry of PVBAR-CPV in ILC
    </H2>
    <table width=700>

    <tr>
         <td>CPV perspective view</td>
         <td>CPV front view      </td>
    </tr>

    <tr>
         <td> <img height=300 width=290 src="../images/CPVallPersp.gif"> </td>
         <td> <img height=300 width=290 src="../images/CPVallFront.gif"> </td>
    </tr>

    <tr>
         <td>One CPV module, perspective view                            </td>
         <td>One CPV module, front view (extended in vertical direction) </td>
    </tr>

    <tr>
         <td><img height=300 width=290 src="../images/CPVmodulePers.gif"></td>
         <td><img height=300 width=290 src="../images/CPVmoduleSide.gif"></td>
    </tr>

    </table>

    <H2>
    Geant3 geometry tree of PVBAR-CPV in ILC
    </H2>
    <center>
    <img height=300 width=290 src="../images/CPVtree.gif">
    </center>
  */
  //END_HTML  

  Float_t par[3], x,y,z;

  // Get pointer to the array containing media indexes
  Int_t *idtmed = fIdtmed->GetArray() - 699 ;

  IlcPVBARGeometry * geom = GetGeometry() ; 

  // The box containing all CPV for one PVBAR module filled with air 
  par[0] = geom->GetCPVBoxSize(0) / 2.0 ;  
  par[1] = geom->GetCPVBoxSize(1) / 2.0 ; 
  par[2] = geom->GetCPVBoxSize(2) / 2.0 ;
  gMC->Gsvolu("PCPV", "BOX ", idtmed[798], par, 3) ;

  const Float_t * emcParams = geom->GetEMCAGeometry()->GetEMCParams() ;
  z = - emcParams[3] ;
  Int_t rotm ;
  IlcMatrix(rotm, 90.,0., 0., 0., 90., 90.) ;

  gMC->Gspos("PCPV", 1, "PVBAR", 0.0, 0.0, z, rotm, "ONLY") ; 
  
  // Gassiplex board
  
  par[0] = geom->GetGassiplexChipSize(0)/2.;
  par[1] = geom->GetGassiplexChipSize(1)/2.;
  par[2] = geom->GetGassiplexChipSize(2)/2.;
  gMC->Gsvolu("PCPC","BOX ",idtmed[707],par,3);
  
  // Cu+Ni foil covers Gassiplex board

  par[1] = geom->GetCPVCuNiFoilThickness()/2;
  gMC->Gsvolu("PCPD","BOX ",idtmed[710],par,3);
  y      = -(geom->GetGassiplexChipSize(1)/2 - par[1]);
  gMC->Gspos("PCPD",1,"PCPC",0,y,0,0,"ONLY");

  // Position of the chip inside CPV

  Float_t xStep = geom->GetCPVActiveSize(0) / (geom->GetNumberOfCPVChipsPhi() + 1);
  Float_t zStep = geom->GetCPVActiveSize(1) / (geom->GetNumberOfCPVChipsZ()   + 1);
  Int_t   copy  = 0;
  y = geom->GetCPVFrameSize(1)/2           - geom->GetFTPosition(0) +
    geom->GetCPVTextoliteThickness() / 2 + geom->GetGassiplexChipSize(1) / 2 + 0.1;
  for (Int_t ix=0; ix<geom->GetNumberOfCPVChipsPhi(); ix++) {
    x = xStep * (ix+1) - geom->GetCPVActiveSize(0)/2;
    for (Int_t iz=0; iz<geom->GetNumberOfCPVChipsZ(); iz++) {
      copy++;
      z = zStep * (iz+1) - geom->GetCPVActiveSize(1)/2;
      gMC->Gspos("PCPC",copy,"PCPV",x,y,z,0,"ONLY");
    }
  }

  // Foiled textolite (1 mm of textolite + 50 mkm of Cu + 6 mkm of Ni)
  
  par[0] = geom->GetCPVActiveSize(0)        / 2;
  par[1] = geom->GetCPVTextoliteThickness() / 2;
  par[2] = geom->GetCPVActiveSize(1)        / 2;
  gMC->Gsvolu("PCPF","BOX ",idtmed[707],par,3);

  // Argon gas volume

  par[1] = (geom->GetFTPosition(2) - geom->GetFTPosition(1) - geom->GetCPVTextoliteThickness()) / 2;
  gMC->Gsvolu("PCPG","BOX ",idtmed[715],par,3);

  for (Int_t i=0; i<4; i++) {
    y = geom->GetCPVFrameSize(1) / 2 - geom->GetFTPosition(i) + geom->GetCPVTextoliteThickness()/2;
    gMC->Gspos("PCPF",i+1,"PCPV",0,y,0,0,"ONLY");
    if(i==1){
      y-= (geom->GetFTPosition(2) - geom->GetFTPosition(1)) / 2;
      gMC->Gspos("PCPG",1,"PCPV ",0,y,0,0,"ONLY");
    }
  }

  // Dummy sensitive plane in the middle of argone gas volume

  par[1]=0.001;
  gMC->Gsvolu("PCPQ","BOX ",idtmed[715],par,3);
  gMC->Gspos ("PCPQ",1,"PCPG",0,0,0,0,"ONLY");

  // Cu+Ni foil covers textolite

  par[1] = geom->GetCPVCuNiFoilThickness() / 2;
  gMC->Gsvolu("PCP1","BOX ",idtmed[710],par,3);
  y = geom->GetCPVTextoliteThickness()/2 - par[1];
  gMC->Gspos ("PCP1",1,"PCPF",0,y,0,0,"ONLY");

  // Aluminum frame around CPV

  par[0] = geom->GetCPVFrameSize(0)/2;
  par[1] = geom->GetCPVFrameSize(1)/2;
  par[2] = geom->GetCPVBoxSize(2)  /2;
  gMC->Gsvolu("PCF1","BOX ",idtmed[701],par,3);

  par[0] = geom->GetCPVBoxSize(0)/2 - geom->GetCPVFrameSize(0);
  par[1] = geom->GetCPVFrameSize(1)/2;
  par[2] = geom->GetCPVFrameSize(2)/2;
  gMC->Gsvolu("PCF2","BOX ",idtmed[701],par,3);

  for (Int_t j=0; j<=1; j++) {
    x = TMath::Sign(1,2*j-1) * (geom->GetCPVBoxSize(0) - geom->GetCPVFrameSize(0)) / 2;
    gMC->Gspos("PCF1",j+1,"PCPV", x,0,0,0,"ONLY");
    z = TMath::Sign(1,2*j-1) * (geom->GetCPVBoxSize(2) - geom->GetCPVFrameSize(2)) / 2;
    gMC->Gspos("PCF2",j+1,"PCPV",0, 0,z,0,"ONLY");
  }

}


//____________________________________________________________________________
void IlcPVBARv0::CreateGeometryforSupport()
{
  // Create the PVBAR' support geometry for GEANT
    //BEGIN_HTML
  /*
    <H2>
    Geant3 geometry of the PVBAR's support
    </H2>
    <P><CENTER>
    <IMG Align=BOTTOM ALT="EMC geant tree" SRC="../images/PVBAR_support.gif"> 
    </CENTER><P>
  */
  //END_HTML  
  
  Float_t par[5], x0,y0,z0 ; 
  Int_t   i,j,copy;

  // Get pointer to the array containing media indexes
  Int_t *idtmed = fIdtmed->GetArray() - 699 ;

  IlcPVBARGeometry * geom = GetGeometry() ; 

  // --- Dummy box containing two rails on which PVBAR support moves
  // --- Put these rails to the bottom of the L3 magnet

  par[0] =  geom->GetRailRoadSize(0) / 2.0 ;
  par[1] =  geom->GetRailRoadSize(1) / 2.0 ;
  par[2] =  geom->GetRailRoadSize(2) / 2.0 ;
  gMC->Gsvolu("PRRD", "BOX ", idtmed[798], par, 3) ;

  y0     = -(geom->GetRailsDistanceFromIP() - geom->GetRailRoadSize(1) / 2.0) ;
  gMC->Gspos("PRRD", 1, "ILCM", 0.0, y0, 0.0, 0, "ONLY") ; 

  // --- Dummy box containing one rail

  par[0] =  geom->GetRailOuterSize(0) / 2.0 ;
  par[1] =  geom->GetRailOuterSize(1) / 2.0 ;
  par[2] =  geom->GetRailOuterSize(2) / 2.0 ;
  gMC->Gsvolu("PRAI", "BOX ", idtmed[798], par, 3) ;

  for (i=0; i<2; i++) {
    x0     = (2*i-1) * geom->GetDistanceBetwRails()  / 2.0 ;
    gMC->Gspos("PRAI", i, "PRRD", x0, 0.0, 0.0, 0, "ONLY") ; 
  }

  // --- Upper and bottom steel parts of the rail

  par[0] =  geom->GetRailPart1(0) / 2.0 ;
  par[1] =  geom->GetRailPart1(1) / 2.0 ;
  par[2] =  geom->GetRailPart1(2) / 2.0 ;
  gMC->Gsvolu("PRP1", "BOX ", idtmed[716], par, 3) ;

  y0     = - (geom->GetRailOuterSize(1) - geom->GetRailPart1(1))  / 2.0 ;
  gMC->Gspos("PRP1", 1, "PRAI", 0.0, y0, 0.0, 0, "ONLY") ;
  y0     =   (geom->GetRailOuterSize(1) - geom->GetRailPart1(1))  / 2.0 - geom->GetRailPart3(1);
  gMC->Gspos("PRP1", 2, "PRAI", 0.0, y0, 0.0, 0, "ONLY") ;

  // --- The middle vertical steel parts of the rail

  par[0] =  geom->GetRailPart2(0) / 2.0 ;
  par[1] =  geom->GetRailPart2(1) / 2.0 ;
  par[2] =  geom->GetRailPart2(2) / 2.0 ;
  gMC->Gsvolu("PRP2", "BOX ", idtmed[716], par, 3) ;

  y0     =   - geom->GetRailPart3(1) / 2.0 ;
  gMC->Gspos("PRP2", 1, "PRAI", 0.0, y0, 0.0, 0, "ONLY") ; 

  // --- The most upper steel parts of the rail

  par[0] =  geom->GetRailPart3(0) / 2.0 ;
  par[1] =  geom->GetRailPart3(1) / 2.0 ;
  par[2] =  geom->GetRailPart3(2) / 2.0 ;
  gMC->Gsvolu("PRP3", "BOX ", idtmed[716], par, 3) ;

  y0     =   (geom->GetRailOuterSize(1) - geom->GetRailPart3(1))  / 2.0 ;
  gMC->Gspos("PRP3", 1, "PRAI", 0.0, y0, 0.0, 0, "ONLY") ; 

  // --- The wall of the cradle
  // --- The wall is empty: steel thin walls and air inside

  par[1] =  TMath::Sqrt(TMath::Power((geom->GetIPtoCPVDistance() + geom->GetOuterBoxSize(3)),2) +
			TMath::Power((geom->GetOuterBoxSize(1)/2),2))+10. ;
  par[0] =  par[1] - geom->GetCradleWall(1) ;
  par[2] =  geom->GetCradleWall(2) / 2.0 ;
  par[3] =  geom->GetCradleWall(3) ;
  par[4] =  geom->GetCradleWall(4) ;
  gMC->Gsvolu("PCRA", "TUBS", idtmed[716], par, 5) ;

  par[0] +=  geom->GetCradleWallThickness() ;
  par[1] -=  geom->GetCradleWallThickness() ;
  par[2] -=  geom->GetCradleWallThickness() ;
  gMC->Gsvolu("PCRE", "TUBS", idtmed[798], par, 5) ;
  gMC->Gspos ("PCRE", 1, "PCRA", 0.0, 0.0, 0.0, 0, "ONLY") ; 

  for (i=0; i<2; i++) {
    z0 = (2*i-1) * (geom->GetOuterBoxSize(2) + geom->GetCradleWall(2) )/ 2.0  ;
        gMC->Gspos("PCRA", i, "ILCM", 0.0, 0.0, z0, 0, "ONLY") ; 
  }

  // --- The "wheels" of the cradle
  
  par[0] = geom->GetCradleWheel(0) / 2;
  par[1] = geom->GetCradleWheel(1) / 2;
  par[2] = geom->GetCradleWheel(2) / 2;
  gMC->Gsvolu("PWHE", "BOX ", idtmed[716], par, 3) ;

  y0 = -(geom->GetRailsDistanceFromIP() - geom->GetRailRoadSize(1) -
	 geom->GetCradleWheel(1)/2) ;
  for (i=0; i<2; i++) {
    z0 = (2*i-1) * ((geom->GetOuterBoxSize(2) + geom->GetCradleWheel(2))/ 2.0 +
                    geom->GetCradleWall(2));
    for (j=0; j<2; j++) {
      copy = 2*i + j;
      x0 = (2*j-1) * geom->GetDistanceBetwRails()  / 2.0 ;
      gMC->Gspos("PWHE", copy, "ILCM", x0, y0, z0, 0, "ONLY") ; 
    }
  }

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
    TGeoPNEntry *alignableEntry = gGeoManager->GetAlignableEntryByUID(modUID) ;

    Float_t angle = GetGeometry()->GetPVBARAngle(iModule);
    TGeoHMatrix* globMatrix = alignableEntry->GetGlobalOrig();

    TGeoHMatrix *matTtoL = new TGeoHMatrix;
    matTtoL->RotateZ(-90.+angle);
    matTtoL->MultiplyLeft(&(globMatrix->Inverse()));
    alignableEntry->SetMatrix(matTtoL);
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
    TGeoPNEntry *alignableEntry = gGeoManager->GetAlignableEntryByUID(modUID) ;

    Float_t angle = GetGeometry()->GetPVBARAngle(iModule);
    TGeoHMatrix* globMatrix = alignableEntry->GetGlobalOrig();

    TGeoHMatrix *matTtoL = new TGeoHMatrix;
    matTtoL->RotateZ(-90.+angle);
    matTtoL->MultiplyLeft(&(globMatrix->Inverse()));
    alignableEntry->SetMatrix(matTtoL);
    
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
